#ifndef QPLAYER_H
#define QPLAYER_H

#include <QObject>
#include "config.h"

typedef enum _Ruolo
{
    POR,
    DIF,
    CEN,
    ATT
} Ruolo;

class QPlayer : public QObject
{
    Q_OBJECT

public:
    explicit QPlayer(QObject *parent = 0);

private:
    bool calcolaValore1314();
    bool inizDaFile( );

public:
    bool setInizParam( int codice, bool nuovoAcq, int valore );

signals:

public slots:

private:
    int mCodice;
    Ruolo mRuolo;
    QString mNome;
    QString mSquadra;
    int mGiornAcq;
    int mGiocate;
    int mAmm;
    int mEsp;
    int mGolFatti;
    int mGolSubiti;
    int mAssTot;
    int mAssMov;
    int mAssFer;
    int mRigPar;
    int mRigSbag;
    int mRigSegn;
    int mAutogol;
    int mGolWin;
    float mMediaVec[3];
    float mMediaFCVec[3];
    float mMedia;
    float mMediaFC;

    int mValAcq13_14;
    bool mNuovoAcq;
    int mValIniz14_15;
};

#endif // QPLAYER_H
