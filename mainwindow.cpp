#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include "qcessionedlg.h"
#include "qplayer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Inizializzazione stagione 2014/2015 a partire da 2013/2015
    mStag14_15.init2014_2015( ui->spinBox->value() );

    // >>>>> Inizializzazione Tabs
    for( int i=0; i<8; i++ )
    {
        QString nome, pres;
        int startBudget, budget;

        mStag14_15.getTeam(i)->getParams( nome, pres, startBudget, budget );
        ui->tabWidget_teams->setTabText( i, nome );        
    }

    mStag14_15.getTeam(0)->aggiornaTabella( ui->widget_1 );
    mStag14_15.getTeam(1)->aggiornaTabella( ui->widget_2 );
    mStag14_15.getTeam(2)->aggiornaTabella( ui->widget_3 );
    mStag14_15.getTeam(3)->aggiornaTabella( ui->widget_4 );
    mStag14_15.getTeam(4)->aggiornaTabella( ui->widget_5 );
    mStag14_15.getTeam(5)->aggiornaTabella( ui->widget_6 );
    mStag14_15.getTeam(6)->aggiornaTabella( ui->widget_7 );
    mStag14_15.getTeam(7)->aggiornaTabella( ui->widget_8 );
    // <<<<< Inizializzazione Tabs

    connect( ui->widget_1, SIGNAL(eliminaGiocatore(int)),
             this, SLOT(onEliminaGiocatore_squadra1(int)) );
    connect( ui->widget_2, SIGNAL(eliminaGiocatore(int)),
             this, SLOT(onEliminaGiocatore_squadra2(int)) );
    connect( ui->widget_3, SIGNAL(eliminaGiocatore(int)),
             this, SLOT(onEliminaGiocatore_squadra3(int)) );
    connect( ui->widget_4, SIGNAL(eliminaGiocatore(int)),
             this, SLOT(onEliminaGiocatore_squadra4(int)) );
    connect( ui->widget_5, SIGNAL(eliminaGiocatore(int)),
             this, SLOT(onEliminaGiocatore_squadra5(int)) );
    connect( ui->widget_6, SIGNAL(eliminaGiocatore(int)),
             this, SLOT(onEliminaGiocatore_squadra6(int)) );
    connect( ui->widget_7, SIGNAL(eliminaGiocatore(int)),
             this, SLOT(onEliminaGiocatore_squadra7(int)) );
    connect( ui->widget_8, SIGNAL(eliminaGiocatore(int)),
             this, SLOT(onEliminaGiocatore_squadra8(int)) );
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
    for( int i=0; i<8; i++ )
    {
        mStag14_15.getTeam(i)->salvaSquadra( ui->spinBox->value() );
    }
}

void MainWindow::on_spinBox_valueChanged(const QString &arg1)
{
    setCursor( Qt::WaitCursor);

    int giornata = arg1.toInt();

    if( giornata==-1 )
    {
        ui->pushButton_copia->setEnabled(false);
        ui->pushButton_salva->setEnabled(false);
    }
    else if( giornata==38 )
    {
        ui->pushButton_salva->setEnabled(false);
    }
    else
    {
        ui->pushButton_copia->setEnabled(true);
        ui->pushButton_salva->setEnabled(true);
    }

    QTeamWidget* wdgArray[8] = {ui->widget_1,ui->widget_2,ui->widget_3,ui->widget_4,ui->widget_5,ui->widget_6,ui->widget_7,ui->widget_8};

    for( int i=0; i<8; i++ )
    {
        if( !mStag14_15.getTeam( i )->caricaSquadra( giornata ) )
        {
            QString nome, pres;
            int startBudget, budget;

            mStag14_15.getTeam( i )->getParams( nome, pres, startBudget, budget );
            QMessageBox::warning( this, tr("Attenzione"),
                                  tr("La squadra %1 per la giornata %2 non esiste ").arg(nome).arg(giornata) );

            wdgArray[i]->initTable();
        }
        else
            mStag14_15.getTeam(i)->aggiornaTabella( wdgArray[i] );
    }

    unsetCursor();
}

void MainWindow::on_pushButton_copia_clicked()
{
    int giornata = ui->spinBox->value();
    for( int i=0; i<8; i++ )
    {
        QString nome, pres;
        int startBudget, budget;

        mStag14_15.getTeam( i )->getParams( nome, pres, startBudget, budget );

        QFile squadra;
        QString orig = tr("%1/%2_G%3.txt").arg(SQ_PATH).arg(pres).arg(giornata-1);
        QString dest = tr("%1/%2_G%3.txt").arg(SQ_PATH).arg(pres).arg(giornata);
        squadra.setFileName( orig );
        if( !squadra.copy( dest) )
        {
            QMessageBox::warning(this, tr("Errore"),
                                 tr("Impossibile copiare i file dalla giornata precedente.\r\nVerificare che non esistano già."));
            return;
        }
    }

    on_spinBox_valueChanged( tr("%1").arg(giornata));
}

void MainWindow::onEliminaGiocatore_squadra1( int codice )
{
    QPlayer* gioc = mStag14_15.getTeam( 0 )->getGiocatore( codice );

    QCessioneDlg dlg(gioc->mNome, gioc->mSquadra, gioc->mValAcquisto, gioc->mValore, this);

    if( dlg.exec()==QDialog::Accepted )
    {
        mStag14_15.getTeam(0)->cediGiocatore( codice, dlg.getSvalutAttiva() );
        mStag14_15.getTeam(0)->aggiornaTabella( ui->widget_1 );
    }
}

void MainWindow::onEliminaGiocatore_squadra2( int codice )
{
    QPlayer* gioc = mStag14_15.getTeam( 1 )->getGiocatore( codice );

    QCessioneDlg dlg(gioc->mNome, gioc->mSquadra, gioc->mValAcquisto, gioc->mValore, this);

    if( dlg.exec()==QDialog::Accepted )
    {
        mStag14_15.getTeam(1)->cediGiocatore( codice, dlg.getSvalutAttiva() );
        mStag14_15.getTeam(1)->aggiornaTabella( ui->widget_2 );
    }
}

void MainWindow::onEliminaGiocatore_squadra3( int codice )
{
    QPlayer* gioc = mStag14_15.getTeam( 2 )->getGiocatore( codice );

    QCessioneDlg dlg(gioc->mNome, gioc->mSquadra, gioc->mValAcquisto, gioc->mValore, this);

    if( dlg.exec()==QDialog::Accepted )
    {
        mStag14_15.getTeam(2)->cediGiocatore( codice, dlg.getSvalutAttiva() );
        mStag14_15.getTeam(2)->aggiornaTabella( ui->widget_3 );
    }
}

void MainWindow::onEliminaGiocatore_squadra4( int codice )
{
    QPlayer* gioc = mStag14_15.getTeam( 3 )->getGiocatore( codice );

    QCessioneDlg dlg(gioc->mNome, gioc->mSquadra, gioc->mValAcquisto, gioc->mValore, this);

    if( dlg.exec()==QDialog::Accepted )
    {
        mStag14_15.getTeam(3)->cediGiocatore( codice, dlg.getSvalutAttiva() );
        mStag14_15.getTeam(3)->aggiornaTabella( ui->widget_4 );
    }
}

void MainWindow::onEliminaGiocatore_squadra5( int codice )
{
    QPlayer* gioc = mStag14_15.getTeam( 4 )->getGiocatore( codice );

    QCessioneDlg dlg(gioc->mNome, gioc->mSquadra, gioc->mValAcquisto, gioc->mValore, this);

    if( dlg.exec()==QDialog::Accepted )
    {
        mStag14_15.getTeam(4)->cediGiocatore( codice, dlg.getSvalutAttiva() );
        mStag14_15.getTeam(4)->aggiornaTabella( ui->widget_5 );
    }
}

void MainWindow::onEliminaGiocatore_squadra6( int codice )
{
    QPlayer* gioc = mStag14_15.getTeam( 5 )->getGiocatore( codice );

    QCessioneDlg dlg(gioc->mNome, gioc->mSquadra, gioc->mValAcquisto, gioc->mValore, this);

    if( dlg.exec()==QDialog::Accepted )
    {
        mStag14_15.getTeam(5)->cediGiocatore( codice, dlg.getSvalutAttiva() );
        mStag14_15.getTeam(5)->aggiornaTabella( ui->widget_6 );
    }
}

void MainWindow::onEliminaGiocatore_squadra7( int codice )
{
    QPlayer* gioc = mStag14_15.getTeam( 6 )->getGiocatore( codice );

    QCessioneDlg dlg(gioc->mNome, gioc->mSquadra, gioc->mValAcquisto, gioc->mValore, this);

    if( dlg.exec()==QDialog::Accepted )
    {
        mStag14_15.getTeam(6)->cediGiocatore( codice, dlg.getSvalutAttiva() );
        mStag14_15.getTeam(6)->aggiornaTabella( ui->widget_7 );
    }
}

void MainWindow::onEliminaGiocatore_squadra8( int codice )
{
    QPlayer* gioc = mStag14_15.getTeam( 7 )->getGiocatore( codice );

    QCessioneDlg dlg(gioc->mNome, gioc->mSquadra, gioc->mValAcquisto, gioc->mValore, this);

    if( dlg.exec()==QDialog::Accepted )
    {
        mStag14_15.getTeam(7)->cediGiocatore( codice, dlg.getSvalutAttiva() );
        mStag14_15.getTeam(7)->aggiornaTabella( ui->widget_8 );
    }
}
