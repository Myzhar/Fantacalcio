#include "include\qteam.h"

QTeam::QTeam(QObject *parent) :
    QObject(parent)
{
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
