#include "Include.h"
Clyde::Clyde(QPoint position) :Ghosts()
{
	vulnerable = false;
	ghost_eat = false;
	impossible = false;
	state = SCATTER;
	ai = false;

	clast_node = 70;
	arrivatoC = true;
	cgoout = false;
	gdir = DOWN;
	gcurrent_node = 30;

	gcan_right = false;
	gcan_left = false;
	gcan_up = false;
	gcan_down = true;

	//texture ghost Clyde [texture][direction]
	texture_Clyde[0][1][0] = Sprites::instance()->get("orange-ghost-dx-0");
	texture_Clyde[0][2][0] = Sprites::instance()->get("orange-ghost-sx-0");
	texture_Clyde[0][3][0] = Sprites::instance()->get("orange-ghost-up-0");
	texture_Clyde[0][4][0] = Sprites::instance()->get("orange-ghost-down-0");

	texture_Clyde[1][1][0] = Sprites::instance()->get("orange-ghost-dx-1");
	texture_Clyde[1][2][0] = Sprites::instance()->get("orange-ghost-sx-1");
	texture_Clyde[1][3][0] = Sprites::instance()->get("orange-ghost-up-1");
	texture_Clyde[1][4][0] = Sprites::instance()->get("orange-ghost-down-1");

	//Mod Impossible
	texture_Clyde[0][1][1] = Sprites::instance()->get("clydedx-0");
	texture_Clyde[0][2][1] = Sprites::instance()->get("clydesx-0");
	texture_Clyde[0][3][1] = Sprites::instance()->get("clydeup-0");
	texture_Clyde[0][4][1] = Sprites::instance()->get("clydedown-0");

	texture_Clyde[1][1][1] = Sprites::instance()->get("clydedx-1");
	texture_Clyde[1][2][1] = Sprites::instance()->get("clydesx-1");
	texture_Clyde[1][3][1] = Sprites::instance()->get("clydeup-1");
	texture_Clyde[1][4][1] = Sprites::instance()->get("clydedown-1");


	setPixmap(texture_Clyde[0][3][0]);
	setPos(position - QPoint(pixmap().width(), pixmap().height()));

	for (int i = 0; i < 2; i++)
		for (int j = 1; j < 5; j++)
		{
			texture_Clyde[i][j][0].setMask(texture_Clyde[i][j][0].createMaskFromColor(QColor(0, 0, 0)));
			texture_Clyde[i][j][1].setMask(texture_Clyde[i][j][1].createMaskFromColor(QColor(0, 0, 0)));

		}
	//VulnerableMask
	for (int i = 0; i < 8; i++)
	{
		texture_finish_vulnerable[i][0].setMask(texture_finish_vulnerable[i][0].createMaskFromColor(QColor(0, 0, 0)));
		texture_finish_vulnerable[i][0].setMask(texture_finish_vulnerable[i][0].createMaskFromColor(QColor(0, 0, 0)));
	}

	texture_vulnerable[0][1].setMask(texture_vulnerable[0][1].createMaskFromColor(QColor(0, 0, 0)));
	texture_vulnerable[1][1].setMask(texture_vulnerable[1][1].createMaskFromColor(QColor(0, 0, 0)));
	texture_vulnerable[0][0].setMask(texture_vulnerable[0][0].createMaskFromColor(QColor(0, 0, 0)));
	texture_vulnerable[1][0].setMask(texture_vulnerable[1][0].createMaskFromColor(QColor(0, 0, 0)));


	setZValue(3);
}
void Clyde::advance()
{
	
	Ghosts::advance();

	if (Game::instance()->getSecond() < 7 && Sounds::instance()->isFinished("beginning"))
	{
		isMoving = true;
		moving_speed = 0.3;
		cgoout = false;
	}
	if (Game::instance()->getSecond() >= 7)
	{
		moving_speed = 1.05;
		isMoving = true;
		cgoout = true;
	}
	if (pacman->getIsDying())
		moving_speed = 0;

	//uscita forzata
	if (gcurrent_node == 70 && state != AI)
	{
		if (!cgoout)
		{
			gdir = DOWN;
			gcan_down = true;
			gcan_left = false;
			gcan_up = false;
			gcan_right = false;
			clast_node = 70;
		}
		else if (cgoout)
		{
			gdir = UP;
			gcan_down = false;
			gcan_left = false;
			gcan_up = true;
			gcan_right = false;
		}
	}
	if (gcurrent_node == 30 && state != AI)
	{
		if (!cgoout)
		{
			if (clast_node == 70)
			{
				gdir = DOWN;
				gcan_down = true;
				gcan_left = false;
				gcan_up = false;
				gcan_right = false;
			}
			if (clast_node == 71)
			{
				gdir = UP;
				gcan_down = false;
				gcan_left = false;
				gcan_up = true;
				gcan_right = false;
			}
		}
		else if (cgoout)
		{
			gdir = UP;
			gcan_down = false;
			gcan_left = false;
			gcan_up = true;
			gcan_right = false;
	
		}
	}
	if (gcurrent_node == 71 && state != AI)
	{
		gdir = UP;
		gcan_down = false;
		gcan_left = false;
		gcan_up = true;
		gcan_right = false;
		clast_node = 71;
	}

	//scatter
	if (state == SCATTER)
	{
		gtarget_x = 0;
		gtarget_y = 276;

		vicini[24][3] = -1;
	}

	//chase
	if (state == CHASE)
	{
		int dist;
		dist = sqrt(pow(pacman->x() - x(), 2) + pow(pacman->y() - y(), 2));
		if (dist >= 50)
		{
			gtarget_x = 0;
			gtarget_y = 276;
		}
			
		else if (dist < 50)
		{
			QPoint tmp = pacman->getPos();
			gtarget_x = tmp.x();
			gtarget_y = tmp.y();
		}
		
	}
	
	//Ai
	if (ai == true && !vulnerable && !ghost_eat)
	{
		state =AI;
		
	}

	
	
}
void Clyde::animate()
{
	Ghosts::animate();

	if (!impossible)
	{
		if (gdir == RIGHT && !vulnerable && !ghost_eat)
		{
			setPixmap(texture_Clyde[0][1][0]);
			setPixmap(texture_Clyde[(animation_counter++ / 15) % 2][1][0]);
		}
		else if (gdir == LEFT && !vulnerable && !ghost_eat)
		{
			setPixmap(texture_Clyde[0][2][0]);
			setPixmap(texture_Clyde[(animation_counter++ / 15) % 2][2][0]);
		}
		else if (gdir == UP && !vulnerable && !ghost_eat)
		{
			setPixmap(texture_Clyde[0][3][0]);
			setPixmap(texture_Clyde[(animation_counter++ / 15) % 2][3][0]);
		}
		else if (gdir == DOWN && !vulnerable && !ghost_eat)
		{
			setPixmap(texture_Clyde[0][4][0]);
			setPixmap(texture_Clyde[(animation_counter++ / 15) % 2][4][0]);
		}
	}
	else if (impossible)
	{
		if (gdir == RIGHT && !vulnerable && !ghost_eat)
		{
			setPixmap(texture_Clyde[0][1][1]);
			setPixmap(texture_Clyde[(animation_counter++ / 15) % 2][1][1]);
		}
		else if (gdir == LEFT && !vulnerable && !ghost_eat)
		{
			setPixmap(texture_Clyde[0][2][1]);
			setPixmap(texture_Clyde[(animation_counter++ / 15) % 2][2][1]);
		}
		else if (gdir == UP && !vulnerable && !ghost_eat)
		{
			setPixmap(texture_Clyde[0][3][1]);
			setPixmap(texture_Clyde[(animation_counter++ / 15) % 2][3][1]);
		}
		else if (gdir == DOWN && !vulnerable && !ghost_eat)
		{
			setPixmap(texture_Clyde[0][4][1]);
			setPixmap(texture_Clyde[(animation_counter++ / 15) % 2][4][1]);
		}
	}
}