#include "qcessionedlg.h"
#include "ui_qcessionedlg.h"

QCessioneDlg::QCessioneDlg(QString nome, QString squadra, int val_iniz, int rivalutaz, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QCessioneDlg)
{
    ui->setupUi(this);

    mRival = rivalutaz;

    ui->lineEdit_nome->setText(nome);
    ui->lineEdit_squadra->setText(squadra);
    ui->lineEdit_val_iniz->setText(tr("%1").arg(val_iniz));
    ui->lineEdit_valore->setText( tr("%1").arg(mRival));
}

QCessioneDlg::~QCessioneDlg()
{
    delete ui;
}

bool QCessioneDlg::getSvalutAttiva()
{
    return mSvalAttiva;
}

void QCessioneDlg::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void QCessioneDlg::on_checkBox_clicked()
{
    mSvalAttiva = ui->checkBox->isChecked();

    if( mSvalAttiva )
    {
        int newVal = (int)(((float)mRival)-((float)mRival*0.1f)+0.5f);
        ui->lineEdit_valore->setText( tr("%1").arg(newVal));
    }
    else
        ui->lineEdit_valore->setText( tr("%1").arg(mRival));
}
