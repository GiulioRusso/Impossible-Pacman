#include "Include.h"
Food::Food(QPoint position, bool isBig) : Inert()
{
	//Verifica se e' una PowerBall o meno
	big = isBig;
	isEat = false;

	//Texture
	texture_food[0][0] = Sprites::instance()->get("food");
	texture_food[1][0] = Sprites::instance()->get("big-food");

	animation_food[0][0] = Sprites::instance()->get("nofood");
	animation_food[1][0] = Sprites::instance()->get("big-food");

	//Ai
	texture_food[0][1] = Sprites::instance()->get("foodAi");
	texture_food[1][1] = Sprites::instance()->get("big-foodAi");
	
	animation_food[0][1] = Sprites::instance()->get("nofood");
	animation_food[1][1] = Sprites::instance()->get("big-foodAi");
	//Set texture and correct y-coordinate w.r.t. texture height
	setPixmap(texture_food[big][0]);
	setPos(position - QPoint(pixmap().width(), pixmap().height()));


	//Set a high z-value so it is displayed on top of other objects
	setZValue(1);
}
void Food::animate()
{
	Inert::animate();

	if (big && Sounds::instance()->isFinished("beginning") && !impossible)
		setPixmap(animation_food[(animationCounterInert / 8) % 2][0]);

	else if (big && Sounds::instance()->isFinished("beginning") && impossible)
		setPixmap(animation_food[(animationCounterInert / 8) % 2][1]);
	
	if (impossible && !big)
		setPixmap(texture_food[big][1]);
	if (impossible && big)
		setPixmap(texture_food[1][1]);
	
	else if(!impossible && !big)
		setPixmap(texture_food[big][0]);
	
	if (isEat)
		setOpacity(0);
		
}
void Food::advance()
{
	pacman = Game::instance()->getPacman();
	blinky = Game::instance()->getBlinky();
	clyde = Game::instance()->getClyde();
	pinky = Game::instance()->getPinky();
	inky = Game::instance()->getInky();

	static int countFood = 0;

	if (((x() == pacman->x() +6 && y() == pacman->y() +6) || (x() == pacman->x() + 3 && y() == pacman->y() + 3))&& !isEat)
	{		
		isEat = true;
		countFood++;

		//Nel caso mangio una PowerBall
		if (big)
		{
			Sounds::instance()->play("hunt");
			//Aziono vulnerabilita e setto il contatore dell'animazione a 0
			if (blinky->getState() != EATEN)
			{
				blinky->setVulnerable(true);
				blinky->setState(VULNERABLE);
				blinky->setVulnerableCounter(0);
			}
			if (inky->getState() != EATEN)
			{
				inky->setVulnerable(true);
				inky->setState(VULNERABLE);
				inky->setVulnerableCounter(0);
			}
			if (pinky->getState() != EATEN)
			{
				pinky->setVulnerable(true);
				pinky->setState(VULNERABLE);
				pinky->setVulnerableCounter(0);
			}
			if (clyde->getState() != EATEN)
			{
				clyde->setVulnerable(true);
				clyde->setState(VULNERABLE);
				clyde->setVulnerableCounter(0);
			}
		}
		
		//Imposto musica di quando mangio il cibo
		else if (countFood %2  ==0) //%2 perchè alterna i due chomp
			Sounds::instance()->play("eat1");
		else
			Sounds::instance()->play("eat2");
	}
	
}