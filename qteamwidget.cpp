#include "qteamwidget.h"
#include "ui_qteamwidget.h"
#include <QStringList>

QTeamWidget::QTeamWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QTeamWidget)
{
    ui->setupUi(this);

    initTable();
}

QTeamWidget::~QTeamWidget()
{
    delete ui;
}

void QTeamWidget::initTable()
{
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(25);
    ui->tableWidget->setColumnCount(20);

    QStringList horHead;
    horHead << "Cod" << "Nome" << "Squadra"
            << "Valore" << "Val 13/14" << "Val. Iniz."
            << "G.Acq" << "Pres" << "Amm" << "Esp"
            << "GF" << "GS" << "Ass" << "Rig. Par."
            << "R-" << "R+" << "Aut" << "GW"
            << "Media" << "Media FC";

    ui->tableWidget->setHorizontalHeaderLabels( horHead );

    QStringList verHead;

    verHead << "P" << "P" << "P"
               << "D" << "D" << "D" << "D" << "D" << "D" << "D" << "D"
               << "C" << "C" << "C" << "C" << "C" << "C" << "C" << "C"
               << "A" << "A" << "A" << "A" << "A" << "A";

    ui->tableWidget->setVerticalHeaderLabels( verHead);

    QHeaderView* header = ui->tableWidget->horizontalHeader();
    //header->setSectionResizeMode(QHeaderView::Stretch);
    header->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void QTeamWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

QTableWidget* QTeamWidget::getTablePtr()
{
    return ui->tableWidget;
}

void QTeamWidget::setInfo( int budget, int capitale, int daComprare, int maxSpesa )
{
    ui->lineEdit_budge_iniz->setText(tr("%1").arg(budget));
    ui->lineEdit_budge_capitale->setText(tr("%1").arg(capitale));
    ui->lineEdit_gioc_da_compr->setText(tr("%1").arg(daComprare));
    ui->lineEdit_max_sp_gioc->setText(tr("%1").arg(maxSpesa));
}


void QTeamWidget::on_tableWidget_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column);

    bool ok;
    int codice = ui->tableWidget->item( row, 0 )->text().toInt( &ok );

    if( !ok )
        emit aggiungiGiocatore();
    else
        emit eliminaGiocatore( codice );
}
