#include "include\qteam.h"
#include <QDebug>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QTableWidget>

#define BUF_SIZE 1024

QTeam::QTeam(QObject *parent) :
    QObject(parent)
{
}

//QTeam::QTeam(const QTeam& team)
//{
//    mNome = team.mNome;
//    mPres = team.mPres;
//    mStartBudget = team.mStartBudget;
//    mBudget = team.mBudget;

//    mPorList = team.mPorList;
//    mDifList = team.mDifList;
//    mCenList = team.mCenList;
//    mAttList = team.mAttList;
//}

void QTeam::setParams( QString nome, QString pres, int startBudget )
{
    mNome = nome;
    mPres = pres;
    mStartBudget = startBudget;
    mBudget = mStartBudget;
}

void QTeam::getParams( QString& nome, QString& pres, int& startBudget, int& currBudget )
{
    nome = mNome;
    pres = mPres;
    startBudget = mStartBudget;
    currBudget = mBudget;
}

bool QTeam::aggiungiGioc( int codice, bool nuovoAcq, int valore )
{
    QPlayer* gioc = new QPlayer();
    if( !gioc->setInizParam( codice, nuovoAcq, valore ) )
        return false;

    if( gioc->mRuolo == POR )
        mPorList << gioc;
    else if( gioc->mRuolo == DIF )
        mDifList << gioc;
    else if( gioc->mRuolo == CEN )
        mCenList << gioc;
    else if( gioc->mRuolo == ATT )
        mAttList << gioc;

    return true;
}

void QTeam::salvaSquadra(int giornata)
{
    QFile squadra;
    QString path = tr("%1/%2_G%3.txt").arg(SQ_PATH).arg(mPres).arg(giornata);
    squadra.setFileName( path );
    squadra.open( QIODevice::WriteOnly );

    // TODO Salvare la Squadra
}

bool QTeam::caricaSquadra(int giornata)
{
    QFile squadra;
    QString path = tr("%1/%2_G%3.txt").arg(SQ_PATH).arg(mPres).arg(giornata);
    squadra.setFileName( path );
    if( !squadra.open( QIODevice::ReadOnly ) )
        return false;

    char buf[BUF_SIZE];
    while(1)
    {
        qint64 lineLength = squadra.readLine(buf, sizeof(buf));
        if (lineLength == -1)
        {
            qDebug() << tr("File %1 terminato").arg(path);
            squadra.close();
            break;
        }

        QString line = buf;
        QStringList strings = line.split(",");

        if( strings.size()==2 && strings[0].compare( "CREDITI", Qt::CaseInsensitive)==0 )
        {
            mBudget = strings[1].toInt();
            continue;
        }

        if( strings.size()==4 )
        {
            int codice = strings[1].toInt();
            bool nuovo = strings[2].toInt()==1;
            int val = strings[3].toInt();

            aggiungiGioc( codice, nuovo, val );
        }
    }

    return true;
}


