//This File contain Blinky's code. one of the four ghosts of the game.
#include "Include.h"
Blinky::Blinky(QPoint position) : Ghosts()
{
	vulnerable =false;
	ghost_eat = false;
	isMoving = true;
	state = SCATTER;
	impossible = false;
	ai = false;

	gdir = LEFT;
	gcurrent_node = 24;
	gdest_node = 23;

	gcan_right = false;
	gcan_left = true;
	gcan_up = false;
	gcan_down = false;

	//texture ghost Blinky [texture][direction]
	texture_Blinky[0][1][0] = Sprites::instance()->get("red-ghost-dx-0");
	texture_Blinky[0][2][0] = Sprites::instance()->get("red-ghost-sx-0");
	texture_Blinky[0][3][0] = Sprites::instance()->get("red-ghost-up-0");
	texture_Blinky[0][4][0] = Sprites::instance()->get("red-ghost-down-0");

	texture_Blinky[1][1][0] = Sprites::instance()->get("red-ghost-dx-1");
	texture_Blinky[1][2][0] = Sprites::instance()->get("red-ghost-sx-1");
	texture_Blinky[1][3][0] = Sprites::instance()->get("red-ghost-up-1");
	texture_Blinky[1][4][0] = Sprites::instance()->get("red-ghost-down-1");

	//Mod Impossible
	texture_Blinky[0][1][1] = Sprites::instance()->get("blinkydx-0");
	texture_Blinky[0][2][1] = Sprites::instance()->get("blinkysx-0");
	texture_Blinky[0][3][1] = Sprites::instance()->get("blinkyup-0");
	texture_Blinky[0][4][1] = Sprites::instance()->get("blinkydown-0");

	texture_Blinky[1][1][1] = Sprites::instance()->get("blinkydx-1");
	texture_Blinky[1][2][1] = Sprites::instance()->get("blinkysx-1");
	texture_Blinky[1][3][1] = Sprites::instance()->get("blinkyup-1");
	texture_Blinky[1][4][1] = Sprites::instance()->get("blinkydown-1");

	//setMask
	for (int i = 0; i < 2; i++)
		for (int j = 1; j < 5; j++)
		{
			texture_Blinky[i][j][0].setMask(texture_Blinky[i][j][0].createMaskFromColor(QColor(0, 0, 0)));
			texture_Blinky[i][j][1].setMask(texture_Blinky[i][j][1].createMaskFromColor(QColor(0, 0, 0)));

		}
	setPixmap(texture_Blinky[0][3][0]);

	//VulnerableMask
	for (int i = 0; i < 8; i++)
	{
		texture_finish_vulnerable[i][0].setMask(texture_finish_vulnerable[i][0].createMaskFromColor(QColor(0, 0, 0)));
		texture_finish_vulnerable[i][0].setMask(texture_finish_vulnerable[i][0].createMaskFromColor(QColor(0, 0, 0)));
	}

	texture_vulnerable[0][1].setMask(texture_vulnerable[0][1].createMaskFromColor(QColor(0,0,0)));
	texture_vulnerable[1][1].setMask(texture_vulnerable[1][1].createMaskFromColor(QColor(0, 0, 0)));
	texture_vulnerable[0][0].setMask(texture_vulnerable[0][0].createMaskFromColor(QColor(0, 0, 0)));
	texture_vulnerable[1][0].setMask(texture_vulnerable[1][0].createMaskFromColor(QColor(0, 0, 0)));


	setPos(position - QPoint(pixmap().width(), pixmap().height()));
	setZValue(3);
}
void Blinky::animate()
{
	Ghosts::animate();

	if (!impossible)
	{
		if (gdir == RIGHT && !vulnerable && !ghost_eat)
		{
			setPixmap(texture_Blinky[0][1][0]);
			setPixmap(texture_Blinky[(animation_counter++ / 15) % 2][1][0]);
		}
		else if (gdir == LEFT && !vulnerable && !ghost_eat)
		{
			setPixmap(texture_Blinky[0][2][0]);
			setPixmap(texture_Blinky[(animation_counter++ / 15) % 2][2][0]);
		}
		else if (gdir == UP && !vulnerable && !ghost_eat)
		{
			setPixmap(texture_Blinky[0][3][0]);
			setPixmap(texture_Blinky[(animation_counter++ / 15) % 2][3][0]);
		}
		else if (gdir == DOWN && !vulnerable && !ghost_eat)
		{
			setPixmap(texture_Blinky[0][4][0]);
			setPixmap(texture_Blinky[(animation_counter++ / 15) % 2][4][0]);
		}
	}
	if (impossible)
	{
		if (gdir == RIGHT && !vulnerable && !ghost_eat)
		{
			setPixmap(texture_Blinky[0][1][1]);
			setPixmap(texture_Blinky[(animation_counter++ / 15) % 2][1][1]);
		}
		else if (gdir == LEFT && !vulnerable && !ghost_eat)
		{
			setPixmap(texture_Blinky[0][2][1]);
			setPixmap(texture_Blinky[(animation_counter++ / 15) % 2][2][1]);
		}
		else if (gdir == UP && !vulnerable && !ghost_eat)
		{
			setPixmap(texture_Blinky[0][3][1]);
			setPixmap(texture_Blinky[(animation_counter++ / 15) % 2][3][1]);
		}
		else if (gdir == DOWN && !vulnerable && !ghost_eat)
		{
			setPixmap(texture_Blinky[0][4][1]);
			setPixmap(texture_Blinky[(animation_counter++ / 15) % 2][4][1]);
		}
	}
}
void Blinky::advance()
{
	Ghosts::advance();

	if (ai == true && !vulnerable && !ghost_eat )
	{
		state = AI;
	}


	
	
	if (Sounds::instance()->isFinished("beginning"))
	{
		isMoving = true;
		moving_speed = 1.05;
	}
	if (pacman->getIsDying())
		moving_speed = 0;

	//scatter
	if (state == SCATTER)
	{
		gtarget_x = 205;
		gtarget_y = 0;

		vicini[24][3] = -1;
	}

	//chase
	if (state == CHASE)
	{
		QPoint tmp = pacman->getPos();
		gtarget_x =tmp.x();
		gtarget_y = tmp.y();

		vicini[24][3] = -1;
	}



}
