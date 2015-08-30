#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "JsonTableModel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();    

private slots:
    void fromTextEdited(const QString&);
    void toTextEdited(const QString&);
    void fromSelectionChanged();
    void toSelectionChanged();
    void buttonFindFlightsPressed();
    void findFlights();

private:
    Ui::MainWindow *ui;
    JsonTablemodel *mOutboundFlights;
};

#endif // MAINWINDOW_H
