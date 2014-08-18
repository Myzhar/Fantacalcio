#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "qseason.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_pushButton_salva_clicked();
    void on_spinBox_valueChanged(const QString &arg1);
    void on_pushButton_copia_clicked();

    void onEliminaGiocatore_squadra1( int codice );
    void onEliminaGiocatore_squadra2( int codice );
    void onEliminaGiocatore_squadra3( int codice );
    void onEliminaGiocatore_squadra4( int codice );
    void onEliminaGiocatore_squadra5( int codice );
    void onEliminaGiocatore_squadra6( int codice );
    void onEliminaGiocatore_squadra7( int codice );
    void onEliminaGiocatore_squadra8( int codice );

private:
    Ui::MainWindow *ui;

    QSeason mStag14_15;
};

#endif // MAINWINDOW_H
