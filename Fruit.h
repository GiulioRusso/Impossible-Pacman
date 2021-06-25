#pragma once
#include "Include.h"
class Fruit : public Inert
{
private:

	// textures
	QPixmap texture_fruit;

	//se il cibo è stato mangiato
	bool fruit_eat;

	//tempo in cui è stato mangiato
	int fruit_eat_time;

	Pacman* pacman;

	//per lo score (100)
	QGraphicsTextItem* fruit_score;

public:

	Fruit(QPoint position);

	virtual void animate() {};
	virtual void advance();

	bool get_Fruit_iseat() { return fruit_eat; }
	void set_Fruit_iseat(bool _fruit_eat) { fruit_eat = _fruit_eat; }
	void setOp() { fruit_score->setOpacity(0); }

};