#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Inizializzazione stagione 2014/2015 a partire da 2013/2015
    mStag14_15.init2014_2015( -1 );

    // >>>>> Inizializzazione Tabs
    for( int i=0; i<8; i++ )
    {
        QString nome, pres;
        int startBudget, budget;

        mStag14_15.getTeam(i)->getParams( nome, pres, startBudget, budget );
        ui->tabWidget_teams->setTabText( i, nome );
    }
    // <<<<< Inizializzazione Tabs


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_pushButton_salva_clicked()
{
    // TODO salvare squadre per la giornata selezionata
}
