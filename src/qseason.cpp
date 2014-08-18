#include "include\qseason.h"

QSeason::QSeason(QObject *parent) :
    QObject(parent)
{
}

bool QSeason::init2014_2015( int giornata )
{
    mGiornata = giornata;

    QTeam* team0 = new QTeam();
    QTeam* team1 = new QTeam();
    QTeam* team2 = new QTeam();
    QTeam* team3 = new QTeam();
    QTeam* team4 = new QTeam();
    QTeam* team5 = new QTeam();
    QTeam* team6 = new QTeam();
    QTeam* team7 = new QTeam();

    team0->setParams( "Andrea", "Andrea", 250 );
    team1->setParams( "Angelo", "Angelo", 250 );
    team2->setParams( "Daniel", "Daniel", 250 );
    team3->setParams( "Diego", "Diego", 250 );
    team4->setParams( "Fabio", "Fabio", 250 );
    team5->setParams( "Gabriele", "Gabriele", 250 );
    team6->setParams( "Stefano", "Stefano", 250 );
    team7->setParams( "F.C. Myzhar", "Walter", 250 );

    if( !team0->caricaSquadra( mGiornata ) )
        return false;
    if( !team1->caricaSquadra( mGiornata ) )
        return false;
    if( !team2->caricaSquadra( mGiornata ) )
        return false;
    if( !team3->caricaSquadra( mGiornata ) )
        return false;
    if( !team4->caricaSquadra( mGiornata ) )
        return false;
    if( !team5->caricaSquadra( mGiornata ) )
        return false;
    if( !team6->caricaSquadra( mGiornata ) )
        return false;
    if( !team7->caricaSquadra( mGiornata ) )
        return false;

    addTeam( team0 );
    addTeam( team1 );
    addTeam( team2 );
    addTeam( team3 );
    addTeam( team4 );
    addTeam( team5 );
    addTeam( team6 );
    addTeam( team7 );

    return true;
}
