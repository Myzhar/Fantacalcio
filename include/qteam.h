#ifndef QTEAM_H
#define QTEAM_H

#include <QObject>
#include <QList>
#include <QTableWidget>

#include "qplayer.h"

class QTeam : public QObject
{
    Q_OBJECT

public:
    explicit QTeam(QObject *parent = 0);
    //QTeam(const QTeam& team);

    void setParams(QString nome, QString pres, int startBudget );
    void getParams( QString& nome, QString& pres, int& startBudget, int& currBudget );
    bool aggiungiGioc( int codice, bool nuovoAcq, int valore );

    void salvaSquadra(int giornata);
    bool caricaSquadra(int giornata);
    void aggiornaTabella( QTableWidget* tableWidget );

protected:

signals:

public slots:

private:
    QString mNome;
    QString mPres;

    int mStartBudget;
    int mBudget;

    QList<QPlayer*> mPorList;
    QList<QPlayer*> mDifList;
    QList<QPlayer*> mCenList;
    QList<QPlayer*> mAttList;
};

#endif // QTEAM_H
