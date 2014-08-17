#ifndef QTEAM_H
#define QTEAM_H

#include <QObject>
#include "qplayer.h"

class QTeam : public QObject
{
    Q_OBJECT

public:
    explicit QTeam(QObject *parent = 0);

    bool aggiungiGioc( int codice, bool nuovoAcq, int valore );

signals:

public slots:

private:
    QString mNome;
    QString mPres;

    float mBudget;

    QList<QPlayer> mPorList;
    QList<QPlayer> mDifList;
    QList<QPlayer> mCenList;
    QList<QPlayer> mAttList;
};

#endif // QTEAM_H
