#pragma once
#include "Include.h"




class Food : public Inert
{
private:

	bool	 big,isEat;

	//Puntantori a entity
	Pacman* pacman;
	Blinky* blinky;
	Pinky* pinky;
	Inky* inky;
	Clyde* clyde;

	QPixmap texture_food[2][2];
	QPixmap animation_food[2][2];

public:
	Food(QPoint position, bool isBig);

	virtual void animate() ;
	virtual void advance() ;

	void setFoodIsEat(bool _isEat) { isEat = _isEat; }
	bool getFoodIsEat() {return isEat; }
	bool getBig() { return big; }
};