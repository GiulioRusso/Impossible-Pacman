#pragma once

#include "Include.h"
#include "Utils.h"

class Entity : public Object
{

protected:

	//Nodi della mappa e vicini di ogni nodo
	Point		 nodo[76];
	int			 vicini[76][4];
	//Riferimenti ai nodi
	int			 current_node, dest_node;
	
	//Direzione
	Direction	 dir, last_dir, next_dir;

	//Bool di controllo sulle direzioni
	bool		 can_right, can_left, can_up, can_down;
	
	//Velocita oggetto
	float		 moving_speed;
	
	//Flag
	bool		 isMoving,isDead,isDying;
	
	//Animation Counter
	int			 animation_counter,death_counter;
	
	//Gioco Impossibile
	bool		 impossible;

	//Matrice delle Distanze tra tutti i nodi
	int			 distanza[76][76];


public:
	
	Entity() ;

	virtual void advance() ;//vuoto
	virtual void animate() ;

	//Modifica la direzione 
	virtual void setDirection(Direction _dir)	 { dir = _dir; }
	virtual Direction getDirection()			 { return dir; }

	virtual void setMoving(bool _isMoving)		 { isMoving = _isMoving; }

	//Ritorna la posizione corrente
	virtual QPoint getPos()						 { return QPoint(x(), y()); }
	
	virtual void die();//vuoto

	//Funzione che calcola tutte le distanze tra ogni coppia di nodi
	virtual void calcoloDistMin();
	
	//Verifica se due nodi sono vicini
	virtual bool is_near(int i, int j);

	//Ritorna il vicino 
	virtual int have_Near(int i, int scorr);
	
	//Get delle coordinate del nodo
	virtual int getNodoX(int a) { return nodo[a].x; }
	virtual int getNodoY(int a) { return nodo[a].y; }
};
