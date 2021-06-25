#include "Include.h"
Pacman::Pacman(QPoint position) : Entity()
{
	// set flags
	moving_speed = 1;
	vicini[24][3] = -1;				//e' il nodo che collega il percorso alla box dei fantasmi
	current_node = 69;				//Nodo da cui parto
	dest_node = 49;					//Nodo al quale mi dirigo
	isDying = false;
	

	// get delle texture di movimento
	// right
	texture_walking[0][0] = Sprites::instance()->get("pacman-dx-0");
	texture_walking[0][1] = Sprites::instance()->get("pacman-dx-1");
	texture_walking[0][2] = Sprites::instance()->get("pacman-dx-2");
	texture_walking[0][3] = Sprites::instance()->get("pacman-dx-3");
	// left
	texture_walking[1][0] = Sprites::instance()->get("pacman-sx-0");
	texture_walking[1][1] = Sprites::instance()->get("pacman-sx-1");
	texture_walking[1][2] = Sprites::instance()->get("pacman-sx-2");
	texture_walking[1][3] = Sprites::instance()->get("pacman-sx-3");
	// down
	texture_walking[2][0] = Sprites::instance()->get("pacman-down-0");
	texture_walking[2][1] = Sprites::instance()->get("pacman-down-1");
	texture_walking[2][2] = Sprites::instance()->get("pacman-down-2");
	texture_walking[2][3] = Sprites::instance()->get("pacman-down-3");
	// up
	texture_walking[3][0] = Sprites::instance()->get("pacman-up-0");
	texture_walking[3][1] = Sprites::instance()->get("pacman-up-1");
	texture_walking[3][2] = Sprites::instance()->get("pacman-up-2");
	texture_walking[3][3] = Sprites::instance()->get("pacman-up-3");
	
	// Pacman death
	texture_die[0]		  = Sprites::instance()->get("pacman-dead-0");
	texture_die[1]		  = Sprites::instance()->get("pacman-dead-1");
	texture_die[2]		  = Sprites::instance()->get("pacman-dead-2");
	texture_die[3]		  = Sprites::instance()->get("pacman-dead-3");
	texture_die[4]		  = Sprites::instance()->get("pacman-dead-4");
	texture_die[5]		  = Sprites::instance()->get("pacman-dead-5");
	texture_die[6]		  = Sprites::instance()->get("pacman-dead-6");
	texture_die[7]		  = Sprites::instance()->get("pacman-dead-7");
	texture_die[8]		  = Sprites::instance()->get("pacman-dead-8");
	texture_die[9]		  = Sprites::instance()->get("pacman-dead-9");
	texture_die[10]		  = Sprites::instance()->get("pacman-dead-10");
	texture_die[11]		  = Sprites::instance()->get("pacman-dead-11");
	texture_die[12]		  = Sprites::instance()->get("pacman-dead-12");

	//Inserisco la texture iniziale di pacman
	setPixmap(texture_walking[1][0]);

	//Posizione di partenza di pacman (nodo 70)
	setPos(105, 201);        
	
	// set della trasparenza della sprite di pacman quando si muove
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			texture_walking[i][j].setMask(texture_walking[i][j].createMaskFromColor(QColor(0, 0, 0)));

	// set trasparenza texture morte
	for (int j = 0; j < 12; j++)
		texture_die[j].setMask(texture_die[j].createMaskFromColor(QColor(0, 0, 0)));

	setZValue(3);
}
void Pacman::advance()
{
	//Quando la musica iniziale e' terminata inizio a muovermi
	if (Sounds::instance()->isFinished("beginning"))
		isMoving= true;

	//Se mi muovo
	if (isMoving)
	{
		//teletrasporto di pacman da sx a dx
		if (x() == -13 && y() == 129 && dir == LEFT)
		{
			//se esco dal bordo sinistro
			setX(224);
			setY(129);
			//setto la posizione del bordo destro
		}
		//teletrasporto di pacman da dx a sx
		if (x() == 224 && y() == 129 && dir == RIGHT)
		{
			//se esco dal bordo destro
			setX(-13);
			setY(129);
			//setto la posizione del bordo sinistro
		}

		//right
		if (dir == RIGHT && can_right == true)
		{
			last_dir = RIGHT; //memorizzo la direzione corrente
			if (vicini[current_node][0] != -1) //se il vicino non è un muro
			{
				//se il nodo destinazione era il sinistro rispetto all'ultimo nodo visitato
				//allora ho fatto un cambio di direzione prima di arrivare a destinazione
				//vuol dire che la mia nuova destinazione è il nodo in cui stavo
				if (dest_node == vicini[current_node][1])
					current_node = dest_node;

				//in ogni caso, la destinazione è il vicino destro rispetto a dove sto
				dest_node = vicini[current_node][0];

				//se la coordinata x coincide
				if (x() == nodo[dest_node].x - 8)
				{
					//sono arrivato a destinazione
					current_node = dest_node;
					//abilito il movimento nelle direzioni ortogonali
					can_up = true;
					can_down = true;

					//se prima di arrivare a dest ho premuto un'altra direzione
					if (next_dir != UNKNOWN)
					{
						//setto l'ultima direzione premuta
						dir = next_dir;
						//e la prossima ovviamente sarà sconosciuta
						next_dir = UNKNOWN;
					}
				}
				else
				{
					//se non sono arrivato mi muovo in base alla velocità
					isMoving = true;
					setX(x() + moving_speed);
					can_up = false;
					can_down = false;
				}
			}
			else
			{
				//per il cambio di direzione prima di arrivare al dest_node
				if (dest_node == vicini[current_node][1])
					current_node = dest_node;
				else
					 isMoving = false; //se incontro un muro devo fermarmi
			}
		}
		//anche se premo su, ma la mia ultima dir è destra (e quindi i movimenti ortogonali sono bloccati)
		if (dir == UP && last_dir == RIGHT && can_up == false && can_down == false)
		{
			//se una volta al dest potrò andare su, allora memorizzo la direzione
			if (vicini[dest_node][2] != -1)
				next_dir = dir;
			//e se non premo nulla, rimane destra
			dir = RIGHT;
		}
		//anche se premo giu, ma la mia ultima dir è destra (e quindi i movimenti ortogonali sono bloccati)
		if (dir == DOWN && last_dir == RIGHT && can_up == false && can_down == false)
		{
			//se una volta al dest potrò andare giu, allora memorizzo la direzione
			if (vicini[dest_node][3] != -1)
				next_dir = dir;
			//e se non premo nulla, rimane destra
			dir = RIGHT;
		}

		//left
		if (dir == LEFT && can_left == true)
		{
			last_dir = LEFT;
			if (vicini[current_node][1] != -1)
			{
				if (dest_node == vicini[current_node][0])
					current_node = dest_node;

				dest_node = vicini[current_node][1];

				if (x() == nodo[dest_node].x - 8)
				{
					// isMoving = false;
					current_node = dest_node;
					can_up = true;
					can_down = true;

					if (next_dir != UNKNOWN)
					{
						dir = next_dir;
						next_dir = UNKNOWN;
					}
				}
				else
				{
					 isMoving = true;
					setX(x() - moving_speed);
					can_up = false;
					can_down = false;
				}
			}
			else
			{
				if (dest_node == vicini[current_node][0])
					current_node = dest_node;
				else
					 isMoving = false;
			}
		}
		if (dir == UP && last_dir == LEFT && can_up == false && can_down == false)
		{
			if (vicini[dest_node][2] != -1)
				next_dir = dir;
			dir = LEFT;
		}
		if (dir == DOWN && last_dir == LEFT && can_up == false && can_down == false)
		{
			if (vicini[dest_node][3] != -1)
				next_dir = dir;
			dir = LEFT;
		}

		//up
		if (dir == UP && can_up == true)
		{
			last_dir = UP;
			if (vicini[current_node][2] != -1)
			{
				if (dest_node == vicini[current_node][3])
					current_node = dest_node;

				dest_node = vicini[current_node][2];

				if (y() == nodo[dest_node].y - 8)
				{
					// isMoving = false;
					current_node = dest_node;
					can_right = true;
					can_left = true;

					if (next_dir != UNKNOWN)
					{
						dir = next_dir;
						next_dir = UNKNOWN;
					}
				}
				else
				{
					 isMoving = true;
					setY(y() - moving_speed);
					can_right = false;
					can_left = false;
				}
			}
			else
			{
				if (dest_node == vicini[current_node][3])
					current_node = dest_node;
				else
					 isMoving = false;
			}
		}
		if (dir == RIGHT && last_dir == UP && can_right == false && can_left == false)
		{
			if (vicini[dest_node][0] != -1)
				next_dir = dir;
			dir = UP;
		}
		if (dir == LEFT && last_dir == UP && can_right == false && can_left == false)
		{
			if (vicini[dest_node][1] != -1)
				next_dir = dir;
			dir = UP;
		}

		//down
		if (dir == DOWN && can_down == true)
		{
			last_dir = DOWN;
			if (vicini[current_node][3] != -1)
			{
				if (dest_node == vicini[current_node][2])
					current_node = dest_node;

				dest_node = vicini[current_node][3];

				if (y() == nodo[dest_node].y - 8)
				{
					// isMoving = false;
					current_node = dest_node;
					can_right = true;
					can_left = true;

					if (next_dir != UNKNOWN)
					{
						dir = next_dir;
						next_dir = UNKNOWN;
					}
				}
				else
				{
					 isMoving = true;
					setY(y() + moving_speed);
					can_right = false;
					can_left = false;
				}
			}
			else
			{
				if (dest_node == vicini[current_node][2])
					current_node = dest_node;
				else
					 isMoving = false;
			}
		}
		if (dir == RIGHT && last_dir == DOWN && can_right == false && can_left == false)
		{
			if (vicini[dest_node][0] != -1)
				next_dir = dir;
			dir = DOWN;
		}
		if (dir == LEFT && last_dir == DOWN && can_right == false && can_left == false)
		{
			if (vicini[dest_node][1] != -1)
				next_dir = dir;
			dir = DOWN;
		}
	}
}

void Pacman::die()
{
	isDying = true;
	
	//Pacman non si muove piu
	moving_speed = 0;
}

void Pacman::animate()
{
	Entity::animate();
	//Se mi sto muovendo
	if (isMoving)
	{
		if (dir == RIGHT )
		{
			setPixmap(texture_walking[0][0]);	                                           //Cambia texture e inserisce quella di pacman verso dx
			setPixmap(texture_walking[0][(animation_counter / 5) % 4]);				      //Animazione camminata dx
		}
		else if (dir == LEFT )
		{
			setPixmap(texture_walking[1][0]);	                                           //Cambia texture e inserisce quella di pacman verso sx
			setPixmap(texture_walking[1][(animation_counter / 5) % 4]);				      //Animazione camminata sx
		}
		else if (dir == DOWN )
		{
			setPixmap(texture_walking[2][0]);		                                        //Cambia texture e inserisce quella di pacman verso sotto
			setPixmap(texture_walking[2][(animation_counter / 5) % 4]);					    //Animazione camminata sotto
		}
		else if (dir == UP )
		{
			setPixmap(texture_walking[3][0]);	                                           //Cambia texture e inserisce quella di pacman verso sopra
			setPixmap(texture_walking[3][(animation_counter / 5) % 4]);				       //Animazione camminata sopra
		}

	}
	//Se non mi muovo
	else
	{
		if (dir == RIGHT)
		{
			setPixmap(texture_walking[0][0]);                                           //Cambia texture e inserisce quella di pacman verso dx
		}
		else if (dir == LEFT )
		{
			setPixmap(texture_walking[1][0]);                                           //Cambia texture e inserisce quella di pacman verso sx
		}
		else if (dir == DOWN)
		{
			setPixmap(texture_walking[2][0]);											//Cambia texture e inserisce quella di pacman verso sotto
		}
		else if (dir == UP)
		{
			setPixmap(texture_walking[3][0]);                                           //Cambia texture e inserisce quella di pacman verso sopra
		}
	}

	if (isDying == true && death_counter < 72) // 72 sono i frame che pacman impiega per morire
	{
		//Appena inizia la morte attivo la musica
		if (death_counter == 1)
			Sounds::instance()->play("death");

		setMoving(false);
		setPixmap(texture_die[(death_counter / 6) % 12]);
	
		if (death_counter == 71)    //Quando Finice il ciclo e' morto definitamente
		{
			death = true;
		}
	}
}

bool Pacman::canGoLeft()
{
	bool flag = true;
	//Controllo se puo andare a sx 
	//Il -8 e' la distanza tra il pixel della posizione del nodo e del punto in alto a sx della texture di pacman
	if ((x() == (nodo[current_node].x - 8)) && (y() == nodo[current_node ].y - 8))
		if (vicini[current_node][1] == -1)
			flag = false;
	return flag;

	return false;
}
bool Pacman::canGoDown()
{
	bool flag = true;
	//Controllo se puo andare a giu' 
	//Il -8 e' la distanza tra il pixel della posizione del nodo e del punto in alto a sx della texture di pacman
	if ((x() == (nodo[current_node ].x - 8)) && (y() == nodo[current_node ].y - 8))
		if (vicini[current_node ][3] == -1)
			flag = false;
	return flag;

	return false;
}
bool Pacman::canGoUp()
{
	bool flag = true;
	//Controllo se puo andare a sopra
	//Il -8 e' la distanza tra il pixel della posizione del nodo e del punto in alto a sx della texture di pacman
	if ((x() == (nodo[current_node ].x - 8)) && (y() == nodo[current_node ].y - 8))
		if (vicini[current_node ][2] == -1)
			flag = false;
	return flag;

	return false;
}
bool Pacman::canGoRight()
{
	bool flag = true;
	//Controllo se puo andare a dx 
	//Il -8 e' la distanza tra il pixel della posizione del nodo e del punto in alto a sx della texture di pacman
	if ((x() == (nodo[current_node - 1].x - 8)) && (y() == nodo[current_node - 1].y - 8))
		if (vicini[current_node - 1][0] == -1)
			flag = false;
	return flag;

	return false;
}