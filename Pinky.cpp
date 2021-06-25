#include "Include.h"
Pinky::Pinky(QPoint position) : Ghosts()
{
	vulnerable = false;
	ghost_eat = false;
	isMoving = false;
	state = SCATTER;
	impossible = false;
	ai = false;

	gdir = UP;
	gcurrent_node = 29;

	gcan_right = false;
	gcan_left = false;
	gcan_up = true;
	gcan_down = false;

	plast_node = 73;
	pgoout = false;

	//texture ghost Pinky [texture][direction]
	texture_Pinky[0][1][0] = Sprites::instance()->get("pink-ghost-dx-0");
	texture_Pinky[0][2][0] = Sprites::instance()->get("pink-ghost-sx-0");
	texture_Pinky[0][3][0] = Sprites::instance()->get("pink-ghost-up-0");
	texture_Pinky[0][4][0] = Sprites::instance()->get("pink-ghost-down-0");

	texture_Pinky[1][1][0] = Sprites::instance()->get("pink-ghost-dx-1");
	texture_Pinky[1][2][0] = Sprites::instance()->get("pink-ghost-sx-1");
	texture_Pinky[1][3][0] = Sprites::instance()->get("pink-ghost-up-1");
	texture_Pinky[1][4][0] = Sprites::instance()->get("pink-ghost-down-1");

	//Mod Impossible
	texture_Pinky[0][1][1] = Sprites::instance()->get("pinkydx-0");
	texture_Pinky[0][2][1] = Sprites::instance()->get("pinkysx-0");
	texture_Pinky[0][3][1] = Sprites::instance()->get("pinkyup-0");
	texture_Pinky[0][4][1] = Sprites::instance()->get("pinkydown-0");

	texture_Pinky[1][1][1] = Sprites::instance()->get("pinkydx-1");
	texture_Pinky[1][2][1] = Sprites::instance()->get("pinkysx-1");
	texture_Pinky[1][3][1] = Sprites::instance()->get("pinkyup-1");
	texture_Pinky[1][4][1] = Sprites::instance()->get("pinkydown-1");

	setPixmap(texture_Pinky[0][3][0]);
	setPos(position - QPoint(pixmap().width(), pixmap().height()));

	//SetMask
	for (int i = 0; i < 2; i++)
		for (int j = 1; j < 5; j++)
		{
			texture_Pinky[i][j][0].setMask(texture_Pinky[i][j][0].createMaskFromColor(QColor(0, 0, 0)));
			texture_Pinky[i][j][1].setMask(texture_Pinky[i][j][1].createMaskFromColor(QColor(0, 0, 0)));
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
void Pinky::animate()
{
	Ghosts::animate();
	if (!impossible)
	{
		if (gdir == RIGHT && !vulnerable && !ghost_eat)
		{
			setPixmap(texture_Pinky[0][1][0]);
			setPixmap(texture_Pinky[(animation_counter++ / 15) % 2][1][0]);
		}
		else if (gdir == LEFT && !vulnerable && !ghost_eat)
		{
			setPixmap(texture_Pinky[0][2][0]);
			setPixmap(texture_Pinky[(animation_counter++ / 15) % 2][2][0]);
		}
		else if (gdir == UP && !vulnerable && !ghost_eat)
		{
			setPixmap(texture_Pinky[0][3][0]);
			setPixmap(texture_Pinky[(animation_counter++ / 15) % 2][3][0]);
		}
		else if (gdir == DOWN && !vulnerable && !ghost_eat)
		{
			setPixmap(texture_Pinky[0][4][0]);
			setPixmap(texture_Pinky[(animation_counter++ / 15) % 2][4][0]);
		}
	}
	else if (impossible)
	{
		if (gdir == RIGHT && !vulnerable && !ghost_eat)
		{
			setPixmap(texture_Pinky[0][1][1]);
			setPixmap(texture_Pinky[(animation_counter++ / 15) % 2][1][1]);
		}
		else if (gdir == LEFT && !vulnerable && !ghost_eat)
		{
			setPixmap(texture_Pinky[0][2][1]);
			setPixmap(texture_Pinky[(animation_counter++ / 15) % 2][2][1]);
		}
		else if (gdir == UP && !vulnerable && !ghost_eat)
		{
			setPixmap(texture_Pinky[0][3][1]);
			setPixmap(texture_Pinky[(animation_counter++ / 15) % 2][3][1]);
		}
		else if (gdir == DOWN && !vulnerable && !ghost_eat)
		{
			setPixmap(texture_Pinky[0][4][1]);
			setPixmap(texture_Pinky[(animation_counter++ / 15) % 2][4][1]);
		}
	}

}
	void Pinky::advance()
	{
		Ghosts::advance();

		//uscita forzata
		{
			if (gcurrent_node == 72)
			{
				gdir = DOWN;
				gcan_down = true;
				gcan_left = false;
				gcan_right = false;
				gcan_up = false;
				plast_node = 72;
			}
			if (gcurrent_node == 29)
			{
				if (!pgoout)
				{
					if (plast_node == 72)
					{
						gdir = DOWN;
						gcan_down = true;
						gcan_left = false;
						gcan_right = false;
						gcan_up = false;
					}
					if (plast_node == 73)
					{
						gdir = UP;
						gcan_down = false;
						gcan_left = false;
						gcan_right = false;
						gcan_up = true;
					}
				}
				else if (pgoout)
				{
					gdir = RIGHT;
					gcan_down = false;
					gcan_left = false;
					gcan_right = true;
					gcan_up = false;
				}
			}
			if (gcurrent_node == 73)
			{
				gdir = UP;
				can_down = false;
				gcan_left = false;
				gcan_right = false;
				gcan_up = true;
				plast_node = 73;
			}
			if (gcurrent_node == 30)
			{
				gdir = UP;
				gcan_down = false;
				gcan_left = false;
				gcan_right = false;
				gcan_up = true;
			}
			if (gcurrent_node == 70)
			{
				gdir = UP;
				gcan_down = false;
				gcan_left = false;
				gcan_up = true;
				gcan_right = false;
			}
		}

		if (Game::instance()->getSecond() < 6 && Sounds::instance()->isFinished("beginning"))
		{
			isMoving = true;
			moving_speed = 0.3;
			pgoout = false;
		}
		if (Game::instance()->getSecond() >= 6)
		{
			moving_speed = 1.05;
			isMoving = true;
			pgoout = true;
		}

		if (pacman->getIsDying())
			moving_speed = 0;

		//scatter
		if (state == SCATTER )
		{
			gtarget_x = 19;
			gtarget_y = 0;

			vicini[24][3] = -1;
		}

		//chase
		if (state == CHASE)
		{

			if (pacman->getDirection() == LEFT)
			{
				gtarget_x = pacman->x() - 26;
				gtarget_y = pacman->y();
			}
			else if (pacman->getDirection() == RIGHT)
			{
				gtarget_x = pacman->x() + 26;
				gtarget_y = pacman->y();
			}
			else if (pacman->getDirection() == DOWN)
			{
				gtarget_x = pacman->x();
				gtarget_y = pacman->y()+26;
			}
			else if (pacman->getDirection() == UP)
			{
				gtarget_x = pacman->x() - 26;
				gtarget_y = pacman->y() - 26;
			}
		}
		//Ai
		if (ai == true && !vulnerable && !ghost_eat)
		{
			
			state = CHASE;
		}
	}