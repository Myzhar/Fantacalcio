#include "include\qplayer.h"
#include <QFile>
#include <QStringList>
#include <QDebug>

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

bool QPlayer::setInizParam(int codice, bool nuovoAcq, int valore, int giornAcq)
{
    mCodice = codice;
    mNuovoAcq = nuovoAcq;
    mGiornAcq = giornAcq;

    if(mNuovoAcq)
    {
        mValAcq13_14 = 0;
        mValIniz14_15 = valore;

        mValore = mValIniz14_15;

        return inizDaFile( giornAcq );
    }
    else
    {
        mValAcq13_14 = valore;

        mValore = mValIniz14_15;

        return calcolaValore1314();
    }
}

int QPlayer::cercaCodiceCorretto(QString nome, int giornata)
{
    // >>>>> parsing Napoli per cercare medie Napoli e bonus/malus
    QFile napoli;
    QString napoliPath;
    if( giornata < 0 )
    {
        napoliPath = tr("%1/%2").arg(STAT_INIZ_PATH).arg(STAT_NAP);
    }
    else
    {
        QString fileVoti = tr(VOTI_NAP).arg(giornata);
        napoliPath = tr("%1/%2").arg(VOTI_PATH).arg(fileVoti);
    }

    napoli.setFileName( napoliPath );
    napoli.open( QIODevice::ReadOnly );

    char buf[BUF_SIZE];
    while(1)
    {
        qint64 lineLength = napoli.readLine(buf, sizeof(buf));
        if (lineLength == -1)
        {
            qDebug() << tr("Giocatore %1 non trovato").arg(nome);
            napoli.close();
            return -1;
        }

        QString line = buf;
        QStringList strings = line.split(",");

        if( strings.size() != 7 ) // TODO Verificare con file voti (dovrebbe essere 20)!
            continue;

        if( nome.compare( strings[2],Qt::CaseInsensitive)==0 )
            return strings[0].toInt();
    }
}

bool QPlayer::inizDaFile( int giornata )
{
    // >>>>> parsing Napoli per cercare nome ruolo e squadra
    QString fileVoti = tr(VOTI_NAP).arg(giornata);

    QFile napoli;
    QString napoliPath = tr("%1/%2").arg(VOTI_PATH).arg(fileVoti);
    napoli.setFileName( napoliPath );
    if( !napoli.open( QIODevice::ReadOnly ) )
    {
        qDebug() << tr("File %1 non trovato").arg(napoliPath);
        return false;
    }

    char buf[BUF_SIZE];
    while(1)
    {
        qint64 lineLength = napoli.readLine(buf, sizeof(buf));
        if (lineLength == -1)
        {
            qDebug() << tr("Giocatore %1 non trovato").arg(mCodice);
            napoli.close();
            return false;
        }

        QString line = buf; // NOTE: occhio ai numeri con la virgola "xx,x"
        line.replace("\"","");

        QStringList strings = line.split(",");

        if( strings.size() != 7 ) // TODO Verificare con file voti (dovrebbe essere 20)!
            continue;

        bool ok;
        int cod = strings[0].toInt(&ok);
        if( !ok || cod != mCodice )
            continue;

        //qDebug() << tr("Giocatore %1 trovato").arg(mCodice);

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

        break;
    }
    // <<<<< parsing Napoli per cercare nome ruolo e squadra

    if( giornata >0 )
    {
        // TODO caricare valori da statistiche

        mValore = calcolaRivalutazione( giornata );
    }

    return true;
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
            qDebug() << tr("Milano -> Giocatore %1 non trovato").arg(mCodice);
            milano.close();
            return false;
        }

        QString line = buf;
        QStringList strings = line.split(",");

        if( strings.size() != 21 )
            continue;

        bool ok;
        int cod = strings[0].toInt(&ok);
        if( !ok || cod != mCodice )
            continue;

        // qDebug() << tr("Milano -> Giocatore %1 trovato").arg(mCodice);

        mMediaVec[MIL] = strings[17].toFloat( &ok );
        if( !ok )
            continue;

        mMediaFCVec[MIL] = strings[19].toFloat( &ok );
        if( !ok )
            continue;

        break;
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
            qDebug() << tr("Napoli -> Giocatore %1 non trovato").arg(mCodice);
            napoli.close();
            return false;
        }

        QString line = buf;
        QStringList strings = line.split(",");

        if( strings.size() != 21 )
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

        //qDebug() << tr("Napoli -> Giocatore %1 trovato - Ruolo: %2").arg(mCodice).arg(mRuolo);


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

        break;
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
            qDebug() << tr("Roma -> Giocatore %1 non trovato").arg(mCodice);
            roma.close();
            return false;
        }

        QString line = buf;
        QStringList strings = line.split(",");

        if( strings.size() != 21 )
            continue;

        bool ok;
        int cod = strings[0].toInt(&ok);
        if( !ok || cod != mCodice )
            continue;

        //qDebug() << tr("Roma -> Giocatore %1 trovato").arg(mCodice);

        mMediaVec[ROM] = strings[17].toFloat( &ok );
        if( !ok )
            continue;

        mMediaFCVec[ROM] = strings[19].toFloat( &ok );
        if( !ok )
            continue;

        break;
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

    // >>>>> applicare formula 2013/2014
    /*
    Difensore:
    gol_f = 2
    assist_f  = 2

    Centrocampista:
    gol_f = 1
    assist_f = 2

    Attaccante:
    gol_f = 1
    assist_f = 1

    Fattore costo:
    valore < 10: bonus_mul = 7.5
    valore < 20: bonul_mul = 5
    valore < 30: bonus_mul = 2.5
    valore > 80: malus_mul = 2.5
    valore > 60: malus_mul = 2
    valore > 40: malus_mul = 1.5

    presenze: valore x totpresenze x 0.01
    assenze:  valore x * (ultima_giornata-totpresenze) x 0.025
    gol: valore x tot_golsegnati x gol_f x 0.025
    rig_parato: valore x totrigp x 0.10
    rig_sbagliato: valore x totrigsb x 0.05
    autogol: valore x totautogol x 0.05
    assist: valore x totass  x assist_f x 0.015
    espulsione: valore x totesp x 0.01
    ammonizione: valore x totamm x 0.005
    gol_subito: valore x tot_golsubiti x 0.005
    media_voto: valore x (media_giornale - 6.0) x 0.1

    bonus_portiere: 1.0 - (tot_golsubiti/totpresenze)
    malus_attaccante: (1.0 – (tot_golsegnati/totpresenze))

    totale_bonus = (presenze+gol+rig_parato+assist+media_voto-assenze-gol_subito)*bonus_mul;
    totale_malus = (rig_sbagliato+espulsione+ammonizione)*malus_mul;

    Valore_finale: valore + (totale_bonus - totale_malus)
    */
    float gol_factor=1.0f,ass_factor=1.0f;
    float bonus_mul=1.0f,malus_mul=1.0f;
    float port = 0.0f;
    float att = 0.0f;

    if( mRuolo==POR || mRuolo==DIF )
    {
        gol_factor = 2.0f;
        ass_factor = 2.0f;
    }
    else if( mRuolo==CEN )
    {
        gol_factor = 1.0f;
        ass_factor = 2.0f;
    }

    if(mValAcq13_14 < 10 )
        bonus_mul = 7.5f;
    else if(mValAcq13_14 < 20 )
        bonus_mul = 5.0f;
    else if(mValAcq13_14 < 30 )
        bonus_mul = 2.5f;
    else if(mValAcq13_14 > 80 )
        malus_mul = 2.5f;
    else if(mValAcq13_14 > 60 )
        malus_mul = 2.0f;
    else if(mValAcq13_14 > 40 )
        malus_mul = 1.5f;

    float presenze = (float)mValAcq13_14 * (float)mGiocate * 0.01f;
    float assenze = (float)mValAcq13_14 * (float)(38-mGiocate) * 0.025f;
    float gol = (float)mValAcq13_14 * (float)mGolFatti * gol_factor * 0.025f;
    float rig_par = (float)mValAcq13_14 * (float)mRigPar * 0.1f;
    float rig_sbag = (float)mValAcq13_14 * (float)mRigSbag * 0.05f;
    float autogol = (float)mValAcq13_14 * (float)mAutogol * 0.05f;
    float assist = (float)mValAcq13_14 * (float)mAssTot * ass_factor * 0.015f;
    float esp = (float)mValAcq13_14 * (float)mEsp * 0.01f;
    float amm = (float)mValAcq13_14 * (float)mAmm * 0.005f;
    float gol_sub = (float)mValAcq13_14 * (float)mGolSubiti * 0.005f;
    float media = (float)mValAcq13_14 * ( mMedia - 6.0f) * 0.1f;

    if( mRuolo==POR )
        port = 1.0f-(mGolSubiti/mGiocate);
    if( mRuolo==ATT )
        att = 1.0f-(mGolFatti/mGiocate);

    float giorn_factor = ((float)(39-mGiornAcq))/38.0f;

    float bonus = (presenze+gol+rig_par+assist+port+(media>0.0f?media:0.0f))*bonus_mul*giorn_factor;
    float malus = (rig_sbag+esp+amm+att+assenze+gol_sub+autogol+att+(media<0.0f?-media:0.0f))*malus_mul*giorn_factor;

    mValIniz14_15 = mValAcq13_14 + (bonus - malus);
    if( mValIniz14_15 <=0.0f )
        mValIniz14_15 = 1.0f;
    // <<<<< applicare formula 2013/2014

    mValore = mValIniz14_15;
    //mValore = calcolaRivalutazione( 38 );

    return true;
}

int QPlayer::calcolaRivalutazione( int giornata )
{
    int val_iniz = mValIniz14_15;
    // int val_iniz = mValAcq13_14; // Usato per debuggare la formula!!!

    float gol_factor=1.0f,ass_factor=1.0f;
    float bonus_mul=1.0f,malus_mul=1.0f;

    if( mRuolo==POR || mRuolo==DIF )
    {
        gol_factor = 2.0f;
        ass_factor = 2.0f;
    }
    else if( mRuolo==CEN )
    {
        gol_factor = 1.5f;
    }
    else if( mRuolo==ATT )
    {
        ass_factor = 1.5f;
    }

    if(val_iniz < 10 )
        bonus_mul = 5.0f;
    else if(val_iniz < 20 )
        bonus_mul = 3.0f;
    else if(val_iniz < 30 )
        bonus_mul = 1.5f;
    else if(val_iniz > 250 )
        malus_mul = 3.5f;
    else if(val_iniz > 150 )
        malus_mul = 2.5f;
    else if(val_iniz > 100 )
        malus_mul = 2.0f;
    else if(val_iniz > 80 )
        malus_mul = 1.75f;
    else if(val_iniz > 60 )
        malus_mul = 1.5f;
    else if(val_iniz > 40 )
        malus_mul = 1.25f;

    float presenze = ((float)val_iniz * 0.015f) * (float)mGiocate;               // 15% ogni 10 giornate giocate - max 57%
    float gol =      ((float)val_iniz * 0.025f) * (float)mGolFatti * gol_factor; // 2.5% ogni gol fatto - max 60%/75%
    float rig_par =  ((float)val_iniz * 0.15f)  * (float)mRigPar;                // 15% ogni rigore parato - max 45%/75%
    float assist =   ((float)val_iniz * 0.02f)  * (float)mAssTot * ass_factor;   // 2% ogni assist fatto - max 20%/30%

    float assenze =  ((float)val_iniz * 0.02f)  * (float)(giornata-mGiocate);    // 20% ogni 10 giornate non giocate - max 76%
    float autogol =  ((float)val_iniz * 0.075f) * (float)mAutogol;               // 7.5% ogni autogol - max 15%/22.5%
    float rig_sbag = ((float)val_iniz * 0.1f)   * (float)mRigSbag;               // 10% ogni rigore sbagliato - max 30%/50%
    float esp =      ((float)val_iniz * 0.04f)  * (float)mEsp;                   // 4% ogni espulsione - max 30%/50%
    float amm =      ((float)val_iniz * 0.02f)  * (float)mAmm;                   // 2% ogni ammonizione - max 20%/30%
    float gol_sub =  ((float)val_iniz * 0.015f) * (float)mGolSubiti;             // 1.5% ogni gol subito - max 40%/50%

    float media =    ((float)val_iniz * 0.4f)   * (mMedia - 6.0f);               // 20% ogni 0.5 punti di media

    /*
    float port = 0.0f;
    float att = 0.0f;
    if( mRuolo==POR )
        port = ((float)val_iniz * 0.02f) * (1.0f-(mGolSubiti/mGiocate)); // 2% ogni 0.1 punti di media gol subiti
    if( mRuolo==ATT )
        att =  ((float)val_iniz * 0.02f) * (1.0f-(mGolFatti/mGiocate));  // 2% ogni 0.1 punti di media gol fatti*/

    float giorn_factor = ((float)(38-mGiornAcq))/38.0f; // Fattore di riduzione in base alla giornata di acquisto

    float bonus = (presenze+gol+rig_par+assist+(media>0.0f?media:0.0f))*bonus_mul*giorn_factor;
    float malus = (assenze+autogol+rig_sbag+esp+amm+gol_sub+(media<0.0f?-media:0.0f))*malus_mul*giorn_factor;

    int nuovo_valore = val_iniz + (bonus - malus);
    if( nuovo_valore <=0.0f )
        nuovo_valore = 1.0f;

    return nuovo_valore;
}

