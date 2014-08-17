#include "include\qplayer.h"
#include <QFile>
#include <QStringList>

#define BUF_SIZE 1024

QPlayer::QPlayer(QObject *parent) :
    QObject(parent)
{
    mCodice = -1;
    mRuolo = POR;
    mNome = "---";
    mSquadra = "---";
    mGiornAcq = -1;
    mGiocate = -1;
    mAmm = -1;
    mEsp = -1;
    mGolFatti = -1;
    mGolSubiti = -1;
    mAssTot = -1;
    mAssMov = -1;
    mAssFer = -1;
    mRigPar = -1;
    mRigSbag = -1;
    mRigSegn = -1;
    mAutogol = -1;
    mGolWin = -1;
    mMediaVec[MIL]=0.0f;
    mMediaVec[NAP]=0.0f;
    mMediaVec[ROM]=0.0f;
    mMediaFCVec[MIL]=0.0f;
    mMediaFCVec[NAP]=0.0f;
    mMediaFCVec[ROM]=0.0f;
    mMedia = 0.0f;
    mMediaFC = 0.0f;

    mValAcq13_14 = 0;
    mNuovoAcq = false;
    mValIniz14_15 = 0;
}

bool QPlayer::setInizParam(int codice, bool nuovoAcq, int valore)
{
    mCodice = codice;
    mNuovoAcq = nuovoAcq;

    if(mNuovoAcq)
    {
        mValAcq13_14 = 0;
        mValIniz14_15 = valore;
        return inizDaFile();
    }
    else
    {
        mValAcq13_14 = valore;
        return calcolaValore1314();
    }
}

bool QPlayer::inizDaFile()
{
    // >>>>> parsing Napoli per cercare nome ruolo e squadra
    QFile napoli;
    QString napoliPath = tr("%1/%2").arg(VOTI_PATH).arg(LISTA_INIZ); // TODO inserire file mercato iniziale
    napoli.setFileName( napoliPath );
    napoli.open( QIODevice::ReadOnly );
    while(1)
    {
        char buf[BUF_SIZE];

        qint64 lineLength = napoli.readLine(buf, sizeof(buf));
        if (lineLength == -1)
        {
            qDebug() << tr("Giocatore %1 non trovato").arg(mCodice);
            napoli.close();
            return false;
        }

        QString line = buf;
        QStringList strings = line.split(",");

        if( strings.size() != 7 ) // TODO Verificare!
            continue;

        bool ok;
        int cod = strings[0].toInt(&ok);
        if( !ok || cod != mCodice )
            continue;

        if( strings[1].compare("P",Qt::CaseInsensitive)==0 )
            mRuolo = POR;
        else if( strings[1].compare("D",Qt::CaseInsensitive)==0 )
            mRuolo = DIF;
        else if( strings[1].compare("C",Qt::CaseInsensitive)==0 )
            mRuolo = CEN;
        else if( strings[1].compare("A",Qt::CaseInsensitive)==0 )
            mRuolo = ATT;

        mNome = strings[2];

        mSquadra = strings[3];
    }
    // <<<<< parsing Napoli per cercare nome ruolo e squadra
}

bool QPlayer::calcolaValore1314()
{
    if( mCodice==-1 )
        return false;

    // >>>>> parsing Milano per cercare medie Milano
    QFile milano;
    QString milanoPath = tr("%1/%2").arg(STAT_INIZ_PATH).arg(STAT_MIL);
    milano.setFileName( milanoPath );
    milano.open( QIODevice::ReadOnly );
    while(1)
    {
        char buf[BUF_SIZE];

        qint64 lineLength = milano.readLine(buf, sizeof(buf));
        if (lineLength == -1)
        {
            qDebug() << tr("Giocatore %1 non trovato").arg(mCodice);
            milano.close();
            return false;
        }

        QString line = buf;
        QStringList strings = line.split(",");

        if( strings.size() != 22 ) // TODO Verificare!
            continue;

        bool ok;
        int cod = strings[0].toInt(&ok);
        if( !ok || cod != mCodice )
            continue;

        mMediaVec[MIL] = strings[17].toFloat( &ok );
        if( !ok )
            continue;

        mMediaFCVec[MIL] = strings[19].toFloat( &ok );
        if( !ok )
            continue;
    }
    // <<<<< parsing Milano per cercare medie Milano

    // >>>>> parsing Napoli per cercare medie Napoli e bonus/malus
    QFile napoli;
    QString napoliPath = tr("%1/%2").arg(STAT_INIZ_PATH).arg(STAT_NAP);
    napoli.setFileName( napoliPath );
    napoli.open( QIODevice::ReadOnly );
    while(1)
    {
        char buf[BUF_SIZE];

        qint64 lineLength = napoli.readLine(buf, sizeof(buf));
        if (lineLength == -1)
        {
            qDebug() << tr("Giocatore %1 non trovato").arg(mCodice);
            napoli.close();
            return false;
        }

        QString line = buf;
        QStringList strings = line.split(",");

        if( strings.size() != 22 ) // TODO Verificare!
            continue;

        bool ok;
        int cod = strings[0].toInt(&ok);
        if( !ok || cod != mCodice )
            continue;

        if( strings[1].compare("P",Qt::CaseInsensitive)==0 )
            mRuolo = POR;
        else if( strings[1].compare("D",Qt::CaseInsensitive)==0 )
            mRuolo = DIF;
        else if( strings[1].compare("C",Qt::CaseInsensitive)==0 )
            mRuolo = CEN;
        else if( strings[1].compare("A",Qt::CaseInsensitive)==0 )
            mRuolo = ATT;

        mNome = strings[2];

        mSquadra = strings[3];

        mGiocate = strings[4].toInt( &ok );
        if( !ok )
            continue;

        mAmm = strings[5].toInt( &ok );
        if( !ok )
            continue;

        mEsp = strings[6].toInt( &ok );
        if( !ok )
            continue;

        mGolFatti = strings[7].toInt( &ok );
        if( !ok )
            continue;

        mGolSubiti = strings[8].toInt( &ok );
        if( !ok )
            continue;

        mAssTot = strings[9].toInt( &ok );
        if( !ok )
            continue;

        mAssMov = strings[10].toInt( &ok );
        if( !ok )
            continue;

        mAssFer = strings[11].toInt( &ok );
        if( !ok )
            continue;

        mRigPar = strings[12].toInt( &ok );
        if( !ok )
            continue;

        mRigSbag = strings[13].toInt( &ok );
        if( !ok )
            continue;

        mRigSegn = strings[14].toInt( &ok );
        if( !ok )
            continue;

        mAutogol = strings[15].toInt( &ok );
        if( !ok )
            continue;

        mGolWin = strings[16].toInt( &ok );
        if( !ok )
            continue;

        mMediaVec[NAP] = strings[17].toFloat( &ok );
        if( !ok )
            continue;

        mMediaFCVec[NAP] = strings[19].toFloat( &ok );
        if( !ok )
            continue;
    }
    // <<<<< parsing Napoli per cercare medie Napoli e bonus/malus

    // >>>>> parsing Roma per cercare medie Roma
    QFile roma;
    QString romaPath = tr("%1/%2").arg(STAT_INIZ_PATH).arg(STAT_ROM);
    roma.setFileName( romaPath );
    roma.open( QIODevice::ReadOnly );
    while(1)
    {
        char buf[BUF_SIZE];

        qint64 lineLength = roma.readLine(buf, sizeof(buf));
        if (lineLength == -1)
        {
            qDebug() << tr("Giocatore %1 non trovato").arg(mCodice);
            roma.close();
            return false;
        }

        QString line = buf;
        QStringList strings = line.split(",");

        if( strings.size() != 22 ) // TODO Verificare!
            continue;

        bool ok;
        int cod = strings[0].toInt(&ok);
        if( !ok || cod != mCodice )
            continue;

        mMediaVec[ROM] = strings[17].toFloat( &ok );
        if( !ok )
            continue;

        mMediaFCVec[ROM] = strings[19].toFloat( &ok );
        if( !ok )
            continue;
    }
    // <<<<< parsing Roma per cercare medie Roma

    // >>>>> calcolo media e media FC
    float sum = 0.0f;
    float sumFC = 0.0f;
    for( int i=0; i<3; i++ )
    {
        sum += mMediaVec[i];
        sumFC += mMediaFCVec[i];
    }
    mMedia = sum/3.0f;
    mMediaFC = sumFC/3.0f;
    // <<<<< calcolo media e media FC

    // TODO applicare formula 2013/2014

}