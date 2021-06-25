#include <QApplication>
#include <QThread>
#include <QIcon>

#include "Include.h"

Game* Game::uniqueInstance = 0;
Game* Game::instance()
{
	if (uniqueInstance == 0)
		uniqueInstance = new Game();

	return uniqueInstance;
}
Game::Game(QGraphicsView* parent) : QGraphicsView(parent)
{
	//Inserisco Titolo ed Icona di gioco
	this->setWindowTitle("Impossible Pacman");
	this->setWindowIcon(QIcon(":/Sprites/icona.png"));

	//Imposto grandezza finestra
	this->setFixedHeight(2 * 288);
	this->setFixedWidth(2 * 224);

	//Imposto Scena e View
	scene = new QGraphicsScene();
	setScene(scene);
	scene->setSceneRect(0, 0, 224, 288);
	scale(2.0, 2.0);
	centerOn(0, 0);

	setInteractive(false);													// Disabilita Eventi
	setHorizontalScrollBarPolicy	(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy		(Qt::ScrollBarAlwaysOff);

	//Imposto motore di gioco 
	QObject::connect(&engine, SIGNAL(timeout()), this, SLOT(advance()));
	engine.setInterval(18);													//Aggiorna l'advance ogni 18 millisecondi											

	//Inizializzo Pacman
	pacman = 0;
	//Inizializzo Testi di gioco
	ready	 = new QGraphicsPixmapItem();
	lv		 = new QGraphicsPixmapItem();
	sc		 = new QGraphicsPixmapItem();

	//Inizializzo altre variabili
	second = 0;
	msecond = 0;
	contaVite = 3;
	fruttoMangiato = false;

	for (int i = 0; i < 245; i++)
		ciboRimanente[i] = false;

	//linea per inserire testo quando finisci
	lineone				= new QLineEdit(this);
	//bottoni
	ok					= new QPushButton(this);
	buttongoback		= new QPushButton(this);
	buttonRank		    = new QPushButton(this);
	buttonback			= new QPushButton(this);
	buttonStart			= new QPushButton(this);
	buttonai			= new QPushButton(this);
	buttonnormal		= new QPushButton(this);

	//Nascondo bottoni
	lineone->hide();
	ok->hide();
	buttongoback->hide();

	//connessione bottoni
	QObject::connect(buttongoback, SIGNAL(clicked()), this, SLOT(reset()));
	QObject::connect(buttonRank, SIGNAL(clicked()), this, SLOT(ranking()));
	QObject::connect(buttonback, SIGNAL(clicked()), this, SLOT(reset()));
	QObject::connect(ok, SIGNAL(clicked()), this, SLOT(ok_clicked()));
	QObject::connect(buttonStart, SIGNAL(clicked()), this, SLOT(start()));
	QObject::connect(buttonnormal, SIGNAL(clicked()), this, SLOT(modNormal()));
	QObject::connect(buttonai, SIGNAL(clicked()), this, SLOT(modImpossible()));


	targetActive = false;
	impossible	 = false;
	targetAi = false;
	
	//Dopo aver inizializzato le varie variabili di Game chiamo la funzione reset()
	reset();
}
void Game::advance()
{	
	// do nothing if game is not running
	if (cur_state != RUNNING)
		return;

	//Ricavo una unita temporale gestita in secondi
	msecond++;
	if (msecond >= second * 55) //circa un secondo
		second++;

	if (Sounds::instance()->isFinished("beginning"))
		ready->setZValue(0);
	
	//Pacman death
	if (pacman->isDeath())
		gameover();
	
	//YouWin
	if ( (contaVite != 0) && (punteggio+punt_big_food >= 245) )
		gameover();

	//Sirena
	if (getSecond() == 4)
	{
		Sounds::instance()->play("siren_1");
		Sounds::instance()->setLoops("siren_1",QSound::Infinite);
	}

	//Punteggio

	//incremento vita se maggiore di 10000
	check_score = (punteggio * 10) + (punt_big_food * 100) + punteggio_ghost + cont_fruit_eat;
	if (check_score >= 10000 && inc_life)
	{
		life[2]->setPos(life[contaVite - 1]->x(), life[contaVite - 1]->y());
		life[2]->setOpacity(1);
		contaVite++;
		inc_life = false;
	}

	if (pacman->is_Moving() == true)
	{
		//score
		for (int i = 0; i < 245; i++)
		{
			eat = confronto(food[i], pacman);
			if (eat == true && ciboRimanente[i] == false)
			{
				ciboRimanente[i] = true;
				//Aggiorna lo score 
				score->setOpacity(0);
				if (food[i]->getBig() == false)
					punteggio++;
				else if (food[i]->getBig() == true)
				{
					punt_big_food++;
					fantasmiMangiati = 0;
				}

				score->setOpacity(0);
				score = scene->addText(QString::number((punteggio * 10) + (punt_big_food * 100) + punteggio_ghost + cont_fruit_eat));
				score->setPos(48, 265);                                       //setta la posizione
				score->setDefaultTextColor(QColor(255, 255, 51));             //set del colore
				score->setZValue(3);
				score->setOpacity(100);
			}
		}
		if (fruit->get_Fruit_iseat() == true && !fruttoMangiato)
		{
			cont_fruit_eat = cont_fruit_eat + 100;
			

			score->setOpacity(0);
			score = scene->addText(QString::number((punteggio * 10) + (punt_big_food * 100) + punteggio_ghost + cont_fruit_eat));
			score->setPos(48, 265);                                       //setta la posizione
			score->setDefaultTextColor(QColor(255, 255, 51));             //set del colore
			score->setZValue(3);
			score->setOpacity(100);
			fruttoMangiato = true;

			QTimer::singleShot(1000, this, SLOT(hide_fruit()));
		}
		
	}

	//Quando mangio fantasmi durante powerball
	{
		QFont myFont("Arial", 7, QFont::Bold);
		if (blinky->getFantasmaMangiato())
		{
			blinky->setFantasmaMangiato(false);
			fantasmiMangiati++;
			increase = pow(2, fantasmiMangiati) * 100;
			punteggio_ghost += increase;

			point_ghostEatb = scene->addText(QString::number(increase), myFont);
			point_ghostEatb->setPos(blinky->x(), blinky->y());
			point_ghostEatb->setDefaultTextColor(QColor(255, 255, 255));
			point_ghostEatb->setZValue(4);
			point_ghostEatb->setOpacity(1);

			score->setOpacity(0);
			score = scene->addText(QString::number((punteggio * 10) + (punt_big_food * 100) + punteggio_ghost + cont_fruit_eat));
			score->setPos(48, 265);                                       //setta la posizione
			score->setDefaultTextColor(QColor(255, 255, 51));             //set del colore
			score->setZValue(3);
			score->setOpacity(100);

			QTimer::singleShot(1000, this, SLOT( hide_score_b() ));
		}
		if (clyde->getFantasmaMangiato())
		{
			clyde->setFantasmaMangiato(false);
			fantasmiMangiati++;
			increase = pow(2, fantasmiMangiati) * 100;
			punteggio_ghost += increase;

			// std::cout<<"Mangiato fantasma"<<std::endl;
			// std::cout<<"Num: "<<fantasmiMangiati<<std::endl;
			// std::cout<<"Increase: "<<increase<<std::endl;

			point_ghostEatc = scene->addText(QString::number(increase), myFont);
			point_ghostEatc->setPos(clyde->x(), clyde->y());
			point_ghostEatc->setDefaultTextColor(QColor(255, 255, 255));
			point_ghostEatc->setZValue(4);
			point_ghostEatc->setOpacity(1);

			score->setOpacity(0);
			score = scene->addText(QString::number((punteggio * 10) + (punt_big_food * 100) + punteggio_ghost + cont_fruit_eat));
			score->setPos(48, 265);                                       //setta la posizione
			score->setDefaultTextColor(QColor(255, 255, 51));             //set del colore
			score->setZValue(3);
			score->setOpacity(100);

			QTimer::singleShot(1000, this, SLOT( hide_score_c() ));

		}
		if (pinky->getFantasmaMangiato())
		{
			pinky->setFantasmaMangiato(false);
			fantasmiMangiati++;
			increase = pow(2, fantasmiMangiati) * 100;
			punteggio_ghost += increase;

			// std::cout<<"Mangiato fantasma"<<std::endl;
			// std::cout<<"Num: "<<fantasmiMangiati<<std::endl;
			// std::cout<<"Increase: "<<increase<<std::endl;

			point_ghostEatp = scene->addText(QString::number(increase), myFont);
			point_ghostEatp->setPos(pinky->x(), pinky->y());
			point_ghostEatp->setDefaultTextColor(QColor(255, 255, 255));
			point_ghostEatp->setZValue(4);
			point_ghostEatp->setOpacity(1);

			score->setOpacity(0);
			score = scene->addText(QString::number((punteggio * 10) + (punt_big_food * 100) + punteggio_ghost + cont_fruit_eat));
			score->setPos(48, 265);                                       //setta la posizione
			score->setDefaultTextColor(QColor(255, 255, 51));             //set del colore
			score->setZValue(3);
			score->setOpacity(100);

			QTimer::singleShot(1000, this, SLOT( hide_score_p() ));

		}
		if (inky->getFantasmaMangiato())
		{
			inky->setFantasmaMangiato(false);
			fantasmiMangiati++;
			increase = pow(2, fantasmiMangiati) * 100;
			punteggio_ghost += increase;

			// std::cout<<"Mangiato fantasma"<<std::endl;
			// std::cout<<"Num: "<<fantasmiMangiati<<std::endl;
			// std::cout<<"Increase: "<<increase<<std::endl;

			point_ghostEati = scene->addText(QString::number(increase), myFont);
			point_ghostEati->setPos(inky->x(), inky->y());
			point_ghostEati->setDefaultTextColor(QColor(255, 255, 255));
			point_ghostEati->setZValue(4);
			point_ghostEati->setOpacity(1);

			score->setOpacity(0);
			score = scene->addText(QString::number((punteggio * 10) + (punt_big_food * 100) + punteggio_ghost + cont_fruit_eat));
			score->setPos(48, 265);                                       //setta la posizione
			score->setDefaultTextColor(QColor(255, 255, 51));             //set del colore
			score->setZValue(3);
			score->setOpacity(100);

			QTimer::singleShot(1000, this, SLOT( hide_score_i() ));

		}
	}

	//Target
	if (targetActive)
	{
		lineBlinky->setLine(blinky->getTargetx() + 7, blinky->getTargety() + 7, blinky->x() + 7, blinky->y() + 7);
		linePinky->setLine(pinky->getTargetx() + 7, pinky->getTargety() + 7, pinky->x() + 7, pinky->y() + 7);
		lineInky->setLine(inky->getTargetx() + 7, inky->getTargety() + 7, inky->x() + 7, inky->y() + 7);
		lineClyde->setLine(clyde->getTargetx() + 7, clyde->getTargety() + 7, clyde->x() + 7, clyde->y() + 7);

	

	}
	if (targetAi)
	{
		assegnaB();
		assegnaC();
		linePinky->setLine(pinky->getTargetx() + 7, pinky->getTargety() + 7, pinky->x() + 7, pinky->y() + 7);
		lineInky->setLine(inky->getTargetx() + 7, inky->getTargety() + 7, inky->x() + 7, inky->y() + 7);
	}
		
	
	// tell all game objects to animate and advance in the scene
	for (auto& item : scene->items())
	{
		Object* obj = dynamic_cast<Object*>(item);
		if (obj)
		{
			obj->animate();
			obj->advance();

			// destroy died Entity objects, except Pacman
			// Entity* entity_obj = dynamic_cast<Entity*>(obj);
			// Pacman* pacman_obj = dynamic_cast<Pacman*>(obj);
			
		}
	}

	//Nascondo i residui dei bottoni del reset() 
	buttonRank->hide();
	buttonStart->hide();
}

void Game::reset()
{	
	//Imposto Stato di gioco
	cur_state = READY;

	//Imposto scena iniziale
	scene->clear();
	setScene(scene);
	scene->addPixmap(Sprites::instance()->get("title-screen"));

	//Fermo motore di gioco
	engine.stop();

	second = 0;
	msecond = 0;
	targetActive = false;
	
	//Variabile per conteggio vite
	contaVite=3;

	//Variabili per Score
	punteggio = 0;
	punt_big_food = 0;
	cont_fruit_eat = 0;
	punteggio_ghost = 0;
	increase = 0;
	fantasmiMangiati = 0;
	
	for (int i = 0; i < 245; i++)
		ciboRimanente[i] = false;

	//nascondo bottoni
	ok->hide();
	buttongoback->hide();
	lineone->hide();
	buttonback->hide();
	buttonai->hide();
	buttonnormal->hide();
	
	//Visualizzo bottoni play e ranking
	buttonPlay();
	buttonRanking();
	
	//Stop di tutte le musiche
	Sounds::instance()->stop("beginning");
	Sounds::instance()->stop("hunt");
	Sounds::instance()->stop("eyes");
	Sounds::instance()->stop("death");
	Sounds::instance()->stop("siren_1");
}

void Game::start()
{
	scene->clear();
	scene->setBackgroundBrush(QColor(Qt::black));

	//Mostro bottoni modalità
	buttonAI();
	buttonNormal();

	engine.start();

	//Carico Oggetti di gioco dal Level Manager

	//Box
	box		 = new Boxx(QPoint(0, 0));

	//Pacman
	pacman	 = new Pacman(QPoint(105, 201));

	//Ghosts
	blinky	 = new Blinky(QPoint(120, 120));
	pinky	 = new Pinky(QPoint(104, 143));
	clyde	 = new Clyde(QPoint(120, 143));
	inky	 = new Inky(QPoint(136, 143));

	//Food
	{
		int i = 0;
		for (int j = 13; j < (13 + (8 * 12)); j += 8)

			food[i++] = new Food(QPoint(j, 33), false);

		for (int j = 125; j < (125 + (8 * 12)); j += 8)
			food[i++] = new Food(QPoint(j, 33), false);

		food[i++] = new Food(QPoint(13, 33 + (8 * 1)), false);
		food[i++] = new Food(QPoint(13 + (8 * 5), 33 + (8 * 1)), false);
		food[i++] = new Food(QPoint(13 + (8 * 11), 33 + (8 * 1)), false);

		food[i++] = new Food(QPoint(125, 33 + (8 * 1)), false);
		food[i++] = new Food(QPoint(125 + (8 * 6), 33 + (8 * 1)), false);
		food[i++] = new Food(QPoint(125 + (8 * 11), 33 + (8 * 1)), false);

		food[i++] = new Food(QPoint(16, 52), true);
		food[i++] = new Food(QPoint(13 + (8 * 5), 33 + (8 * 2)), false);
		food[i++] = new Food(QPoint(13 + (8 * 11), 33 + (8 * 2)), false);

		food[i++] = new Food(QPoint(125, 33 + (8 * 2)), false);
		food[i++] = new Food(QPoint(125 + (8 * 6), 33 + (8 * 2)), false);
		food[i++] = new Food(QPoint(128 + (8 * 11), 52), true);

		food[i++] = new Food(QPoint(13, 33 + (8 * 3)), false);
		food[i++] = new Food(QPoint(13 + (8 * 11), 33 + (8 * 3)), false);
		food[i++] = new Food(QPoint(13 + (8 * 5), 33 + (8 * 3)), false);

		food[i++] = new Food(QPoint(125, 33 + (8 * 3)), false);
		food[i++] = new Food(QPoint(125 + (8 * 11), 33 + (8 * 3)), false);
		food[i++] = new Food(QPoint(125 + (8 * 6), 33 + (8 * 3)), false);

		for (int j = 13; j < (13 + (8 * 26)); j += 8)
			food[i++] = new Food(QPoint(j, 33 + (8 * 4)), false);

		food[i++] = new Food(QPoint(13, 33 + (8 * 5)), false);
		food[i++] = new Food(QPoint(13 + (8 * 8), 33 + (8 * 5)), false);
		food[i++] = new Food(QPoint(13 + (8 * 5), 33 + (8 * 5)), false);

		food[i++] = new Food(QPoint(125 + (8 * 3), 33 + (8 * 5)), false);
		food[i++] = new Food(QPoint(125 + (8 * 11), 33 + (8 * 5)), false);
		food[i++] = new Food(QPoint(125 + (8 * 6), 33 + (8 * 5)), false);

		food[i++] = new Food(QPoint(13, 33 + (8 * 6)), false);
		food[i++] = new Food(QPoint(13 + (8 * 8), 33 + (8 * 6)), false);
		food[i++] = new Food(QPoint(13 + (8 * 5), 33 + (8 * 6)), false);

		food[i++] = new Food(QPoint(125 + (8 * 3), 33 + (8 * 6)), false);
		food[i++] = new Food(QPoint(125 + (8 * 11), 33 + (8 * 6)), false);
		food[i++] = new Food(QPoint(125 + (8 * 6), 33 + (8 * 6)), false);

		for (int j = 13; j < (13 + (8 * 6)); j += 8)
			food[i++] = new Food(QPoint(j, 33 + (8 * 7)), false);

		for (int j = 125; j < (125 + (8 * 4)); j += 8)
			food[i++] = new Food(QPoint(j, 33 + (8 * 7)), false);

		for (int j = 13 + (8 * 8); j < (77 + (8 * 4)); j += 8)
			food[i++] = new Food(QPoint(j, 33 + (8 * 7)), false);
		//Ricontrolla questa riga va dal 93
		for (int j = 125 + (8 * 6); j < (125 + 77 + (8 * 2)); j += 8)
			food[i++] = new Food(QPoint(j, 33 + (8 * 7)), false);

		//vanno dal 100 al 111 messi manualmente perche erano sbagliati
		food[i++] = new Food(QPoint(53, 97), false);
		food[i++] = new Food(QPoint(53, 105), false);
		food[i++] = new Food(QPoint(53, 113), false);
		food[i++] = new Food(QPoint(53, 121), false);
		food[i++] = new Food(QPoint(53, 129), false);
		food[i++] = new Food(QPoint(53, 137), false);
		food[i++] = new Food(QPoint(53, 145), false);
		food[i++] = new Food(QPoint(53, 153), false);
		food[i++] = new Food(QPoint(53, 161), false);
		food[i++] = new Food(QPoint(53, 169), false);
		food[i++] = new Food(QPoint(53, 177), false);
		food[i++] = new Food(QPoint(53, 185), false);

		food[i++] = new Food(QPoint(173, 97), false);
		food[i++] = new Food(QPoint(173, 105), false);
		food[i++] = new Food(QPoint(173, 113), false);
		food[i++] = new Food(QPoint(173, 121), false);
		food[i++] = new Food(QPoint(173, 129), false);
		food[i++] = new Food(QPoint(173, 137), false);
		food[i++] = new Food(QPoint(173, 145), false);
		food[i++] = new Food(QPoint(173, 153), false);
		food[i++] = new Food(QPoint(173, 161), false);
		food[i++] = new Food(QPoint(173, 169), false);
		food[i++] = new Food(QPoint(173, 177), false);

		for (int j = 13; j < (13 + (8 * 12)); j += 8)
			food[i++] = new Food(QPoint(j, 33 + (8 * 19)), false);

		for (int j = 125; j < (125 + (8 * 12)); j += 8)
			food[i++] = new Food(QPoint(j, 33 + (8 * 19)), false);
		//fino a qui 146 tutti corretti

		food[i++] = new Food(QPoint(13, 33 + (8 * 20)), false);
		food[i++] = new Food(QPoint(13 + (8 * 5), 33 + (8 * 20)), false);
		food[i++] = new Food(QPoint(13 + (8 * 11), 33 + (8 * 20)), false);

		food[i++] = new Food(QPoint(125, 33 + (8 * 20)), false);
		food[i++] = new Food(QPoint(125 + (8 * 6), 33 + (8 * 20)), false);
		food[i++] = new Food(QPoint(125 + (8 * 11), 33 + (8 * 20)), false);

		food[i++] = new Food(QPoint(13, 33 + (8 * 21)), false);
		food[i++] = new Food(QPoint(13 + (8 * 5), 33 + (8 * 21)), false);
		food[i++] = new Food(QPoint(13 + (8 * 11), 33 + (8 * 21)), false);

		food[i++] = new Food(QPoint(125, 33 + (8 * 21)), false);
		food[i++] = new Food(QPoint(125 + (8 * 6), 33 + (8 * 21)), false);
		food[i++] = new Food(QPoint(125 + (8 * 11), 33 + (8 * 21)), false);

		food[i++] = new Food(QPoint(16, 52 + (8 * 20)), true);
		food[i++] = new Food(QPoint(13 + (8 * 1), 33 + (8 * 22)), false);
		food[i++] = new Food(QPoint(13 + (8 * 2), 33 + (8 * 22)), false);

		food[i++] = new Food(QPoint(128 + (8 * 11), 52 + (8 * 20)), true);
		food[i++] = new Food(QPoint(125 + (8 * 9), 33 + (8 * 22)), false);
		food[i++] = new Food(QPoint(125 + (8 * 10), 33 + (8 * 22)), false);

		for (int j = 13 + (8 * 5); j < (13 + (4 * 21)); j += 8)
			food[i++] = new Food(QPoint(j, 33 + (8 * 22)), false);

		food[i++] = new Food(QPoint(101, 209), false);
		food[i++] = new Food(QPoint(125, 209), false);
		food[i++] = new Food(QPoint(133, 209), false);
		food[i++] = new Food(QPoint(141, 209), false);
		food[i++] = new Food(QPoint(149, 209), false);
		food[i++] = new Food(QPoint(157, 209), false);
		food[i++] = new Food(QPoint(165, 209), false);
		food[i++] = new Food(QPoint(173, 209), false);
		//fino qui 178
		food[i++] = new Food(QPoint(13 + (8 * 2), 33 + (8 * 23)), false);
		food[i++] = new Food(QPoint(13 + (8 * 5), 33 + (8 * 23)), false);
		food[i++] = new Food(QPoint(13 + (8 * 8), 33 + (8 * 23)), false);

		food[i++] = new Food(QPoint(125 + (8 * 3), 33 + (8 * 23)), false);
		food[i++] = new Food(QPoint(125 + (8 * 6), 33 + (8 * 23)), false);
		food[i++] = new Food(QPoint(125 + (8 * 9), 33 + (8 * 23)), false);

		food[i++] = new Food(QPoint(13 + (8 * 2), 33 + (8 * 24)), false);
		food[i++] = new Food(QPoint(13 + (8 * 5), 33 + (8 * 24)), false);
		food[i++] = new Food(QPoint(13 + (8 * 8), 33 + (8 * 24)), false);

		food[i++] = new Food(QPoint(125 + (8 * 3), 33 + (8 * 24)), false);
		food[i++] = new Food(QPoint(125 + (8 * 6), 33 + (8 * 24)), false);
		food[i++] = new Food(QPoint(125 + (8 * 9), 33 + (8 * 24)), false);

		for (int j = 13; j < (13 + (8 * 6)); j += 8)
			food[i++] = new Food(QPoint(j, 33 + (8 * 25)), false);

		for (int j = 125; j < (125 + (8 * 4)); j += 8)
			food[i++] = new Food(QPoint(j, 33 + (8 * 25)), false);

		for (int j = 13 + (8 * 8); j < (77 + (8 * 4)); j += 8)
			food[i++] = new Food(QPoint(j, 33 + (8 * 25)), false);

		for (int j = 125 + (8 * 6); j < (125 + 77 + (8 * 2)); j += 8)
			food[i++] = new Food(QPoint(j, 33 + (8 * 25)), false);
		//210
		food[i++] = new Food(QPoint(13, 33 + (8 * 26)), false);
		food[i++] = new Food(QPoint(13 + (8 * 11), 33 + (8 * 26)), false);

		food[i++] = new Food(QPoint(125, 33 + (8 * 26)), false);
		food[i++] = new Food(QPoint(125 + (8 * 11), 33 + (8 * 26)), false);

		food[i++] = new Food(QPoint(13, 33 + (8 * 27)), false);
		food[i++] = new Food(QPoint(13 + (8 * 11), 33 + (8 * 27)), false);

		food[i++] = new Food(QPoint(125, 33 + (8 * 27)), false);
		food[i++] = new Food(QPoint(125 + (8 * 11), 33 + (8 * 27)), false);

		for (int j = 13; j < (13 + (8 * 26)); j += 8)
			food[i++] = new Food(QPoint(j, 33 + (8 * 28)), false);

	}

	//nasconde il cibo già mangiato
	for (int i = 0; i < 245; i++)
		if(ciboRimanente[i]==true)
			food[i]->setFoodIsEat(true);

	//Fruit
	fruit	 = new Fruit(QPoint(154, 196));
	fruttoMangiato = false;

	//Vite
	life[0]	 = new Life(QPoint(152, 284));
	life[1]	 = new Life(QPoint(167, 284));
	life[2]	 = new Life(QPoint(182, 284));
	life[2]->setOpacity(0); //se raggiungo 10000 la mostro

	//Se perdo ritorna con una vita in meno
	if (contaVite == 2)
		life[1]->setOpacity(0);
	if (contaVite == 1)
	{
		life[1]->setOpacity(0);
		life[0]->setOpacity(0);
	}
	if (contaVite == 0)
	{
		life[2]->setOpacity(0);
		life[1]->setOpacity(0);
		life[0]->setOpacity(0);
	}

	//Inserisce Get Ready 
	ready = scene->addPixmap(QPixmap(":/Sprites/ready1.png"));
	ready->setPos(81, 155);
	ready->setScale(0.50);
	ready->setZValue(1);

	//inserisco LIVES
	lv = scene->addPixmap(QPixmap(":/Sprites/lives.png"));
	lv->setPos(98, 270);
	lv->setZValue(1);
	lv->setScale(0.45);

	//inserisco SCORE
	sc = scene->addPixmap(QPixmap(":/Sprites/score.png"));
	sc->setPos(5, 270);
	sc->setZValue(1);
	sc->setScale(0.45);

	//Inserisco frutto
	QGraphicsPixmapItem* fr = new QGraphicsPixmapItem();
	fr = scene->addPixmap(Sprites::instance()->get("Cherry"));
	fr->setPos(197, 268);
	fr->setScale(0.45);

	Sounds::instance()->play("beginning");

	//azzero tempo
	msecond = 0;
	second = 0;

	//Set dello score all'inzio della partita
	score = scene->addText(QString::number((punteggio * 10) + (punt_big_food * 100) + punteggio_ghost + cont_fruit_eat));
	score->setPos(48, 265);                    //setta la posizione
	score->setDefaultTextColor(QColor(255, 255, 51));       //setta il colore
	score->setZValue(3);

	//Se la modalita e' impossibile
	if (impossible)
	{
		//cambio texture
		blinky->setImpossible(true);
		clyde->setImpossible(true);
		inky->setImpossible(true);
		pinky->setImpossible(true);
		box->setImpossible(true);
		for (int i = 0; i < 245; i++)
			food[i]->setImpossible(true);

		//cambio stato di gioco per fantasmini
		blinky->setAi(true);
		inky->setAi(true);
		clyde->setAi(true);
		pinky->setAi(true);
	}

	//Percorsi AI
	//per clyde
	for (int i = 0; i < 76; i++) //76 perchè al massimo posso disegnare un percorso di 76 nodi
	{
		tragittoC[i] = new QGraphicsRectItem(3, 0, 3, 3);
		tragittoC[i]->setZValue(100);
		tragittoC[i]->setBrush(QBrush(QColor(255,117,20)));
		tragittoC[i]->setVisible(false);
		scene->addItem(tragittoC[i]);
	}
	//per blinky
	for (int i = 0; i < 76; i++)
	{
		tragittoB[i] = new QGraphicsRectItem(0, 0, 3, 3);
		tragittoB[i]->setZValue(100);
		tragittoB[i]->setBrush(QBrush(QColor(Qt::red)));
		tragittoB[i]->setVisible(false);
		scene->addItem(tragittoB[i]);
	}
	targetAi = false;

	//Il gioco e' pronto a partire
	cur_state = RUNNING;	
}

void Game::gameover()
{
	//Imposto stato di gioco
	cur_state = GAME_OVER;
	contaVite--;

	Sounds::instance()->stop("siren_1");

	for (int i = 0; i < 245; i++)
	{
		if (food[i]->getFoodIsEat())
			ciboRimanente[i] = true;
	}
	targetActive = false;
	//Mostro Scritta GameOver
	QGraphicsPixmapItem* go = scene->addPixmap(QPixmap(":/Sprites/gameover.png"));
	go->setPos(78, 155);
	go->setZValue(5);
	go->setScale(0.6);
	go->setOpacity(0);

	QGraphicsPixmapItem* win = scene->addPixmap(QPixmap(":/Sprites/win.png"));
	win->setPos(83, 153);
	win->setZValue(5);
	win->setScale(0.5);
	win->setOpacity(0);

	//Quando ho finito le vite
	if (contaVite == 0)
	{	
		//Faccio vedere scritta gameover
		go->setOpacity(1);
		//nascondo il resto
		pacman->setOpacity(0);
		blinky->setOpacity(0);
		pinky->setOpacity(0);
		clyde->setOpacity(0);
		inky->setOpacity(0);
		fruit->setOpacity(0);

		//Dopo 1 secondo passa alla schermata per inserire nome
		QTimer::singleShot(1000, this, SLOT(giocoFinito()));
	}
	else if ( (contaVite != 0) && (punteggio+punt_big_food >= 245) )
	{
		win->setOpacity(1);
		//nascondo il resto
		pacman->setOpacity(0);
		blinky->setOpacity(0);
		pinky->setOpacity(0);
		clyde->setOpacity(0);
		inky->setOpacity(0);
		fruit->setOpacity(0);

		//Dopo 2 secondi passa alla schermata per inserire nome
		QTimer::singleShot(2000, this, SLOT(giocoFinito()));
	}
	else //Altrimenti sono pronto per ripartire
	{
		cur_state = READY;
		start();
	}

	
}

void Game::keyPressEvent(QKeyEvent* e)
{
	//Serve per non far ripetere l'azione se mantengo premuto un pulsante
	if (e->isAutoRepeat())
		return;
	// start new game
	if (e->key() == Qt::Key_Return && cur_state == READY)
		start();

	// reset game
	if (e->key() == Qt::Key_R)
		reset();

	//GameOver
	if (e->key() == Qt::Key_D)
		gameover();

	//Target
	if (e->key() == Qt::Key_T && cur_state==RUNNING )
	{
		if (!impossible)
		{
			if (!targetActive)
			{
				lineBlinky = new QGraphicsLineItem();
				lineBlinky->setPen(QPen(QBrush(Qt::red), 0.8));
				lineBlinky->setZValue(100);
				scene->addItem(lineBlinky);
				lineBlinky->setVisible(true);

				lineInky = new QGraphicsLineItem();
				lineInky->setPen(QPen(QBrush(QColor(0, 255, 255)), 0.8));
				lineInky->setZValue(100);
				scene->addItem(lineInky);
				lineInky->setVisible(true);

				linePinky = new QGraphicsLineItem();
				linePinky->setPen(QPen(QBrush(QColor(255, 192, 203)), 0.8));
				linePinky->setZValue(100);
				scene->addItem(linePinky);
				linePinky->setVisible(true);

				lineClyde = new QGraphicsLineItem();
				lineClyde->setPen(QPen(QBrush(QColor(255, 117, 20)), 0.8));
				lineClyde->setZValue(100);
				scene->addItem(lineClyde);
				lineClyde->setVisible(true);

				targetActive = true;
			}
			else
			{
				targetActive = false;

				lineBlinky->setVisible(false);
				lineClyde->setVisible(false);
				linePinky->setVisible(false);
				lineInky->setVisible(false);

			}
		}
	
		else if (impossible)
		{
			if (!targetAi)
			{
				assegnaB();
				assegnaC();

				lineInky = new QGraphicsLineItem();
				lineInky->setPen(QPen(QBrush(QColor(0, 255, 255)), 0.8));
				lineInky->setZValue(100);
				scene->addItem(lineInky);
				lineInky->setVisible(true);

				linePinky = new QGraphicsLineItem();
				linePinky->setPen(QPen(QBrush(QColor(255, 192, 203)), 0.8));
				linePinky->setZValue(100);
				scene->addItem(linePinky);
				linePinky->setVisible(true);

				targetAi = true;
			}
			else
			{
				for (int i = 0; i < 76; i++)
				{
					tragittoB[i]->setVisible(false);
					tragittoC[i]->setVisible(false);
				}
				linePinky->setVisible(false);
				lineInky->setVisible(false);
				targetAi = false;
			}
		}
	}

	// pause game
	if (e->key() == Qt::Key_P)
		tooglePause();
		
	//Set Direzioni pacman
	{
		if (e->key() == Qt::Key_Right && pacman->canGoRight() && Sounds::instance()->isFinished("beginning"))
		{
			pacman->setDirection(Direction::RIGHT);
			pacman->setMoving(true);
		
		}
		if (e->key() == Qt::Key_Left && pacman->canGoLeft() && Sounds::instance()->isFinished("beginning"))
		{
			pacman->setDirection(Direction::LEFT);
			pacman->setMoving(true);
			
		}
		if (e->key() == Qt::Key_Up && pacman->canGoUp() && Sounds::instance()->isFinished("beginning"))
		{
			pacman->setDirection(Direction::UP);
			pacman->setMoving(true);
		}
		if (e->key() == Qt::Key_Down && pacman->canGoDown() && Sounds::instance()->isFinished("beginning"))
		{
			pacman->setDirection(Direction::DOWN);
			pacman->setMoving(true);	
		}
	}

}
//ingrandisce e rimpicciolisce la scena
void Game::wheelEvent(QWheelEvent* e)
{
	if (e->delta() > 0)
		scale(1.1, 1.1);
	else
		scale(1 / 1.1, 1 / 1.1);
}
//quando finisce il gioco aggiorna la scena per inserire il nome in classifica
void Game::giocoFinito()
{
	scenego = new QGraphicsScene();
	centerOn(0, 0);
	scenego->clear();
	setScene(scenego);
	setInteractive(false);    // disables events
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	scenego->setSceneRect(0, 0, 224, 288);
	scenego->addPixmap(Sprites::instance()->get("title-gameover"));
	
	buttonai->hide();
	buttonnormal->hide();

	//Inserimento nome a fine gioco
	{
		lineone->setGeometry(100, 300, 240, 40);
		//lineone->setText("Insert your name");
		QString stylesheet1 = QString("QLineEdit { font-size: 25px; background-color: #323232 ; border: none ; color: yellow; }");
		lineone->setStyleSheet(stylesheet1);
		lineone->show();
		scenego->addWidget(lineone);
	}

	//OkButton
	{
		QPixmap pixmap4(":/Sprites/ok.png");
		QIcon ButtonIcon4(pixmap4);
		ok->setIcon(ButtonIcon4);
		ok->setIconSize(QSize(40, 40));
		ok->move(338, 300);
		ok->setFixedHeight(40);
		ok->setFixedWidth(39);
		ok->show();
		ok->setEnabled(true);
		scenego->addWidget(ok);
	}

	//GoBackButton
	{
		scenego->addWidget(buttongoback);
		QPixmap pixmap5(":/Sprites/goback.png");
		QIcon ButtonIcon5(pixmap5);
		buttongoback->setIcon(ButtonIcon5);
		buttongoback->setIconSize(QSize(180, 50));
		buttongoback->move(135, 520);
		buttongoback->setFixedHeight(50);
		buttongoback->setFixedWidth(180);
		buttongoback->setEnabled(true);
		buttongoback->show();
	}

}

//Bottone Ranking a inizio game
void Game::buttonRanking()
{
	scene->addWidget(buttonRank);
	QPixmap pixmap2(":Sprites/Ranking.png");
	QIcon ButtonIcon2(pixmap2);
	buttonRank->setIcon(ButtonIcon2);
	buttonRank->setIconSize(QSize(106 * 2, 23 * 2));
	buttonRank->move(57 * 2, 172 * 2);
	buttonRank->setFixedHeight(23 * 2);
	buttonRank->setFixedWidth(106 * 2);
	buttonRank->setEnabled(true);
	centerOn(0, 0);
	buttonRank->show();
}

//schermata ranking 
void Game::ranking()
{
	//Creazione di una nuova scena contenente il ranking 
	sceneRanking = new QGraphicsScene();
	setScene(sceneRanking);
	centerOn(0, 0);
	scene->clear();
	setInteractive(false);    // disables events 
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	sceneRanking->setSceneRect(0, 0, 224, 288);
	sceneRanking->addPixmap(Sprites::instance()->get("title-rank"));

	//Aggiungo bottone che permette di tronare indietro dalla modalita Ranking 
	{
		sceneRanking->addWidget(buttonback);
		QPixmap pixmap3(":Sprites/back.png");
		QIcon ButtonIcon3(pixmap3);
		buttonback->setIcon(ButtonIcon3);
		buttonback->setIconSize(QSize(106 * 2, 23 * 2));
		buttonback->move(380, 515);
		buttonback->setFixedHeight(46);
		buttonback->setFixedWidth(50);
		buttonback->setEnabled(true);
		buttonback->show();
	}

	//Nascondo i residui dei bottoni
	buttonRank->hide();
	buttonStart->hide();
	buttonai->hide();
	buttonnormal->hide();

	// setup window size 
	this->setFixedHeight(2 * 288);
	this->setFixedWidth(2 * 225);

	engine.stop();

	//Inserisco numeri da uno a nove con puntino vicino 
	for (int j = 0; j < 9; j++)
	{
		posizione = sceneRanking->addText(QString::number(j + 1));
		posizione->setPos(28, 80 + (j * 15));                                      //setta la posizione 
		posizione->setDefaultTextColor(QColor(255, 255, 51));             //setta il colore 
		posizione->setZValue(3);

		punto = sceneRanking->addText(".");
		punto->setPos(37, 80 + (j * 15));                                      //setta la posizione 
		punto->setDefaultTextColor(QColor(255, 255, 51));             //setta il colore 
		punto->setZValue(3);
	}

	//Per il decimo  
	posizione = sceneRanking->addText("10");
	posizione->setPos(20, 80 + (15 * 9));                                      //setta la posizione 
	posizione->setDefaultTextColor(QColor(255, 255, 51));             //setta il colore 
	posizione->setZValue(3);

	//Per il puntino del decimo 
	punto = sceneRanking->addText(".");
	punto->setPos(37, 80 + (15 * 9));                                      //setta la posizione 
	punto->setDefaultTextColor(QColor(255, 255, 51));             //setta il colore 
	punto->setZValue(3);

	//Apro il file 
	QFile file("Ranking.txt");

	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		std::cout << " Could not open the file for reading\n";
		return;
	}

	QTextStream in(&file);
	QString line[100];
	int key[100];
	for (int l = 0; l < 100; l++)
	{
		if (line[l].isNull())
		{
			line[l] = in.readLine();
			if (l % 2 == 0)
			{
				key[l] = line[l].toInt();
				leaderboard.insert(key[l], NULL);
			}
			else
			{
				leaderboard[key[l - 1]] = line[l];
			}
		}
	}

	QMapIterator<int, QString> iter(leaderboard);
	int i = 0;
	int size = leaderboard.size();
	while (iter.hasNext())
	{	
		rank_pos = 215 - (i * 15) - ((10 - size) * 15);
		iter.next();
		nome = sceneRanking->addText(iter.value());
		nome->setPos(50, rank_pos);                                      //setta la posizione 
		nome->setDefaultTextColor(QColor(255, 255, 51));             //setta il colore 
		nome->setZValue(3);

		top_score = sceneRanking->addText(QString::number(iter.key()));
		top_score->setPos(165, rank_pos);                                      //setta la posizione 
		top_score->setDefaultTextColor(QColor(255, 255, 51));             //setta il colore 
		top_score->setZValue(3);
		if (iter.key()==0)
		{
			nome->setOpacity(0);
			top_score->setOpacity(0);
		}
		if (size > 10)
		{
			if ((size - i) > 10)
			{
				nome->setOpacity(0);
				top_score->setOpacity(0);
			}

		}
		i++; //spaziatura tra testo 
	}

	file.close();
}

// Quando premo ok dopo aver inserito il nome a fine partita
void Game::ok_clicked()
{
	QFile file("Ranking.txt");

	file.open(QFile::WriteOnly | QIODevice::Append);
	QTextStream out(&file);

	//Scrivo su file il nome inserito nella linea e il punteggio 
	name = lineone->text();
	final_score=(punteggio * 10) + (punt_big_food * 100) + punteggio_ghost + cont_fruit_eat ;

	out << final_score << endl << name << endl;
	

    file.flush();
    file.close();

	//Premo una volta ok e poi ritorno a start
	reset();

}

//Bottone Play a inizio game
void Game::buttonPlay()
{
	scene->addWidget(buttonStart);
	QPixmap pixmap(":Sprites/Play.png");
	QIcon ButtonIcon(pixmap);
	buttonStart->setIcon(ButtonIcon);
	buttonStart->setIconSize(QSize(78 * 2, 28 * 2));
	buttonStart->move(72 * 2, 144 * 2);
	buttonStart->setFixedHeight(28 * 2);
	buttonStart->setFixedWidth(78 * 2);
	buttonStart->setEnabled(true);
	buttonStart->show();
}

//Bottone per passaggio a modalità AI
void Game::buttonAI()
{
	scene->addWidget(buttonai);
	QPixmap pixmapai(":Sprites/buttonAi.png");
	QIcon ButtonIconai(pixmapai);
	buttonai->setIcon(ButtonIconai);
	buttonai->setIconSize(QSize(36 * 2, 36 * 2));
	buttonai->move(15, 5);
	buttonai->setFixedHeight(32);
	buttonai->setFixedWidth(36);
	buttonai->setEnabled(true);
	buttonai->show();
}

//Bottone per passaggio a modalità normale
void Game::buttonNormal()
{
	scene->addWidget(buttonnormal);
	QPixmap normale(":Sprites/normal.png");
	QIcon ButtonIconnor(normale);
	buttonnormal->setIcon(ButtonIconnor);
	buttonnormal->setIconSize(QSize(40 * 2, 84 * 2));
	buttonnormal->move(360, 2);
	buttonnormal->setFixedHeight(36);
	buttonnormal->setFixedWidth(80);
	buttonnormal->setEnabled(true);
	buttonnormal->show();
}

//Serve per confrontare posizione pacman e cibo
bool Game::confronto(Food* f, Pacman* p)
{
	//Creo variabile temporanea che prende la posizione del food corrente e la confronta con la posizione di pacman
	//Vado a sommare +6 in modo che le coordinate di pacman e del cibo coincidano
	//Sommo +3 per farlo corrispondere al cibo Power

	QPoint pos;
	pos = QPoint(f->x(), f->y());
	if (pos == QPoint(p->x() + 6, p->y() + 6) ||
		pos == QPoint(p->x() + 3, p->y() + 3))
		return true;
	else
		return false;
}

void Game::modNormal()
{
	impossible = false;
	reset();
}

void Game::modImpossible()
{
	impossible = true;
	reset();
}

// pause / resume game
void Game::tooglePause()
{
	if (cur_state == RUNNING && getSecond() >4)
	{
		
		engine.stop();

		Sounds::instance()->stop("siren_1");

		ps = scene->addPixmap( Sprites::instance()->get("pausa") );
		ps->setPos(84,153);
		ps->setZValue(1);
		ps->setScale(0.45);

		pacman->setOpacity(0);
		blinky->setOpacity(0);
		pinky->setOpacity(0);
		inky->setOpacity(0);
		clyde->setOpacity(0);

		cur_state = PAUSE;
	}
	else if (cur_state == PAUSE)
	{
		Sounds::instance()->play("siren_1");

		engine.start();	
		ps->setZValue(-1);
		cur_state = RUNNING;

		pacman->setOpacity(1);
		blinky->setOpacity(1);
		pinky->setOpacity(1);
		inky->setOpacity(1);
		clyde->setOpacity(1);
	}

}
void Game::assegnaB()
{
	std::list<int> tmp2 = blinky->getCammino();
	dimCamminoB = tmp2.size();
	for (int i = 0; i < 76; i++)
		tragittoB[i]->setVisible(false);

	for (int i = 0; i < blinky->getCammino().size(); i++)
	{
		tmpCamminoB[i] = tmp2.front();
		tragittoB[i]->setVisible(true);
		tragittoB[i]->setPos(pacman->getNodoX(tmpCamminoB[i]), pacman->getNodoY(tmpCamminoB[i]));
		tmp2.pop_front();
	}
	
}
void Game::assegnaC()
{
	std::list<int> tmp = clyde->getCammino();
	dimCamminoC = tmp.size();
	for (int i = 0; i < 76; i++)
		tragittoC[i]->setVisible(false);

	for (int i = 0; i < clyde->getCammino().size(); i++)
	{
		tmpCamminoC[i] = tmp.front();
		tragittoC[i]->setVisible(true);
		tragittoC[i]->setPos(pacman->getNodoX(tmpCamminoC[i]), pacman->getNodoY(tmpCamminoC[i]));
		tmp.pop_front();
	}
	
}