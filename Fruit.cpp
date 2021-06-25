#include "Include.h"
Fruit::Fruit(QPoint position) :Inert()
{
	QFont myFont1("Arial", 7, QFont::Bold);

	fruit_eat = false;
	fruit_eat_time = 0;
	fruit_score = new QGraphicsTextItem();

	//Inserimento 100 punti quando mangio frutta
	fruit_score = Game::instance()->getScene()->addText(QString("100"), myFont1);
	fruit_score->setPos(102, 150);
	fruit_score->setDefaultTextColor(QColor(255, 255, 255));
	fruit_score->setZValue(1);
	fruit_score->setOpacity(0);


	// textures
	texture_fruit = Sprites::instance()->get("Cherry");
	texture_fruit.setMask(texture_fruit.createMaskFromColor(QColor(Qt::black)));

	setPixmap(texture_fruit);
	setPos(position - QPoint(pixmap().width(), pixmap().height()));
	setScale(0.35);
	
	setZValue(0);

	

}
void Fruit::advance()
{
	
	pacman = Game::instance()->getPacman();
	if (Game::instance()->getSecond() >= 10 && Game::instance()->getSecond() <= 15 && !Game::instance()->getFruit())
	{
		setZValue(1);
		if (pacman->getDirection() == RIGHT)
		{
			if (pacman->x() + 2 == x() && pacman->y() - 1 == y())
			{
				if (fruit_eat == false)
				{
					setOpacity(0);
					Sounds::instance()->play("eatfruit");
					fruit_eat = true;
					fruit_eat_time = Game::instance()->getSecond();
					fruit_score->setOpacity(1);
				}
			}
		}
		if (pacman->getDirection() == LEFT)
		{
			if (pacman->x() - 14 ==x() && pacman->y() - 1 == y() && !fruit_eat)
			{
				if (fruit_eat == false)
				{
					setOpacity(0);
					Sounds::instance()->play("eatfruit");
					fruit_eat = true;
					fruit_eat_time = Game::instance()->getSecond();
					fruit_score->setOpacity(1);
				}
			}
		}
	}
	//Faccio sparire frutto dopo tot secondi
	if (Game::instance()->getSecond() > 15)
		setZValue(0);

	//Per scritta 100 frutto
	if (fruit_eat == false && Game::instance()->getSecond() > fruit_eat_time+1)
		fruit_score->setOpacity(0);

}