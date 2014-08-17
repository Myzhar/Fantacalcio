#ifndef QSEASON_H
#define QSEASON_H

#include <QObject>
#include <QList>
#include "qteam.h"

class QSeason : public QObject
{
    Q_OBJECT
public:
    explicit QSeason(QObject *parent = 0);

    bool init2014_2015(int giornata);

    QTeam* getTeam(int idx){return mTeams[idx];}
    void addTeam( QTeam* team ){mTeams << team;}
signals:

public slots:

private:
    QList<QTeam*> mTeams;

    int mGiornata;
};

#endif // QSEASON_H
