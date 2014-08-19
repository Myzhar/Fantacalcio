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

QPlayer *QTeam::aggiungiGioc( int codice, bool nuovoAcq, int valore, int giornAcq )
{
    QPlayer* gioc = new QPlayer();
    if( !gioc->setInizParam( codice, nuovoAcq, valore, giornAcq ) )
    {
        delete gioc;
        return NULL;
    }

    if( gioc->mRuolo == POR )
        mPorList << gioc;
    else if( gioc->mRuolo == DIF )
        mDifList << gioc;
    else if( gioc->mRuolo == CEN )
        mCenList << gioc;
    else if( gioc->mRuolo == ATT )
        mAttList << gioc;

    return gioc;
}

QPlayer* QTeam::getGiocatore( int codice )
{
    for( int i=0; i<mPorList.size(); i++ )
    {
        if( mPorList[i]->mCodice == codice )
            return mPorList[i];
    }

    for( int i=0; i<mDifList.size(); i++ )
    {
        if( mDifList[i]->mCodice == codice )
            return mDifList[i];
    }

    for( int i=0; i<mCenList.size(); i++ )
    {
        if( mCenList[i]->mCodice == codice )
            return mCenList[i];
    }

    for( int i=0; i<mAttList.size(); i++ )
    {
        if( mAttList[i]->mCodice == codice )
            return mAttList[i];
    }

    return NULL;
}

bool QTeam::cancellaGiocatore( int codice )
{
    for( int i=0; i<mPorList.size(); i++ )
    {
        if( mPorList[i]->mCodice == codice )
        {
            delete mPorList[i];
            mPorList.removeAt(i);

            qDebug() << tr("Rimosso giocatore %1").arg(codice);
            return true;
        }
    }

    for( int i=0; i<mDifList.size(); i++ )
    {
        if( mDifList[i]->mCodice == codice )
        {
            delete mDifList[i];
            mDifList.removeAt(i);

            qDebug() << tr("Rimosso giocatore %1").arg(codice);
            return true;
        }
    }

    for( int i=0; i<mCenList.size(); i++ )
    {
        if( mCenList[i]->mCodice == codice )
        {
            delete mCenList[i];
            mCenList.removeAt(i);

            qDebug() << tr("Rimosso giocatore %1").arg(codice);
            return true;
        }
    }

    for( int i=0; i<mAttList.size(); i++ )
    {
        if( mAttList[i]->mCodice == codice )
        {
            delete mAttList[i];
            mAttList.removeAt(i);

            qDebug() << tr("Rimosso giocatore %1").arg(codice);
            return true;
        }
    }

    qDebug() << tr("Giocatore %1 non in lista").arg(codice);
    return false;
}

bool QTeam::cediGiocatore( int codice, bool svaluta )
{
    for( int i=0; i<mPorList.size(); i++ )
    {
        if( mPorList[i]->mCodice == codice )
        {
            int val = mPorList[i]->mValore;

            if( svaluta )
                val = (int)(((float)val)-((float)val*0.1f)+0.5f);

            mBudget += val;

            delete mPorList[i];
            mPorList.removeAt(i);

            return true;
        }
    }

    for( int i=0; i<mDifList.size(); i++ )
    {
        if( mDifList[i]->mCodice == codice )
        {
            int val = mDifList[i]->mValore;

            if( svaluta )
                val = (int)(((float)val)-((float)val*0.1f)+0.5f);

            mBudget += val;

            delete mDifList[i];
            mDifList.removeAt(i);

            return true;
        }
    }

    for( int i=0; i<mCenList.size(); i++ )
    {
        if( mCenList[i]->mCodice == codice )
        {
            int val = mCenList[i]->mValore;

            if( svaluta )
                val = (int)(((float)val)-((float)val*0.1f)+0.5f);

            mBudget += val;

            delete mCenList[i];
            mCenList.removeAt(i);

            return true;
        }
    }

    for( int i=0; i<mAttList.size(); i++ )
    {
        if( mAttList[i]->mCodice == codice )
        {
            int val = mAttList[i]->mValore;

            if( svaluta )
                val = (int)(((float)val)-((float)val*0.1f)+0.5f);

            mBudget += val;

            delete mAttList[i];
            mAttList.removeAt(i);

            return true;
        }
    }

    qDebug() << tr("Giocatore %1 non in lista").arg(codice);
    return false;
}

void QTeam::salvaSquadra(int giornata)
{
    QFile squadra;
    QString path = tr("%1/%2_G%3.txt").arg(SQ_PATH).arg(mPres).arg(giornata);
    squadra.setFileName( path );
    squadra.open( QIODevice::WriteOnly );

    QString line;
    line = tr("CREDITI,%1\r\n").arg(mBudget);
    squadra.write( line.toStdString().c_str() );

    line = tr("Ruolo,Codice,Nuovo,Valore,Giorn_Acq\r\n");
    squadra.write( line.toStdString().c_str() );

    for( int i=0; i<mPorList.size(); i++ )
    {
        line = tr("P,%1,%2,%3,%4,%5\r\n").arg(mPorList[i]->mCodice).arg(1).arg(mPorList[i]->mValore).arg(giornata).arg(mPorList[i]->mNome);
        squadra.write( line.toStdString().c_str() );
    }

    for( int i=0; i<mDifList.size(); i++ )
    {
        line = tr("D,%1,%2,%3,%4,%5\r\n").arg(mDifList[i]->mCodice).arg(1).arg(mDifList[i]->mValore).arg(giornata).arg(mDifList[i]->mNome);
        squadra.write( line.toStdString().c_str() );
    }

    for( int i=0; i<mCenList.size(); i++ )
    {
        line = tr("C,%1,%2,%3,%4,%5\r\n").arg(mCenList[i]->mCodice).arg(1).arg(mCenList[i]->mValore).arg(giornata).arg(mCenList[i]->mNome);
        squadra.write( line.toStdString().c_str() );
    }

    for( int i=0; i<mAttList.size(); i++ )
    {
        line = tr("A,%1,%2,%3,%4,%5\r\n").arg(mAttList[i]->mCodice).arg(1).arg(mAttList[i]->mValore).arg(giornata).arg(mAttList[i]->mNome);
        squadra.write( line.toStdString().c_str() );
    }
}

bool QTeam::caricaSquadra(int giornata)
{
    mPorList.clear();
    mDifList.clear();
    mCenList.clear();
    mAttList.clear();

    QFile squadra;
    QString path = tr("%1/%2_G%3.txt").arg(SQ_PATH).arg(mPres).arg(giornata);
    squadra.setFileName( path );
    if( !squadra.open( QIODevice::ReadOnly ) )
        return false;

    bool daSalvare = false;

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

        if( strings.size()==6 )
        {
            bool ok;
            int codice = strings[1].toInt(&ok);
            if(!ok)
                continue;
            bool nuovo = strings[2].toInt(&ok)==1;
            if(!ok)
                continue;
            int val = strings[3].toInt(&ok);
            if(!ok)
                continue;
            int giornAcq = strings[4].toInt(&ok);
            if(!ok)
                continue;

            QPlayer* gioc = aggiungiGioc( codice, nuovo, val, giornAcq );
            strings[5].remove( "\r" );
            strings[5].remove( "\n" );

            if( nuovo==0 && giornata==0 ) // Conversione di stagione!
            {
                gioc->mGiornAcq = 0;
                gioc->mNuovoAcq = 1;
                int codiceCorr = QPlayer::cercaCodiceCorretto( strings[5],giornata );
                gioc->mCodice = codiceCorr;
                daSalvare = true;
            }

            strings[5].remove( "\r" );
            strings[5].remove( "\n" );

            if( !gioc || strings[5].compare( gioc->mNome )!=0 )
            {
                qDebug() << tr("%3 - Il codice %1 non corrisponde al giocatore %2").arg(codice).arg(strings[5]).arg(mPres);

                int codiceCorr = QPlayer::cercaCodiceCorretto( strings[5],giornata );
                if( codiceCorr>0 )
                {
                    if(gioc)
                        cancellaGiocatore(codice);

                    aggiungiGioc( codiceCorr, nuovo, val, giornata );

                    daSalvare = true;
                    qDebug() << tr("%3 - Il codice corretto del giocatore %2 è %1").arg(codiceCorr).arg(strings[5]).arg(mPres);
                }
            }
        }
    }

    if(daSalvare)
    {
        salvaSquadra(giornata);
        qDebug() << tr("Salvate modifiche a squadra %1 per la giornata %2").arg(mPres).arg(giornata);
    }

    return true;
}

void QTeam::aggiornaTabella( QTeamWidget* widget )
{
    int totGioc = mPorList.size()+mDifList.size()+mCenList.size()+mAttList.size();

    int daComp = 25-totGioc;
    int maxSpesa = mBudget-daComp+1;

    int capitale = mBudget;

    if(daComp == 0)
        maxSpesa--;

    QTableWidget* tableWidget = widget->getTablePtr();

    tableWidget->setUpdatesEnabled(false);
    tableWidget->clearContents();

    int row = 0;
    foreach( const QPlayer* player, mPorList )
    {
        QTableWidgetItem* item0 = new QTableWidgetItem(tr("%1").arg(player->mCodice) );
        tableWidget->setItem( row, 0, item0 );

        QTableWidgetItem* item1 = new QTableWidgetItem(tr("%1").arg(player->mNome) );
        tableWidget->setItem( row, 1, item1 );

        QTableWidgetItem* item2 = new QTableWidgetItem(tr("%1").arg(player->mSquadra) );
        tableWidget->setItem( row, 2, item2 );

        QTableWidgetItem* item3 = new QTableWidgetItem(tr("%1").arg(player->mValore) );
        tableWidget->setItem( row, 3, item3 );

        capitale += player->mValore;

        if(player->mNuovoAcq)
        {
            QTableWidgetItem* item4 = new QTableWidgetItem(tr("%1").arg(0) );
            tableWidget->setItem( row, 4, item4 );
        }
        else
        {
            QTableWidgetItem* item4 = new QTableWidgetItem(tr("%1").arg(player->mValAcq13_14) );
            tableWidget->setItem( row, 4, item4 );
        }

        QTableWidgetItem* item5 = new QTableWidgetItem(tr("%1").arg(player->mValIniz14_15) );
        tableWidget->setItem( row, 5, item5 );

        QTableWidgetItem* item6 = new QTableWidgetItem(tr("%1").arg(player->mGiornAcq) );
        tableWidget->setItem( row, 6, item6 );

        QTableWidgetItem* item7 = new QTableWidgetItem(tr("%1").arg(player->mGiocate) );
        tableWidget->setItem( row, 7, item7 );

        QTableWidgetItem* item8 = new QTableWidgetItem(tr("%1").arg(player->mAmm) );
        tableWidget->setItem( row, 8, item8 );

        QTableWidgetItem* item9 = new QTableWidgetItem(tr("%1").arg(player->mEsp) );
        tableWidget->setItem( row, 9, item9 );

        QTableWidgetItem* item10 = new QTableWidgetItem(tr("%1").arg(player->mGolFatti) );
        tableWidget->setItem( row, 10, item10 );

        QTableWidgetItem* item11 = new QTableWidgetItem(tr("%1").arg(player->mGolSubiti) );
        tableWidget->setItem( row, 11, item11 );

        QTableWidgetItem* item12 = new QTableWidgetItem(tr("%1").arg(player->mAssTot) );
        tableWidget->setItem( row, 12, item12 );

        QTableWidgetItem* item13 = new QTableWidgetItem(tr("%1").arg(player->mRigPar) );
        tableWidget->setItem( row, 13, item13 );

        QTableWidgetItem* item14 = new QTableWidgetItem(tr("%1").arg(player->mRigSbag) );
        tableWidget->setItem( row, 14, item14 );

        QTableWidgetItem* item15 = new QTableWidgetItem(tr("%1").arg(player->mRigSegn) );
        tableWidget->setItem( row, 15, item15 );

        QTableWidgetItem* item16 = new QTableWidgetItem(tr("%1").arg(player->mAutogol) );
        tableWidget->setItem( row, 16, item16 );

        QTableWidgetItem* item17 = new QTableWidgetItem(tr("%1").arg(player->mGolWin) );
        tableWidget->setItem( row, 17, item17 );

        QTableWidgetItem* item18 = new QTableWidgetItem(tr("%1").arg(player->mMedia) );
        tableWidget->setItem( row, 18, item18 );

        QTableWidgetItem* item19 = new QTableWidgetItem(tr("%1").arg(player->mMediaFC) );
        tableWidget->setItem( row, 19, item19 );

        row++;
    }

    row = 3;
    foreach( const QPlayer* player, mDifList )
    {
        QTableWidgetItem* item0 = new QTableWidgetItem(tr("%1").arg(player->mCodice) );
        tableWidget->setItem( row, 0, item0 );

        QTableWidgetItem* item1 = new QTableWidgetItem(tr("%1").arg(player->mNome) );
        tableWidget->setItem( row, 1, item1 );

        QTableWidgetItem* item2 = new QTableWidgetItem(tr("%1").arg(player->mSquadra) );
        tableWidget->setItem( row, 2, item2 );

        QTableWidgetItem* item3 = new QTableWidgetItem(tr("%1").arg(player->mValore) );
        tableWidget->setItem( row, 3, item3 );

        capitale += player->mValore;

        if(player->mNuovoAcq)
        {
            QTableWidgetItem* item4 = new QTableWidgetItem(tr("%1").arg(0) );
            tableWidget->setItem( row, 4, item4 );
        }
        else
        {
            QTableWidgetItem* item4 = new QTableWidgetItem(tr("%1").arg(player->mValAcq13_14) );
            tableWidget->setItem( row, 4, item4 );
        }

        QTableWidgetItem* item5 = new QTableWidgetItem(tr("%1").arg(player->mValIniz14_15) );
        tableWidget->setItem( row, 5, item5 );

        QTableWidgetItem* item6 = new QTableWidgetItem(tr("%1").arg(player->mGiornAcq) );
        tableWidget->setItem( row, 6, item6 );

        QTableWidgetItem* item7 = new QTableWidgetItem(tr("%1").arg(player->mGiocate) );
        tableWidget->setItem( row, 7, item7 );

        QTableWidgetItem* item8 = new QTableWidgetItem(tr("%1").arg(player->mAmm) );
        tableWidget->setItem( row, 8, item8 );

        QTableWidgetItem* item9 = new QTableWidgetItem(tr("%1").arg(player->mEsp) );
        tableWidget->setItem( row, 9, item9 );

        QTableWidgetItem* item10 = new QTableWidgetItem(tr("%1").arg(player->mGolFatti) );
        tableWidget->setItem( row, 10, item10 );

        QTableWidgetItem* item11 = new QTableWidgetItem(tr("%1").arg(player->mGolSubiti) );
        tableWidget->setItem( row, 11, item11 );

        QTableWidgetItem* item12 = new QTableWidgetItem(tr("%1").arg(player->mAssTot) );
        tableWidget->setItem( row, 12, item12 );

        QTableWidgetItem* item13 = new QTableWidgetItem(tr("%1").arg(player->mRigPar) );
        tableWidget->setItem( row, 13, item13 );

        QTableWidgetItem* item14 = new QTableWidgetItem(tr("%1").arg(player->mRigSbag) );
        tableWidget->setItem( row, 14, item14 );

        QTableWidgetItem* item15 = new QTableWidgetItem(tr("%1").arg(player->mRigSegn) );
        tableWidget->setItem( row, 15, item15 );

        QTableWidgetItem* item16 = new QTableWidgetItem(tr("%1").arg(player->mAutogol) );
        tableWidget->setItem( row, 16, item16 );

        QTableWidgetItem* item17 = new QTableWidgetItem(tr("%1").arg(player->mGolWin) );
        tableWidget->setItem( row, 17, item17 );

        QTableWidgetItem* item18 = new QTableWidgetItem(tr("%1").arg(player->mMedia) );
        tableWidget->setItem( row, 18, item18 );

        QTableWidgetItem* item19 = new QTableWidgetItem(tr("%1").arg(player->mMediaFC) );
        tableWidget->setItem( row, 19, item19 );

        row++;
    }

    row = 11;
    foreach( const QPlayer* player, mCenList )
    {
        QTableWidgetItem* item0 = new QTableWidgetItem(tr("%1").arg(player->mCodice) );
        tableWidget->setItem( row, 0, item0 );

        QTableWidgetItem* item1 = new QTableWidgetItem(tr("%1").arg(player->mNome) );
        tableWidget->setItem( row, 1, item1 );

        QTableWidgetItem* item2 = new QTableWidgetItem(tr("%1").arg(player->mSquadra) );
        tableWidget->setItem( row, 2, item2 );

        QTableWidgetItem* item3 = new QTableWidgetItem(tr("%1").arg(player->mValore) );
        tableWidget->setItem( row, 3, item3 );

        capitale += player->mValore;

        if(player->mNuovoAcq)
        {
            QTableWidgetItem* item4 = new QTableWidgetItem(tr("%1").arg(0) );
            tableWidget->setItem( row, 4, item4 );
        }
        else
        {
            QTableWidgetItem* item4 = new QTableWidgetItem(tr("%1").arg(player->mValAcq13_14) );
            tableWidget->setItem( row, 4, item4 );
        }

        QTableWidgetItem* item5 = new QTableWidgetItem(tr("%1").arg(player->mValIniz14_15) );
        tableWidget->setItem( row, 5, item5 );

        QTableWidgetItem* item6 = new QTableWidgetItem(tr("%1").arg(player->mGiornAcq) );
        tableWidget->setItem( row, 6, item6 );

        QTableWidgetItem* item7 = new QTableWidgetItem(tr("%1").arg(player->mGiocate) );
        tableWidget->setItem( row, 7, item7 );

        QTableWidgetItem* item8 = new QTableWidgetItem(tr("%1").arg(player->mAmm) );
        tableWidget->setItem( row, 8, item8 );

        QTableWidgetItem* item9 = new QTableWidgetItem(tr("%1").arg(player->mEsp) );
        tableWidget->setItem( row, 9, item9 );

        QTableWidgetItem* item10 = new QTableWidgetItem(tr("%1").arg(player->mGolFatti) );
        tableWidget->setItem( row, 10, item10 );

        QTableWidgetItem* item11 = new QTableWidgetItem(tr("%1").arg(player->mGolSubiti) );
        tableWidget->setItem( row, 11, item11 );

        QTableWidgetItem* item12 = new QTableWidgetItem(tr("%1").arg(player->mAssTot) );
        tableWidget->setItem( row, 12, item12 );

        QTableWidgetItem* item13 = new QTableWidgetItem(tr("%1").arg(player->mRigPar) );
        tableWidget->setItem( row, 13, item13 );

        QTableWidgetItem* item14 = new QTableWidgetItem(tr("%1").arg(player->mRigSbag) );
        tableWidget->setItem( row, 14, item14 );

        QTableWidgetItem* item15 = new QTableWidgetItem(tr("%1").arg(player->mRigSegn) );
        tableWidget->setItem( row, 15, item15 );

        QTableWidgetItem* item16 = new QTableWidgetItem(tr("%1").arg(player->mAutogol) );
        tableWidget->setItem( row, 16, item16 );

        QTableWidgetItem* item17 = new QTableWidgetItem(tr("%1").arg(player->mGolWin) );
        tableWidget->setItem( row, 17, item17 );

        QTableWidgetItem* item18 = new QTableWidgetItem(tr("%1").arg(player->mMedia) );
        tableWidget->setItem( row, 18, item18 );

        QTableWidgetItem* item19 = new QTableWidgetItem(tr("%1").arg(player->mMediaFC) );
        tableWidget->setItem( row, 19, item19 );

        row++;
    }

    row = 19;
    foreach( const QPlayer* player, mAttList )
    {
        QTableWidgetItem* item0 = new QTableWidgetItem(tr("%1").arg(player->mCodice) );
        tableWidget->setItem( row, 0, item0 );

        QTableWidgetItem* item1 = new QTableWidgetItem(tr("%1").arg(player->mNome) );
        tableWidget->setItem( row, 1, item1 );

        QTableWidgetItem* item2 = new QTableWidgetItem(tr("%1").arg(player->mSquadra) );
        tableWidget->setItem( row, 2, item2 );

        QTableWidgetItem* item3 = new QTableWidgetItem(tr("%1").arg(player->mValore) );
        tableWidget->setItem( row, 3, item3 );

        capitale += player->mValore;

        if(player->mNuovoAcq)
        {
            QTableWidgetItem* item4 = new QTableWidgetItem(tr("%1").arg(0) );
            tableWidget->setItem( row, 4, item4 );
        }
        else
        {
            QTableWidgetItem* item4 = new QTableWidgetItem(tr("%1").arg(player->mValAcq13_14) );
            tableWidget->setItem( row, 4, item4 );
        }

        QTableWidgetItem* item5 = new QTableWidgetItem(tr("%1").arg(player->mValIniz14_15) );
        tableWidget->setItem( row, 5, item5 );

        QTableWidgetItem* item6 = new QTableWidgetItem(tr("%1").arg(player->mGiornAcq) );
        tableWidget->setItem( row, 6, item6 );

        QTableWidgetItem* item7 = new QTableWidgetItem(tr("%1").arg(player->mGiocate) );
        tableWidget->setItem( row, 7, item7 );

        QTableWidgetItem* item8 = new QTableWidgetItem(tr("%1").arg(player->mAmm) );
        tableWidget->setItem( row, 8, item8 );

        QTableWidgetItem* item9 = new QTableWidgetItem(tr("%1").arg(player->mEsp) );
        tableWidget->setItem( row, 9, item9 );

        QTableWidgetItem* item10 = new QTableWidgetItem(tr("%1").arg(player->mGolFatti) );
        tableWidget->setItem( row, 10, item10 );

        QTableWidgetItem* item11 = new QTableWidgetItem(tr("%1").arg(player->mGolSubiti) );
        tableWidget->setItem( row, 11, item11 );

        QTableWidgetItem* item12 = new QTableWidgetItem(tr("%1").arg(player->mAssTot) );
        tableWidget->setItem( row, 12, item12 );

        QTableWidgetItem* item13 = new QTableWidgetItem(tr("%1").arg(player->mRigPar) );
        tableWidget->setItem( row, 13, item13 );

        QTableWidgetItem* item14 = new QTableWidgetItem(tr("%1").arg(player->mRigSbag) );
        tableWidget->setItem( row, 14, item14 );

        QTableWidgetItem* item15 = new QTableWidgetItem(tr("%1").arg(player->mRigSegn) );
        tableWidget->setItem( row, 15, item15 );

        QTableWidgetItem* item16 = new QTableWidgetItem(tr("%1").arg(player->mAutogol) );
        tableWidget->setItem( row, 16, item16 );

        QTableWidgetItem* item17 = new QTableWidgetItem(tr("%1").arg(player->mGolWin) );
        tableWidget->setItem( row, 17, item17 );

        QTableWidgetItem* item18 = new QTableWidgetItem(tr("%1").arg(player->mMedia) );
        tableWidget->setItem( row, 18, item18 );

        QTableWidgetItem* item19 = new QTableWidgetItem(tr("%1").arg(player->mMediaFC) );
        tableWidget->setItem( row, 19, item19 );

        row++;
    }

    widget->setInfo( mBudget, capitale, daComp, maxSpesa );
    tableWidget->setUpdatesEnabled(true);
}



