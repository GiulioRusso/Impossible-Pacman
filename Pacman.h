#pragma once
#include <iostream>
#include "Include.h"

class Pacman : public Entity
{
private:

	QPixmap  texture_walking[4][4];			 //Animazione di pacman
	QPixmap  texture_die[13];	             //Animazione della morte di pacman

	bool death = false;
	
public:
	Pacman(QPoint position);

	virtual void animate() ;
	virtual void advance() ;

	virtual void die();

	//Permette di capire se la direzione in cui sto andando e' possibile
	bool canGoRight();
	bool canGoDown();
	bool canGoUp();
	bool canGoLeft();

	bool isDeath()			 { return death; }					//e' morto?
	bool getIsDying()		 { return isDying; }				//Sta morendo?
	int getCurrentNode()     { return current_node; }			//Nodo in cui mi trovo
	int getDestNode()		 { return dest_node; }				//Nodo in cui mi sto dirigendo
	bool is_Moving()		 { return isMoving; }				//Mi sto muovendo?

	
};