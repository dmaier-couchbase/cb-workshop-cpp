#include <QDebug>
#include <QTimer>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cbdatasourcefactory.h"
#include "login.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mOutboundFlights(new JsonTablemodel(this)),
    userDocumentKey("")
{
    ui->setupUi(this);
    connect(ui->fromLineEdit, SIGNAL(textEdited(const QString&)), SLOT(fromTextEdited(const QString&)));
    connect(ui->toLineEdit, SIGNAL(textEdited(const QString&)), SLOT(toTextEdited(const QString&)));
    connect(ui->findPushButton, SIGNAL(clicked(bool)), SLOT(buttonFindFlightsPressed()));
    connect(ui->acFromlistWidget, SIGNAL(itemSelectionChanged()), SLOT(fromSelectionChanged()));
    connect(ui->acTolistWidget, SIGNAL(itemSelectionChanged()), SLOT(toSelectionChanged()));

    mOutboundFlights->addMapping("name", "Airline");
    mOutboundFlights->addMapping("flight", "Flight");
    mOutboundFlights->addMapping("utc", "Departure");
    mOutboundFlights->addMapping("sourceairport", "From");
    mOutboundFlights->addMapping("destinationairport", "To");
    mOutboundFlights->addMapping("equipment", "Aircraft");
    mOutboundFlights->addMapping("price", "Price");
    ui->outboundFlightsTableView->setModel(mOutboundFlights);

    login();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::login()
{
    Login login;
    bool loggedIn = false;
    while (!loggedIn)
    {
        if (login.exec() == QDialog::Accepted)
        {
            loggedIn = login.createUserOrLogin();
            if (!loggedIn)
            {
                QMessageBox::critical(NULL, "Could not log in", "Error logging in, please try again");
            }
        }
        else
        {
            exit(0);
        }
    }
}

void MainWindow::fromTextEdited(const QString& txt)
{
    ui->acFromlistWidget->clear();
    if (txt.length() > 2)
    {
        N1clResult result = CBDataSourceFactory::Instance().QueryN1cl("SELECT airportname FROM `travel-sample` WHERE faa = '" + txt + "'");
        for (QList<QJsonObject>::const_iterator it = result.items.cbegin(); it != result.items.cend(); ++it)
        {
            QString name = (*it)["airportname"].toString();
            ui->acFromlistWidget->addItem(name);
        }
    }
}

void MainWindow::toTextEdited(const QString& txt)
{
    ui->acTolistWidget->clear();
    if (txt.length() > 2)
    {
        N1clResult result = CBDataSourceFactory::Instance().QueryN1cl("SELECT airportname FROM `travel-sample` WHERE faa = '" + txt + "'");
        for (QList<QJsonObject>::const_iterator it = result.items.cbegin(); it != result.items.cend(); ++it)
        {
            QString name = (*it)["airportname"].toString();
            ui->acTolistWidget->addItem(name);
        }
    }
}

void MainWindow::fromSelectionChanged()
{
    QList<QListWidgetItem*> items = ui->acFromlistWidget->selectedItems();
    if (items.count() > 0)
    {
        QString item = items.first()->text();
        ui->fromLineEdit->setText(item);
        ui->acFromlistWidget->clear();
    }
}

void MainWindow::toSelectionChanged()
{
    QList<QListWidgetItem*> items = ui->acTolistWidget->selectedItems();
    if (items.count() > 0)
    {
        QString item = items.first()->text();
        ui->toLineEdit->setText(item);
        ui->acTolistWidget->clear();
    }
}

void MainWindow::buttonFindFlightsPressed()
{
    ui->findPushButton->setEnabled(false);
    ui->findPushButton->setText("Please wait...");
    QTimer::singleShot(100, this, SLOT(findFlights()));
}

#define pi 3.14159265358979323846

double deg2rad(double deg)
{
  return (deg * pi / 180);
}
double rad2deg(double rad)
{
  return (rad * 180 / pi);
}

double distance(double lat1, double lon1, double lat2, double lon2, char unit)
{
  double theta, dist;
  theta = lon1 - lon2;
  dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(theta));
  dist = acos(dist);
  dist = rad2deg(dist);
  dist = dist * 60 * 1.1515;
  switch(unit) {
    case 'M':
      break;
    case 'K':
      dist = dist * 1.609344;
      break;
    case 'N':
      dist = dist * 0.8684;
      break;
  }
  return (dist);
}

void MainWindow::findFlights()
{
    QString from = ui->fromLineEdit->text();
    QString to = ui->toLineEdit->text();

    QString queryPrep = "SELECT faa as fromAirport,geo FROM `travel-sample` WHERE airportname = '" + from +
        "' UNION SELECT faa as toAirport,geo FROM `travel-sample` WHERE airportname = '" + to + "'";

    N1clResult result = CBDataSourceFactory::Instance().QueryN1cl(queryPrep);

    // TODO: error checking

    QString queryFrom;
    double startLat;
    double startLon;

    QString queryTo;
    double endLat;
    double endLon;

    for (QList<QJsonObject>::const_iterator it = result.items.cbegin(); it != result.items.cend(); ++it)
    {
        if (it->contains("toAirport"))
        {
            queryTo = it->value("toAirport").toString();
            endLat = it->value("geo").toObject().value("lat").toDouble();
            endLon = it->value("geo").toObject().value("lon").toDouble();
        }
        else if (it->contains("fromAirport"))
        {
            queryFrom = it->value("fromAirport").toString();
            startLat = it->value("geo").toObject().value("lat").toDouble();
            startLon = it->value("geo").toObject().value("lon").toDouble();
        }
    }

    double dist = distance(startLat, startLon, endLat, endLon, 'K');
    double flightTime = round(dist / 800); // avgKmHour
    double price = round(dist * 0.1); // distanceCostMultiplier

    // TODO: day
    queryPrep = "SELECT r.id, a.name, s.flight, s.utc, r.sourceairport, r.destinationairport, r.equipment FROM `travel-sample` r UNNEST r.schedule s JOIN `travel-sample` a ON KEYS r.airlineid WHERE r.sourceairport='" + queryFrom +
    "' AND r.destinationairport='" + queryTo + "' AND s.day=" + QString::number(6) + " ORDER BY a.name";

    result = CBDataSourceFactory::Instance().QueryN1cl(queryPrep);

    for (QList<QJsonObject>::iterator it = result.items.begin(); it != result.items.end(); ++it)
    {
        it->insert("flighttime", flightTime);
        it->insert("price", round(price * ((100 - (floor(rand() % 20 + 1))) / 100)));
    }

    mOutboundFlights->setData(result.items);

    ui->tabWidget->setCurrentIndex(1);
    ui->findPushButton->setEnabled(true);
    ui->findPushButton->setText("Find Flights");

    /*

    db.query(queryPrep, user, function (err, res) {
        if (err) {
            done(err, null);
            return;
        }
        if (res) {

            var queryTo;
            var queryFrom;
            var geoStart;
            var geoEnd;
            var flightTime;
            var price;
            var distance;

            for (i = 0; i < res.length; i++) {
                if (res[i].toAirport) {
                    queryTo = res[i].toAirport;
                    geoEnd = {longitude: res[i].geo.lon, latitude: res[i].geo.lat};
                }
                if (res[i].fromAirport) {
                    queryFrom = res[i].fromAirport;
                    geoStart = {longitude: res[i].geo.lon, latitude: res[i].geo.lat};
                }
            }

            distance = haversine(geoStart, geoEnd);
            flightTime = Math.round(distance / config.application.avgKmHr);
            price = Math.round(distance * config.application.distanceCostMultiplier);

            queryPrep = "SELECT r.id, a.name, s.flight, s.utc, r.sourceairport, r.destinationairport, r.equipment " +
            "FROM `" + config.couchbase.bucket + "` r UNNEST r.schedule s JOIN `" +
            config.couchbase.bucket + "` a ON KEYS r.airlineid WHERE r.sourceairport='" + queryFrom +
            "' AND r.destinationairport='" + queryTo + "' AND s.day=" + convDate(leave) + " ORDER BY a.name";

            db.query(queryPrep, user, function (err, flightPaths) {
                if (err) {
                    done(err, null);
                    return;
                }
                if (flightPaths) {
                    if (config.application.verbose) {
                        console.log('--↳ VERBOSE:FINDALLL:',{from:from,to:to,leave:leave},':RESULTS:COUNT:',flightPaths.length);
                    }
                    var resCount = flightPaths.length;
                    for (r = 0; r < flightPaths.length; r++) {
                        resCount--;
                        flightPaths[r].flighttime = flightTime;
                        flightPaths[r].price = Math.round(price * ((100 - (Math.floor(Math.random() * (20) + 1))) / 100));

                        if (resCount == 0) {
                            if (config.application.verbose) {
                                console.log('----↳ VERBOSE:FINDALLL:',{from:from,to:to,leave:leave},':RESULTS:RETURNING:',flightPaths.length);
                            }
                            done(null, flightPaths);
                            return;
                        }
                    }
                    if (config.application.verbose) {
                        console.log('------↳ VERBOSE:FINDALLL:',{from:from,to:to,leave:leave},':RESULTS:NOT RETURNED:',flightPaths.length);
                    }
                    return;
                }
            });
        }
    });
    */
}
