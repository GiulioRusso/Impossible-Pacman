#pragma once
#include <cmath>
#include "Include.h"
#include "Pacman.h"
#include <QDebug>

class Ghosts : public Entity
{
protected:

	Direction	 gdir;												//Direzione del fantasma
	State		 state;												//Stato di gioco
	bool		 gcan_right, gcan_left, gcan_up, gcan_down;			//Direzione possibile?
	int			 gcurrent_node, gdest_node;							//Nodo corrente e do ve sto andando

	//variabili per la distanza
	int			 gtarget_x, gtarget_y;								//Punto a cui punta il fantasma
	int			 gcx, gcy;											//ghost coordinate x, y
	float		 gdright, gdleft, gdup, gddown, gdmin; 				//ghost distance dir
	int			 gtmp_dir; 											//variabile direzione temporanea	

	//Flag stati fantasmi
	bool ghost_eat;													//Eaten
	bool vulnerable;												//Vulnerable

	Pacman *pacman;													//Puntatore a pacman
	QGraphicsTextItem* ghost_score;									//Punteggio che  appare quando il fantasma e' stato mangiato
	bool fantasma_mangiato;											//Sono stato mangiato?

	//Animation Counter
	int vulnerable_counter;											//contatore stato Vulnerable

	//Texture state;
	QPixmap  texture_vulnerable[2][2];
	QPixmap  texture_finish_vulnerable[8][2];
	QPixmap  texture_die[4];

	//Matrice contenente tutti i possibili cammini minimi per ogni  coppia di nodi
	std::list<int> cammini[76][76];
	
	//ai
	bool ai;

	//lista percorso corrente
	std::list<int> percorsoAi;

public:
	
	Ghosts();

	virtual void animate() ;
	virtual void advance() ;

	//Get e set stato Vulnerable
	virtual void  setVulnerable(bool _vulnerable)				{ vulnerable = _vulnerable; }	
	virtual bool  getVulnerable()								{ return vulnerable; }

	//Get e set Stato Eaten
	virtual void  setGhostEat(bool _ghost_eat)					{ ghost_eat = _ghost_eat; }
	virtual bool  getGhostEat()									{ return ghost_eat; }

	//Get e Set dello Stato
	virtual void  setState(State _state)	   					{ state = _state; }
	virtual State getState()									{ return state; }
	
	//ottengo coordinate del nodo a cui puntq
	virtual int   getTargetx()									{ return gtarget_x; }
	virtual int   getTargety()									{ return gtarget_y; }
	
	//Imposta contatore dell' animazione dello stato vulnerable 
	virtual void setVulnerableCounter(int _vulnerable_counter)	{ vulnerable_counter = _vulnerable_counter; }
	
	virtual void die();

	//Imposta il gioco in modalita impossibile
	virtual void setImpossible(bool _impossible)				 { impossible = _impossible; }

	//Ci dice se il fantasma è stato mangiato o meno
	virtual bool getFantasmaMangiato()							 { return fantasma_mangiato; }
	virtual void setFantasmaMangiato(bool _fantasma_Mangiato)    {  fantasma_mangiato= _fantasma_Mangiato; }

	//Funzione che calcola il cammino minimo tra due nodi
	virtual std::list<int> cammino(int x, int y);
	virtual void setAi(bool _ai)								 { ai = _ai; }												//Set Stato in Ai
	virtual std::list<int> getCamminoAi(int x, int y)			 { return cammini[x][y]; }         
	
	//Ritorna nodo in cui mi dirigo
	virtual int getDestNode() { return gdest_node; }

	//Ritorna i nodi del cammino (usata per disegnare il percorso)
	std::list<int> getCammino() { return percorsoAi; }
};