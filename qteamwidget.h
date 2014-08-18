#ifndef QTEAMWIDGET_H
#define QTEAMWIDGET_H

#include <QWidget>
#include <QTableWidget>

namespace Ui {
class QTeamWidget;
}

class QTeamWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QTeamWidget(QWidget *parent = 0);
    ~QTeamWidget();

    QTableWidget* getTablePtr();
    void initTable();
    void setInfo(int budget, int capitale, int daComprare, int maxSpesa );

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_tableWidget_cellDoubleClicked(int row, int column);

signals:
    void aggiungiGiocatore();
    void eliminaGiocatore( int codice );

private:
    Ui::QTeamWidget *ui;
};

#endif // QTEAMWIDGET_H
