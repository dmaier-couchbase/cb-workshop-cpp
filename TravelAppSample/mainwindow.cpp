#include <QDebug>
#include <QTimer>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cbdatasourcefactory.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mOutboundFlights(new JsonTablemodel(this))
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
    ui->outboundFlightsTableView->setModel(mOutboundFlights);
}

MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::findFlights()
{
    QString from = ui->fromLineEdit->text();
    QString to = ui->toLineEdit->text();

    QString queryPrep = "SELECT faa as fromAirport,geo FROM `travel-sample` WHERE airportname = '" + from +
        "' UNION SELECT faa as toAirport,geo FROM `travel-sample` WHERE airportname = '" + to + "'";

    N1clResult result = CBDataSourceFactory::Instance().QueryN1cl(queryPrep);

    // TODO: error checking

    QString queryTo;
    QString queryFrom;

    for (QList<QJsonObject>::const_iterator it = result.items.cbegin(); it != result.items.cend(); ++it)
    {
        if (it->contains("toAirport"))
        {
            queryTo = it->value("toAirport").toString();
        }
        else if (it->contains("fromAirport"))
        {
            queryFrom = it->value("fromAirport").toString();
        }
    }

    // TODO: Price

    // TODO: day
    queryPrep = "SELECT r.id, a.name, s.flight, s.utc, r.sourceairport, r.destinationairport, r.equipment FROM `travel-sample` r UNNEST r.schedule s JOIN `travel-sample` a ON KEYS r.airlineid WHERE r.sourceairport='" + queryFrom +
    "' AND r.destinationairport='" + queryTo + "' AND s.day=" + QString::number(6) + " ORDER BY a.name";

    result = CBDataSourceFactory::Instance().QueryN1cl(queryPrep);

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
