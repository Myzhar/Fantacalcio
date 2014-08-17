#ifndef QTEAMWIDGET_H
#define QTEAMWIDGET_H

#include <QWidget>

namespace Ui {
class QTeamWidget;
}

class QTeamWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QTeamWidget(QWidget *parent = 0);
    ~QTeamWidget();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QTeamWidget *ui;
};

#endif // QTEAMWIDGET_H
