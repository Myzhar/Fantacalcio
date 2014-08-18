#ifndef QCESSIONEDLG_H
#define QCESSIONEDLG_H

#include <QDialog>

namespace Ui {
class QCessioneDlg;
}

class QCessioneDlg : public QDialog
{
    Q_OBJECT

public:
    explicit QCessioneDlg(QString nome, QString squadra, int val_iniz, int rivalutaz, QWidget *parent = 0);
    ~QCessioneDlg();

    bool getSvalutAttiva();

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_checkBox_clicked();

private:
    Ui::QCessioneDlg *ui;

    int mRival;
    bool mSvalAttiva;
};

#endif // QCESSIONEDLG_H
