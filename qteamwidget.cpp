#include "qteamwidget.h"
#include "ui_qteamwidget.h"
#include <QStringList>

QTeamWidget::QTeamWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QTeamWidget)
{
    ui->setupUi(this);

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

QTeamWidget::~QTeamWidget()
{
    delete ui;
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
