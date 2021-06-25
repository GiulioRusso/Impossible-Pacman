#include "Include.h"
Inky::Inky(QPoint position) :Ghosts()
{
	vulnerable = false;
	ghost_eat = false;
	isMoving = false;
	state = SCATTER;
	impossible = false;
	ai = false;

	gdir = UP;
	gcurrent_node = 31;

	igoout = false;

	gcan_right = false;
	gcan_left = false;
	gcan_up = true;
	gcan_down = false;

	//texture ghost Inky [texture][direction]
	texture_Inky[0][1][0] = Sprites::instance()->get("heavenly-ghost-dx-0");
	texture_Inky[0][2][0] = Sprites::instance()->get("heavenly-ghost-sx-0");
	texture_Inky[0][3][0] = Sprites::instance()->get("heavenly-ghost-up-0");
	texture_Inky[0][4][0] = Sprites::instance()->get("heavenly-ghost-down-0");

	texture_Inky[1][1][0] = Sprites::instance()->get("heavenly-ghost-dx-1");
	texture_Inky[1][2][0] = Sprites::instance()->get("heavenly-ghost-sx-1");
	texture_Inky[1][3][0] = Sprites::instance()->get("heavenly-ghost-up-1");
	texture_Inky[1][4][0] = Sprites::instance()->get("heavenly-ghost-down-1");

	//Mod Impossible
	texture_Inky[0][1][1] = Sprites::instance()->get("inkydx-0");
	texture_Inky[0][2][1] = Sprites::instance()->get("inkysx-0");
	texture_Inky[0][3][1] = Sprites::instance()->get("inkyup-0");
	texture_Inky[0][4][1] = Sprites::instance()->get("inkydown-0");

	texture_Inky[1][1][1] = Sprites::instance()->get("inkydx-1");
	texture_Inky[1][2][1] = Sprites::instance()->get("inkysx-1");
	texture_Inky[1][3][1] = Sprites::instance()->get("inkyup-1");
	texture_Inky[1][4][1] = Sprites::instance()->get("inkydown-1");

	setPixmap(texture_Inky[0][3][0]);
	setPos(position - QPoint(pixmap().width(), pixmap().height()));

	for (int i = 0; i < 2; i++)
		for (int j = 1; j < 5; j++)
		{
			texture_Inky[i][j][0].setMask(texture_Inky[i][j][0].createMaskFromColor(QColor(0, 0, 0)));
			texture_Inky[i][j][1].setMask(texture_Inky[i][j][1].createMaskFromColor(QColor(0, 0, 0)));

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
void Inky::animate()
{
	Ghosts::animate();
	if (!impossible)
	{
		if (gdir == RIGHT && !vulnerable && !ghost_eat)
		{
			setPixmap(texture_Inky[0][1][0]);
			setPixmap(texture_Inky[(animation_counter++ / 15) % 2][1][0]);
		}
		else if (gdir == LEFT && !vulnerable && !ghost_eat)
		{
			setPixmap(texture_Inky[0][2][0]);
			setPixmap(texture_Inky[(animation_counter++ / 15) % 2][2][0]);
		}
		else if (gdir == UP && !vulnerable && !ghost_eat)
		{
			setPixmap(texture_Inky[0][3][0]);
			setPixmap(texture_Inky[(animation_counter++ / 15) % 2][3][0]);
		}
		else if (gdir == DOWN && !vulnerable && !ghost_eat)
		{
			setPixmap(texture_Inky[0][4][0]);
			setPixmap(texture_Inky[(animation_counter++ / 15) % 2][4][0]);
		}
	}
	if (impossible)
	{
		if (gdir == RIGHT && !vulnerable && !ghost_eat)
		{
			setPixmap(texture_Inky[0][1][1]);
			setPixmap(texture_Inky[(animation_counter++ / 15) % 2][1][1]);
		}
		else if (gdir == LEFT && !vulnerable && !ghost_eat)
		{
			setPixmap(texture_Inky[0][2][1]);
			setPixmap(texture_Inky[(animation_counter++ / 15) % 2][2][1]);
		}
		else if (gdir == UP && !vulnerable && !ghost_eat)
		{
			setPixmap(texture_Inky[0][3][1]);
			setPixmap(texture_Inky[(animation_counter++ / 15) % 2][3][1]);
		}
		else if (gdir == DOWN && !vulnerable && !ghost_eat)
		{
			setPixmap(texture_Inky[0][4][1]);
			setPixmap(texture_Inky[(animation_counter++ / 15) % 2][4][1]);
		}
	}
}
void Inky::advance()
{
	Ghosts::advance();
	blinky=Game::instance()->getBlinky();

	if (Game::instance()->getSecond() < 8 && Sounds::instance()->isFinished("beginning"))
	{
		isMoving = true;
		moving_speed = 0.3;
		igoout = false;
	}
	if (Game::instance()->getSecond() >= 8)
	{
		moving_speed = 1.05;
		isMoving = true;
		igoout = true;
	}
	if (pacman->getIsDying())
		moving_speed = 0;
	//uscita forzata
	{
		if (gcurrent_node == 74)
		{
			gdir = DOWN;
			gcan_down = true;
			gcan_left = false;
			gcan_right = false;
			gcan_up = false;
			ilast_node = 74;
		}
		if (gcurrent_node == 31)
		{
			if (!igoout)
			{
				if (ilast_node == 74)
				{
					gdir = DOWN;
					gcan_down = true;
					gcan_left = false;
					gcan_right = false;
					gcan_up = false;
				}
				if (ilast_node == 75)
				{
					gdir = UP;
					gcan_down = false;
					gcan_left = false;
					gcan_right = false;
					gcan_up = true;
				}
			}
			else if (igoout)
			{
				gdir = LEFT;
				gcan_down = false;
				gcan_left = true;
				gcan_right = false;
				gcan_up = false;
			}
		}
		if (gcurrent_node == 75)
		{
			gdir = UP;
			gcan_down = false;
			gcan_left = false;
			gcan_right = false;
			gcan_up = true;
			ilast_node = 75;
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

	//scatter
	if (state == SCATTER)
	{
		gtarget_x = 224;
		gtarget_y = 276;

		vicini[24][3] = -1;
	}

	//chase
	if (state == CHASE)
	{

		QPoint tmp;
		int dx, dy;
		if (pacman->getDirection() == LEFT)
		{
			tmp = QPoint(pacman->x() - 16, pacman->y());
			dx = tmp.x() - blinky->x();
			dy = tmp.y() - blinky->y();

			gtarget_x = tmp.x() + dx;
			gtarget_y = tmp.y() + dy;
		}
		else if (pacman->getDirection() == RIGHT)
		{
			tmp = QPoint(pacman->x() + 16, pacman->y());
			dx = tmp.x() - blinky->x();
			dy = tmp.y() - blinky->y();

			gtarget_x = tmp.x() + dx;
			gtarget_y = tmp.y() + dy;
		}
		else if (pacman->getDirection() == DOWN)
		{
			tmp = QPoint(pacman->x(), pacman->y() + 16);
			dx = tmp.x() - blinky->x();
			dy = tmp.y() - blinky->y();

			gtarget_x = tmp.x() + dx;
			gtarget_y = tmp.y() + dy;
		}
		else if (pacman->getDirection() == UP)
		{
			tmp = QPoint(pacman->x() - 16, pacman->y() - 16);
			dx = tmp.x() - blinky->x();
			dy = tmp.y() - blinky->y();

			gtarget_x = tmp.x() + dx;
			gtarget_y = tmp.y() + dy;
		}
	}
	//AI
	if (ai == true && !vulnerable && !ghost_eat)
	{
		state = CHASE;
	}
}