#include "Include.h"

Ghosts::Ghosts() : Entity()
{
	impossible				= true;
	ghost_eat				=	false;
	vulnerable				=	false;
	vulnerable_counter		=	0;
	moving_speed			= 0;
	isMoving				= false;
	state					= SCATTER;
	fantasma_mangiato		= false;
	
	//Collegamento all entrata della box dei fantasmi
	vicini[24][3] =70;
	vicini[30][2]=70;
	
	//Calcolo dei camimini se mi trovo nella modalita impossible
	if (impossible)
	{
		calcoloDistMin();

		for (int i = 0; i < 76; i++)
			for (int j = 0; j < 76; j++)
				cammini[i][j] = cammino(i, j);
	}
	
	//texture Vulnerable
	texture_vulnerable[0][0]		= Sprites::instance()->get("ghost-vulnerable-0");
	texture_vulnerable[1][0]		= Sprites::instance()->get("ghost-vulnerable-1");

	texture_finish_vulnerable[0][0] = Sprites::instance()->get("ghost-vulnerable-0");
	texture_finish_vulnerable[1][0] = Sprites::instance()->get("ghost-vulnerable-1");
	texture_finish_vulnerable[2][0] = Sprites::instance()->get("ghost-vulnerable-0");
	texture_finish_vulnerable[3][0] = Sprites::instance()->get("ghost-vulnerable-1");
	texture_finish_vulnerable[4][0] = Sprites::instance()->get("ghost-finish-vulnerable-0");
	texture_finish_vulnerable[5][0] = Sprites::instance()->get("ghost-finish-vulnerable-1");
	texture_finish_vulnerable[6][0] = Sprites::instance()->get("ghost-finish-vulnerable-0");
	texture_finish_vulnerable[7][0] = Sprites::instance()->get("ghost-finish-vulnerable-1");

	//texture Vulnerable AI
	texture_vulnerable[0][1]		= Sprites::instance()->get("ghost-vulnerable-ai-0");
	texture_vulnerable[1][1]		= Sprites::instance()->get("ghost-vulnerable-ai-1");

	texture_finish_vulnerable[0][1] = Sprites::instance()->get("ghost-vulnerable-ai-0");
	texture_finish_vulnerable[1][1] = Sprites::instance()->get("ghost-vulnerable-ai-1");
	texture_finish_vulnerable[2][1] = Sprites::instance()->get("ghost-vulnerable-ai-0");
	texture_finish_vulnerable[3][1] = Sprites::instance()->get("ghost-vulnerable-ai-1");
	texture_finish_vulnerable[4][1] = Sprites::instance()->get("ghost-finish-vulnerable-ai-0");
	texture_finish_vulnerable[5][1] = Sprites::instance()->get("ghost-finish-vulnerable-ai-1");
	texture_finish_vulnerable[6][1] = Sprites::instance()->get("ghost-finish-vulnerable-ai-0");
	texture_finish_vulnerable[7][1] = Sprites::instance()->get("ghost-finish-vulnerable-ai-1");
	
	//Eaten
	texture_die[0]					= Sprites::instance()->get("eyes-dx");
	texture_die[1]					= Sprites::instance()->get("eyes-sx");
	texture_die[2]					= Sprites::instance()->get("eyes-up");
	texture_die[3]					= Sprites::instance()->get("eyes-down");

	//Set Mask
	//Eyes
	for (int i = 0; i < 4; i++)
	{
		texture_die[i].setMask(texture_die[i].createMaskFromColor(QColor(0, 0, 0)));
		texture_die[i].setMask(texture_die[i].createMaskFromColor(QColor(0, 0, 0)));

	}
	//Vulnerable
	for (int i = 0; i < 8; i++)
	{
		texture_finish_vulnerable[i][0].setMask(texture_finish_vulnerable[i][0].createMaskFromColor(QColor(0, 0, 0)));
		texture_finish_vulnerable[i][1].setMask(texture_finish_vulnerable[i][1].createMaskFromColor(QColor(0, 0, 0)));
	}
	texture_vulnerable[0][0].setMask(texture_vulnerable[0][0].createMaskFromColor(QColor(0, 0, 0)));
	texture_vulnerable[1][0].setMask(texture_vulnerable[1][0].createMaskFromColor(QColor(0, 0, 0)));
	texture_vulnerable[0][1].setMask(texture_vulnerable[0][0].createMaskFromColor(QColor(0, 0, 0)));
	texture_vulnerable[1][1].setMask(texture_vulnerable[1][0].createMaskFromColor(QColor(0, 0, 0)));

}
void Ghosts::advance()
{
	Entity::advance();
	pacman = Game::instance()->getPacman();

	//TELETRASPORTO uguale a pacman
	{
		if (x() < -13 && y() == 129 && gdir == LEFT)
		{
			setX(224);
			setY(129);
		}
		if (x() > 224 && y() == 129 && gdir == RIGHT)
		{
			setX(-13);
			setY(129);
		}
	}

	//Collisioni Fantasma-Pacman
	{
		//CASO pacman si trova a destra del fantasma
		if (pacman->x() > x() && pacman->y() == y())
		{
			int dist = pacman->x() - x();
			if (dist < 10 )
			{
				if (state==VULNERABLE)
				{
					die();										//Fantasma entra nello stato eaten
					fantasma_mangiato = true;				
				}
				else if (state != VULNERABLE && !ghost_eat)
				{
					moving_speed = 0;
					pacman->die();
					//ghost_music->stop();
				}
			}
		}
		//CASO pacman si trova a sinistra del fantasma 
		else if (pacman->x() < x() && pacman->y() == y())
		{
			int dist = x() - pacman->x();
			if (dist < 10)
			{
				if (state == VULNERABLE)
				{
					die();
					fantasma_mangiato = true;			
				}
				else if (state != VULNERABLE && !ghost_eat)
				{
					moving_speed = 0;
					pacman->die();
					//ghost_music->stop();
				}
			}
		}
		//CASO pacman sta sotto il fantasma
		else if (pacman->x() == x() && pacman->y() > y())
		{
			int dist = pacman->y() - y();
			if (dist < 10)
			{
				if (state == VULNERABLE)
				{
					die();
					fantasma_mangiato = true;
				}
				else if (state != VULNERABLE && !ghost_eat)
				{
					moving_speed = 0;
					pacman->die();
					//ghost_music->stop();
				}
			}
		}
		//CASO pacman sta sopra il fantasma
		else if (pacman->x() == x() && pacman->y() < y())
		{
			int dist = y() - pacman->y();
			if (dist < 10)
				if (state == VULNERABLE)
				{
					die();
					fantasma_mangiato = true;
					
				}
				else if (state != VULNERABLE && !ghost_eat)
				{
					moving_speed = 0;
					pacman->die();
					//ghost_music->stop();
				}
		}
	
	}

	//eaten
	if (state == EATEN)
	{
		//Modifico la velocita
		moving_speed = 1.8;
		//Permetto al fantasma di rientrare nella box
		vicini[24][3] = 30;

		//Assegno il punto in cui deve dirigersi (nodo 31)
		gtarget_x = 113;
		gtarget_y = 137;

		//Se sono arrivato al nodo 31
		if (gcurrent_node == 30)
		{
			gdir = UP; //devo uscire dalla box
			gcan_right = false;
			gcan_left = false;
			gcan_up = true;
			gcan_down = false;

			ghost_eat = false;
			if (impossible == true)
			{
				state = AI;
			}
			else
				state = SCATTER;
		}
	}


	//scatter & chase
	if (state != VULNERABLE && state != AI)
	{
		//right scatter & chase
		if (gdir == RIGHT && gcan_right)
		{
			if (gdest_node == vicini[gcurrent_node][1])
				gcurrent_node = gdest_node;

			//assegno destinazione in base alla direzione
			gdest_node = vicini[gcurrent_node][0];

			//se sono arrivato
			if (x() == nodo[gdest_node].x - 8)
			{
				//aggiorno la mia posizione
				gcurrent_node = gdest_node;

				//calcolo le distanze euclidee per ogni direzione possibile
				if (vicini[gcurrent_node][0] != -1) //se non ho un muro calcolo
				{
					gcan_right = true;
					gcx = gtarget_x - (x() + 8);
					gcy = gtarget_y - (y());
					gdright = sqrt((gcx * gcx) + (gcy * gcy));
				}
				else //se ho un muro non posso scegliere la direzione e la distanza sarà la più sconveniente possibile
				{
					gcan_right = false;
					gdright = 720;
				}

				if (vicini[gcurrent_node][2] != -1)
				{
					gcan_up = true;
					gcx = gtarget_x - (x());
					gcy = gtarget_y - (y() - 8);
					gdup = sqrt((gcx * gcx) + (gcy * gcy));
				}
				else
				{
					gcan_up = false;
					gdup = 720;
				}

				if (vicini[gcurrent_node][3] != -1)
				{
					gcan_down = true;
					gcx = gtarget_x - (x());
					gcy = gtarget_y - (y() + 8);
					gddown = sqrt((gcx * gcx) + (gcy * gcy));
				}
				else
				{
					gcan_down = false;
					gddown = 720;
				}

				gdleft = 720; //il fantasmino non può tornare indietro
				gdmin = gdleft; //sicuro quindi avrò una distanza più piccola di questa
				//minimo tra 3 distanze
				if ((gdright <= gdmin) && (gdright <= gdup) && (gdright <= gddown))
				{
					gdmin = gdright;
					////std::cout<<"g sceglie right"<<//std::endl;
					gdir = RIGHT;
					gcan_right = true;
					gcan_left = false;
					gcan_up = false;
					gcan_down = false;
				}
				else if ((gdup <= gdmin) && (gdup <= gdright) && (gdup <= gddown))
				{
					gdmin = gdup;
					////std::cout<<"g sceglie up"<<//std::endl;
					gdir = UP;
					gcan_right = false;
					gcan_left = false;
					gcan_up = true;
					gcan_down = false;
				}
				else if ((gddown <= gdmin) && (gddown <= gdright) && (gddown <= gdup))
				{
					gdmin = gddown;
					////std::cout<<"g sceglie down"<<//std::endl;
					gdir = DOWN;
					gcan_right = false;
					gcan_left = false;
					gcan_up = false;
					gcan_down = true;
				}
				else //se nessuna delle 3 (esclusa quella opposta alla dir corrente) è la minima
				{
					//vuol dire che il fantasmino non può scegliere nessuna direzione
					std::cout << "Errore scelta direzione" << std::endl;
				}

			}
			//se deraglio (supero la coordinata del dest)
			else if (x() > nodo[gdest_node].x - 8)
			{
				//a meno che non stia andando nel teletrasporto
				if (gcurrent_node != 33)
					//mi rimetto sui binari
					setX(nodo[gdest_node].x - 8);
				else
				{
					//altrimenti continuo 
					setX(x() + moving_speed);
					//finchè non mi trovo in un intorno del nodo opposto del teletrasporto
					if (x() >= 45 && x() <= 55)
						//mi rimetto nei binari
						setX(nodo[27].x - 8);
				}
			}
			//se non sono ancora arrivato
			else
			{
				//mi muovo
				isMoving= true;
				setX(x() + moving_speed);
				gcan_left = false;
				gcan_up = false;
				gcan_down = false;
			}
		}
		//left scatter & chase
		if (gdir == LEFT && gcan_left == true)
		{
			if (gdest_node == vicini[gcurrent_node][0])
				gcurrent_node = gdest_node;

			gdest_node = vicini[gcurrent_node][1];

			if (x() == nodo[gdest_node].x - 8)
			{
				gcurrent_node = gdest_node;

				if (vicini[gcurrent_node][1] != -1)
				{
					gcan_left = true;
					gcx = gtarget_x - (x() - 8);
					gcy = gtarget_y - (y());
					gdleft = sqrt((gcx * gcx) + (gcy * gcy));
				}
				else
				{
					gcan_left = false;
					gdleft = 720;
				}

				if (vicini[gcurrent_node][2] != -1)
				{
					gcan_up = true;
					gcx = gtarget_x - (x());
					gcy = gtarget_y - (y() - 8);
					gdup = sqrt((gcx * gcx) + (gcy * gcy));
				}
				else
				{
					gcan_up = false;
					gdup = 720;
				}

				if (vicini[gcurrent_node][3] != -1)
				{
					gcan_down = true;
					gcx = gtarget_x - (x());
					gcy = gtarget_y - (y() + 8);
					gddown = sqrt((gcx * gcx) + (gcy * gcy));
				}
				else
				{
					gcan_down = false;
					gddown = 720;
				}

				gdright = 720;
				gdmin = gdright;
				if ((gdleft <= gdmin) && (gdleft <= gdup) && (gdleft <= gddown))
				{
					gdmin = gdleft;
					////std::cout<<"g sceglie left"<<//std::endl;
					gdir = LEFT;
					gcan_right = false;
					gcan_left = true;
					gcan_up = false;
					gcan_down = false;
				}
				else if ((gdup <= gdmin) && (gdup <= gdleft) && (gdup <= gddown))
				{
					gdmin = gdup;
					////std::cout<<"g sceglie up"<<//std::endl;
					gdir = UP;
					gcan_right = false;
					gcan_left = false;
					gcan_up = true;
					gcan_down = false;
				}
				else if ((gddown <= gdmin) && (gddown <= gdleft) && (gddown <= gdup))
				{
					gdmin = gddown;
					////std::cout<<"g sceglie down"<<//std::endl;
					gdir = DOWN;
					gcan_right = false;
					gcan_left = false;
					gcan_up = false;
					gcan_down = true;
				}
				else
				{
					std::cout << "Errore scelta direzione" << std::endl;
				}
			}
			else if (x() < nodo[gdest_node].x - 8)
			{
				if (gcurrent_node != 27)
					setX(nodo[gdest_node].x - 8);
				else
				{
					setX(x() - moving_speed);
					if (x() >= 155 && x() <= 165)
						setX(nodo[33].x - 8);
				}
			}
			else
			{
				isMoving= true;
				setX(x() - moving_speed);
				gcan_right = false;
				gcan_up = false;
				gcan_down = false;
			}
		}
		//up scatter & chase
		if (gdir == UP && gcan_up == true)
		{
			if (gdest_node == vicini[gcurrent_node][3])
				gcurrent_node = gdest_node;

			gdest_node = vicini[gcurrent_node][2];

			if (y() == nodo[gdest_node].y - 8)
			{
				gcurrent_node = gdest_node;

				if (vicini[gcurrent_node][0] != -1)
				{
					gcan_right = true;
					gcx = gtarget_x - (x() + 8);
					gcy = gtarget_y - (y());
					gdright = sqrt((gcx * gcx) + (gcy * gcy));
				}
				else
				{
					gcan_right = false;
					gdright = 720;
				}

				if (vicini[gcurrent_node][1] != -1)
				{
					gcan_left = true;
					gcx = gtarget_x - (x() - 8);
					gcy = gtarget_y - (y());
					gdleft = sqrt((gcx * gcx) + (gcy * gcy));
				}
				else
				{
					gcan_left = false;
					gdleft = 720;
				}

				if (vicini[gcurrent_node][2] != -1)
				{
					gcan_up = true;
					gcx = gtarget_x - (x());
					gcy = gtarget_y - (y() - 8);
					gdup = sqrt((gcx * gcx) + (gcy * gcy));
				}
				else
				{
					gcan_up = false;
					gdup = 720;
				}

				gddown = 720;
				gdmin = gddown;
				if ((gdright <= gdmin) && (gdright <= gdleft) && (gdright <= gdup))
				{
					gdmin = gdright;
					////std::cout<<"g sceglie right"<<//std::endl;
					gdir = RIGHT;
					gcan_right = true;
					gcan_left = false;
					gcan_up = false;
					gcan_down = false;
				}

				else if ((gdleft <= gdmin) && (gdleft <= gdright) && (gdleft <= gdup))
				{
					gdmin = gdleft;
					////std::cout<<"g sceglie left"<<//std::endl;
					gdir = LEFT;
					gcan_right = false;
					gcan_left = true;
					gcan_up = false;
					gcan_down = false;
				}
				else if ((gdup <= gdmin) && (gdup <= gdleft) && (gdup <= gdright))
				{
					gdmin = gdup;
					////std::cout<<"g sceglie up"<<//std::endl;
					gdir = UP;
					gcan_right = false;
					gcan_left = false;
					gcan_up = true;
					gcan_down = false;
				}
				else
				{
					std::cout << "Errore scelta direzione" << std::endl;
				}

			}
			else if (y() < nodo[gdest_node].y - 8)
			{
				setY(nodo[gdest_node].y - 8);
			}
			else
			{
				isMoving= true;
				setY(y() - moving_speed);
				gcan_right = false;
				gcan_left = false;
				gcan_down = false;
			}
		}
		//down scatter & chase
		if (gdir == DOWN && gcan_down == true)
		{
			if (gdest_node == vicini[gcurrent_node][2])
				gcurrent_node = gdest_node;

			gdest_node = vicini[gcurrent_node][3];

			if (y() == nodo[gdest_node].y - 8)
			{
				gcurrent_node = gdest_node;

				if (vicini[gcurrent_node][0] != -1)
				{
					gcan_right = true;
					gcx = gtarget_x - (x() + 8);
					gcy = gtarget_y - (y());
					gdright = sqrt((gcx * gcx) + (gcy * gcy));
				}
				else
				{
					gcan_right = false;
					gdright = 720;
				}

				if (vicini[gcurrent_node][1] != -1)
				{
					gcan_left = true;
					gcx = gtarget_x - (x() - 8);
					gcy = gtarget_y - (y());
					gdleft = sqrt((gcx * gcx) + (gcy * gcy));
				}
				else
				{
					gcan_left = false;
					gdleft = 720;
				}

				if (vicini[gcurrent_node][3] != -1)
				{
					gcan_down = true;
					gcx = gtarget_x - (x());
					gcy = gtarget_y - (y() + 8);
					gddown = sqrt((gcx * gcx) + (gcy * gcy));
				}
				else
				{
					gcan_down = false;
					gddown = 720;
				}

				gdup = 720;
				gdmin = gdup;
				if ((gdright <= gdmin) && (gdright <= gdleft) && (gdright <= gddown))
				{
					gdmin = gdright;
					////std::cout<<"g sceglie right"<<//std::endl;
					gdir = RIGHT;
					gcan_right = true;
					gcan_left = false;
					gcan_up = false;
					gcan_down = false;
				}
				else if ((gdleft <= gdmin) && (gdleft <= gdright) && (gdleft <= gddown))
				{
					gdmin = gdleft;
					////std::cout<<"g sceglie left"<<//std::endl;
					gdir = LEFT;
					gcan_right = false;
					gcan_left = true;
					gcan_up = false;
					gcan_down = false;
				}
				else if ((gddown <= gdmin) && (gddown <= gdleft) && (gddown <= gdright))
				{
					gdmin = gddown;
					////std::cout<<"g sceglie down"<<//std::endl;
					gdir = DOWN;
					gcan_right = false;
					gcan_left = false;
					gcan_up = false;
					gcan_down = true;
				}
				else
				{
					std::cout << "Errore scelta direzione" << std::endl;
				}
			}
			else if (y() > nodo[gdest_node].y - 8)
			{
				setY(nodo[gdest_node].y - 8);
			}
			else
			{
				isMoving= true;
				setY(y() + moving_speed);
				gcan_right = false;
				gcan_left = false;
				gcan_up = false;
			}

		};
	}

	//vulnerable
	if (state == VULNERABLE)
	{
		//movimento uguale a scatter e chase, ma cambia la velocità
		moving_speed = 0.8;
		vicini[24][3] = -1;

		//right vulnerable
		if (gdir == RIGHT && gcan_right)
		{
			if (gdest_node == vicini[gcurrent_node][1])
				gcurrent_node = gdest_node;

			gdest_node = vicini[gcurrent_node][0];

			if (x() == nodo[gdest_node].x - 8)
			{
				gcurrent_node = gdest_node;

				//scelta casuale della direzione
				do
				{
					gtmp_dir = rand() % 4 + 1;
				} while (vicini[gcurrent_node][gtmp_dir - 1] == -1 || gtmp_dir == 2);
				//a meno che non ci sia un muro o sia la dir opposta a quella corrente

				if (gtmp_dir == 1)
				{
					gdir = RIGHT;
					gcan_right = true;
					gcan_left = false;
					gcan_up = false;
					gcan_down = false;
				}
				else if (gtmp_dir == 2)
				{
					std::cout << "Direzione casuale non possibile" << std::endl;
				}
				else if (gtmp_dir == 3)
				{
					gdir = UP;
					gcan_right = false;
					gcan_left = false;
					gcan_up = true;
					gcan_down = false;
				}
				else if (gtmp_dir == 4)
				{
					gdir = DOWN;
					gcan_right = false;
					gcan_left = false;
					gcan_up = false;
					gcan_down = true;
				}
			}
			//se deraglio
			else if (x() > nodo[gdest_node].x - 8)
			{
				if (gcurrent_node != 33)
					setX(nodo[gdest_node].x - 8);
				else
				{
					setX(x() + moving_speed);
					if (x() >= 45 && x() <= 55)
						setX(nodo[27].x - 8);
				}
			}
			//allora mi muovo
			else
			{
				isMoving= true;
				setX(x() + moving_speed);
				gcan_left = false;
				gcan_up = false;
				gcan_down = false;
			}
		}
		//left vulnerable
		if (gdir == LEFT && gcan_left == true)
		{
			if (gdest_node == vicini[gcurrent_node][0])
				gcurrent_node = gdest_node;

			gdest_node = vicini[gcurrent_node][1];

			if (x() == nodo[gdest_node].x - 8)
			{
				gcurrent_node = gdest_node;

				do
				{
					gtmp_dir = rand() % 4 + 1;
				} while (vicini[gcurrent_node][gtmp_dir - 1] == -1 || gtmp_dir == 1);

				if (gtmp_dir == 1)
				{
					std::cout << "Direzione casuale non possibile" << std::endl;
				}
				else if (gtmp_dir == 2)
				{
					gdir = LEFT;
					gcan_right = false;
					gcan_left = true;
					gcan_up = false;
					gcan_down = false;
				}
				else if (gtmp_dir == 3)
				{
					gdir = UP;
					gcan_right = false;
					gcan_left = false;
					gcan_up = true;
					gcan_down = false;
				}
				else if (gtmp_dir == 4)
				{
					gdir = DOWN;
					gcan_right = false;
					gcan_left = false;
					gcan_up = false;
					gcan_down = true;
				}
			}
			else if (x() < nodo[gdest_node].x - 8)
			{
				if (gcurrent_node != 27)
					setX(nodo[gdest_node].x - 8);
				else
				{
					setX(x() - moving_speed);
					if (x() >= 155 && x() <= 165)
						setX(nodo[33].x - 8);
				}
			}
			else
			{
				isMoving= true;
				setX(x() - moving_speed);
				gcan_right = false;
				gcan_up = false;
				gcan_down = false;
			}
		}
		//up vulnerable
		if (gdir == UP && gcan_up == true)
		{
			if (gdest_node == vicini[gcurrent_node][3])
				gcurrent_node = gdest_node;

			gdest_node = vicini[gcurrent_node][2];

			if (y() == nodo[gdest_node].y - 8)
			{
				gcurrent_node = gdest_node;

				do
				{
					gtmp_dir = rand() % 4 + 1;
				} while (vicini[gcurrent_node][gtmp_dir - 1] == -1 || gtmp_dir == 4);

				if (gtmp_dir == 1)
				{
					gdir = RIGHT;
					gcan_right = true;
					gcan_left = false;
					gcan_up = false;
					gcan_down = false;
				}
				else if (gtmp_dir == 2)
				{
					gdir = LEFT;
					gcan_right = false;
					gcan_left = true;
					gcan_up = false;
					gcan_down = false;
				}
				else if (gtmp_dir == 3)
				{
					gdir = UP;
					gcan_right = false;
					gcan_left = false;
					gcan_up = true;
					gcan_down = false;
				}
				else if (gtmp_dir == 4)
				{
					std::cout << "Direzione casuale non possibile" << std::endl;
				}
			}
			else if (y() < nodo[gdest_node].y - 8)
			{
				setY(nodo[gdest_node].y - 8);
			}
			else
			{
				isMoving= true;
				setY(y() - moving_speed);
				gcan_right = false;
				gcan_left = false;
				gcan_down = false;
			}
		}
		//down vulnerable
		if (gdir == DOWN && gcan_down == true)
		{
			if (gdest_node == vicini[gcurrent_node][2])
				gcurrent_node = gdest_node;

			gdest_node = vicini[gcurrent_node][3];

			if (y() == nodo[gdest_node].y - 8)
			{
				gcurrent_node = gdest_node;

				do
				{
					gtmp_dir = rand() % 4 + 1;
				} while (vicini[gcurrent_node][gtmp_dir - 1] == -1 || gtmp_dir == 3);

				if (gtmp_dir == 1)
				{
					gdir = RIGHT;
					gcan_right = true;
					gcan_left = false;
					gcan_up = false;
					gcan_down = false;
				}
				else if (gtmp_dir == 2)
				{
					gdir = LEFT;
					gcan_right = false;
					gcan_left = true;
					gcan_up = false;
					gcan_down = false;
				}
				else if (gtmp_dir == 3)
				{
					std::cout << "Direzione casuale non possibile" << std::endl;
				}
				else if (gtmp_dir == 4)
				{
					gdir = DOWN;
					gcan_right = false;
					gcan_left = false;
					gcan_up = false;
					gcan_down = true;
				}
			}
			else if (y() > nodo[gdest_node].y - 8)
			{
				setY(nodo[gdest_node].y - 8);
			}
			else
			{
				isMoving = true;
				setY(y() + moving_speed);
				gcan_right = false;
				gcan_left = false;
				gcan_up = false;
			}
		};
	}

	//AI
	if (state == AI)
	{
		//decido dove andare uguagliando il dest con il vicino rispetto a dove sto
		if (vicini[gcurrent_node][0] == gdest_node)
		{
			gdir = RIGHT;
			gcan_right = true;
			gcan_left = false;
			gcan_up = false;
			gcan_down = false;
		}
		else if (vicini[gcurrent_node][1] == gdest_node)
		{
			gdir = LEFT;
			gcan_right = false;
			gcan_left = true;
			gcan_up = false;
			gcan_down = false;
		}
		else if (vicini[gcurrent_node][2] == gdest_node)
		{
			gdir = UP;
			gcan_right = false;
			gcan_left = false;
			gcan_up = true;
			gcan_down = false;
		}
		else if (vicini[gcurrent_node][3] == gdest_node)
		{
			gdir = DOWN;
			gcan_right = false;
			gcan_left = false;
			gcan_up = false;
			gcan_down = true;
		}
		else
		{
			std::cout << "ERRORE: " << gcurrent_node << " non � vicino di " << gdest_node << std::endl;
		}

		//right 
		if (gdir == RIGHT && gcan_right)
		{
			if (gdest_node == vicini[gcurrent_node][1])
				gcurrent_node = gdest_node;

			//inizializzo dest
			gdest_node = vicini[gcurrent_node][0];

			//se sono arrivato
			if (x() == nodo[gdest_node].x - 8)
			{
				gcurrent_node = gdest_node;
				//scelgo il nuovo dest come primo elemento del cammino
				if (!cammini[gcurrent_node][pacman->getDestNode()].empty())
				{
					gdest_node = cammini[gcurrent_node][pacman->getDestNode()].front();
					//assegno percorso da disegnare
					percorsoAi = cammini[gcurrent_node][pacman->getDestNode()];
				}
				else //se il cammino è vuoto vuol dire che nel mio dest ci sta pacman
					gdest_node = pacman->getCurrentNode(); //allora la mia dest è proprio il current_node di pacman
			}
			//se deraglio mi rimetto nei binari
			else if (x() > nodo[gdest_node].x - 8)
			{
				//incluso caso particolare del teletrasporto
				if (gcurrent_node != 33)
					setX(nodo[gdest_node].x - 8);
				else
				{
					setX(x() + moving_speed);
					if (x() >= 45 && x() <= 55)
						setX(nodo[27].x - 8);
				}
			}
			//altrimenti mi muovo
			else
			{
				isMoving = true;
				setX(x() + moving_speed);
				gcan_left = false;
				gcan_up = false;
				gcan_down = false;
			}
		}
		//left 
		if (gdir == LEFT && gcan_left == true)
		{
			if (gdest_node == vicini[gcurrent_node][0])
				gcurrent_node = gdest_node;

			gdest_node = vicini[gcurrent_node][1];

			if (x() == nodo[gdest_node].x - 8)
			{
				gcurrent_node = gdest_node;
				if (!cammini[gcurrent_node][pacman->getDestNode()].empty())
				{
					gdest_node = cammini[gcurrent_node][pacman->getDestNode()].front();
					percorsoAi = cammini[gcurrent_node][pacman->getDestNode()];
				}
				else
					gdest_node = pacman->getCurrentNode();
			}
			else if (x() < nodo[gdest_node].x - 8)
			{
				if (gcurrent_node != 27)
					setX(nodo[gdest_node].x - 8);
				else
				{
					setX(x() - moving_speed);
					if (x() >= 155 && x() <= 165)
						setX(nodo[33].x - 8);
				}
			}
			else
			{
				isMoving = true;
				setX(x() - moving_speed);
				gcan_right = false;
				gcan_up = false;
				gcan_down = false;
			}
		}
		//up
		if (gdir == UP && gcan_up == true)
		{
			if (gdest_node == vicini[gcurrent_node][3])
				gcurrent_node = gdest_node;

			gdest_node = vicini[gcurrent_node][2];

			if (y() == nodo[gdest_node].y - 8)
			{
				gcurrent_node = gdest_node;
				if (!cammini[gcurrent_node][pacman->getDestNode()].empty())
				{
					gdest_node = cammini[gcurrent_node][pacman->getDestNode()].front();
					percorsoAi = cammini[gcurrent_node][pacman->getDestNode()];
				}
				else
					gdest_node = pacman->getCurrentNode();
			}
			else if (y() < nodo[gdest_node].y - 8)
			{
				setY(nodo[gdest_node].y - 8);
			}
			else
			{
				isMoving = true;
				setY(y() - moving_speed);
				gcan_right = false;
				gcan_left = false;
				gcan_down = false;
			}
		}
		//down
		if (gdir == DOWN && gcan_down == true)
		{
			if (gdest_node == vicini[gcurrent_node][2])
				gcurrent_node = gdest_node;

			gdest_node = vicini[gcurrent_node][3];
			if (y() == nodo[gdest_node].y - 8)
			{
				gcurrent_node = gdest_node;
				if (!cammini[gcurrent_node][pacman->getDestNode()].empty())
				{
					gdest_node = cammini[gcurrent_node][pacman->getDestNode()].front();
					percorsoAi = cammini[gcurrent_node][pacman->getDestNode()];
				}
				else
					gdest_node = pacman->getCurrentNode();
			}
			else if (y() > nodo[gdest_node].y - 8)
			{
				setY(nodo[gdest_node].y - 8);
			}
			else
			{
				isMoving = true;
				setY(y() + moving_speed);
				gcan_right = false;
				gcan_left = false;
				gcan_up = false;
			}
		}
	}

	
	

	//OROLOGIO determina i vari stati di gioco nella modalita normale
	if (!ai)
	{
		if (Game::instance()->getSecond() <= 13 && state != EATEN && state != VULNERABLE)
			state = SCATTER;
		else if (Game::instance()->getSecond() > 13 && Game::instance()->getSecond() <= 33 && state != EATEN && state != VULNERABLE)
			state = CHASE;
		else if (Game::instance()->getSecond() > 33 && Game::instance()->getSecond() <= 40 && state != EATEN && state != VULNERABLE)
			state = SCATTER;
		else if (Game::instance()->getSecond() > 40 && Game::instance()->getSecond() <= 60 && state != EATEN && state != VULNERABLE)
			state = CHASE;
		else if (Game::instance()->getSecond() > 60 && Game::instance()->getSecond() <= 65 && state != EATEN && state != VULNERABLE)
			state = SCATTER;
		else if (Game::instance()->getSecond() > 65 && Game::instance()->getSecond() <= 85 && state != EATEN && state != VULNERABLE)
			state = CHASE;
		else if (Game::instance()->getSecond() > 85 && Game::instance()->getSecond() <= 90 && state != EATEN && state != VULNERABLE)
			state = SCATTER;
		else if (Game::instance()->getSecond() > 90 && state != EATEN && state != VULNERABLE)
			state = CHASE;
		else if (Sounds::instance()->isFinished("hunt") && state != EATEN)
		{
			moving_speed = 1.05;
			if ((Game::instance()->getSecond() > 13 && Game::instance()->getSecond() <= 33) || (Game::instance()->getSecond() > 40 && Game::instance()->getSecond() <= 60) || (Game::instance()->getSecond() > 65 && Game::instance()->getSecond() <= 85) || Game::instance()->getSecond() > 95)
				state = CHASE;
			else
				state = SCATTER;
		}
	}
}
void Ghosts::animate()
{
	if (vulnerable && !impossible)
	{
		Sounds::instance()->setLoops("hunt", 11);
		Sounds::instance()->stop("siren_1");

		if (vulnerable_counter < 180)
			setPixmap(texture_vulnerable[(vulnerable_counter++ / 15) % 2][0]);

		else if (vulnerable_counter >= 180 && !Sounds::instance()->isFinished("hunt"))
			setPixmap(texture_finish_vulnerable[(vulnerable_counter++ / 4) % 8][0]);

		else if ( Sounds::instance()->isFinished("hunt"))
		{
			setVulnerable(false);
			vulnerable_counter = 0;
			Sounds::instance()->play("siren_1");
			Sounds::instance()->setLoopsInfinite("siren_1");
		}
	}
	else if (vulnerable && impossible)
	{
		Sounds::instance()->setLoops("hunt", 11);
		Sounds::instance()->stop("siren_1");

		if (vulnerable_counter < 180)
			setPixmap(texture_vulnerable[(vulnerable_counter++ / 15) % 2][1]);

		else if (vulnerable_counter >= 180 && !Sounds::instance()->isFinished("hunt"))
			setPixmap(texture_finish_vulnerable[(vulnerable_counter++ / 4) % 8][1]);

		else if (Sounds::instance()->isFinished("hunt"))
		{
			setVulnerable(false);
			vulnerable_counter = 0;
			Sounds::instance()->play("siren_1");
			Sounds::instance()->setLoopsInfinite("siren_1");	
		}
	}
	if (ghost_eat)
	{
	
		if (Sounds::instance()->isFinished("eyes"))
		{
			Sounds::instance()->play("eyes");
		}
		if (dir == RIGHT)
			setPixmap(texture_die[0]);

		if (dir == LEFT)
			setPixmap(texture_die[1]);

		if (dir == UP)
			setPixmap(texture_die[2]);

		if (dir == DOWN)
			setPixmap(texture_die[3]);

		if (x() == 105 && y() >= 126 && y() <= 129)
		{
			setVulnerable(false);
			Sounds::instance()->play("siren_1");
			Sounds::instance()->setLoopsInfinite("siren_1");
		}
	}

}
void Ghosts::die()
{
	state = EATEN;
	if (!ghost_eat)
		Sounds::instance()->play("eatghost");
	ghost_eat = true;
}

//funzione per costruire il cammino a partire dalla matrice delle distanze
std::list<int> Ghosts::cammino(int x, int y)
{

	int u, v;

	cammini[x][y].push_front(y);
	v = y;
	//std::cout << "Inizio Cammino \n ";
	//se lo start è diverso dall'end, cioè ho un cammino
	while (v != x)
	{
		//std::cout << "SONO nel Ciclo \n ";
		for (int u = 0; u < 76; u++)
		{
			if (distanza[x][u] + distanza[v][u] == distanza[x][v]  &&  is_near(u,v))
			{
				//std::cout << "Cambio di v... \n ";
				//u fa parte di un cammino minimo
				cammini[x][y].push_front(u);
				//mi metto in u e trovo il prossimo nodo da includere nel cammino con la prossima iterazione
				v = u;
				break;
			}
			
		}

	}
	cammini[x][y].pop_front();

	//qDebug() <<" "<<r<<" "<<c<<" "<< cammini[r][c];
 return cammini[x][y];
}