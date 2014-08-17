#include "include\qteam.h"

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
