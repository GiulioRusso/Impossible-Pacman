#pragma once

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <QKeyEvent>
#include <iostream>
#include <QLineEdit>
#include <QPushButton>
#include <QFile>
#include <QTextStream>
#include <QMap>
#include <QMapIterator>

#include "Include.h"




class Game : public QGraphicsView
{
	Q_OBJECT

		enum Game_State { READY, RUNNING, PAUSE, GAME_OVER };
private:

	Game(QGraphicsView* parent = 0);
	static Game* uniqueInstance;
	//Scena
	QGraphicsScene* scene, * scenego, * sceneRanking;
	//Motore di gioco
	QTimer				engine;
	//Stato di gioco
	Game_State			cur_state;
	//Oggetti del gioco
	Pacman* pacman;
	Blinky* blinky;
	Pinky* pinky;
	Inky* inky;
	Clyde* clyde;
	Boxx* box;
	Food* food[245];
	Fruit* fruit;
	Life* life[3];
	//Testi di gioco
	QGraphicsPixmapItem* ready, * lv, * sc;
	//Per inserire nome quando finisci di giocare
	QLineEdit* lineone;
	//Bottoni
	QPushButton* ok, * buttongoback, * buttonRank, * buttonback, * buttonStart, *buttonai, *buttonnormal;

	//Per Classifica finale
	int						 final_score;
	QString 				 name;
	QFile 					 file;
	QMap<int, QString> 		 leaderboard;
	QTextStream				 out, in;
	QGraphicsTextItem		* posizione, * punto, * nome, * top_score;
	int						 rank_pos;

	//variabili gameover
	int 				 	 gioco_finito;

	//Target modalita' Normale
	QGraphicsLineItem* lineBlinky,*linePinky,*lineClyde,*lineInky;
	bool targetActive;

	//Altre Variabili
	bool impossible;
	int second, msecond;
	int contaVite;
	bool ciboRimanente[245],fruttoMangiato=false;

	//Punteggio
	int             fantasmiMangiati = 0;
	int            punteggio_ghost = 0;
	int             increase = 0;
	int            check_score = 0;
	bool            inc_life = true;
	int             punteggio = 0;
	int             punt_big_food = 0;
	int            cont_fruit_eat = 0;

	bool eat = false;
	
	QGraphicsTextItem* score,*point_ghostEatp, * point_ghostEati, * point_ghostEatc, * point_ghostEatb;
	int                point_blinky, point_clyde, point_pinky, point_inky;
	int            	   time_blinky_die, time_inky_die, time_pinky_die, time_clyde_die;

	//Pausa
	QGraphicsPixmapItem* ps;
	
	//PercorsoAi
	QGraphicsRectItem* tragittoB[100],*tragittoC[100];
	bool targetAi ;

	int tmpCamminoB[100], tmpCamminoC[100];
	int dimCamminoB, dimCamminoC;



protected:
	virtual void    keyPressEvent(QKeyEvent* e);
	virtual void	wheelEvent(QWheelEvent* e);

public:
	static Game* instance();

	Pacman* getPacman()							 { return pacman; }
	Blinky* getBlinky()							 { return blinky;}
	Clyde* getClyde()							 { return clyde; }
	Pinky* getPinky()							 { return pinky; }
	Inky* getInky()								 { return inky; }
	QGraphicsScene* getScene()					 { return scene; }

	int				getSecond()					 { return second; }
	Game_State      getCurState()				 { return cur_state; }
	
	void buttonRanking();
	void buttonPlay();
	void buttonAI();
	void buttonNormal();
	int  winner();
	bool confronto(Food* f, Pacman* p);
	bool getFruit() { return fruttoMangiato; }

public slots:
	
	void advance();
	void start();
	void reset();
	void gameover();
	void giocoFinito();
	void tooglePause();
	void ranking();
	void ok_clicked();
	void modImpossible();
	void modNormal();
	void hide_score_b() { point_ghostEatb->setOpacity(0); }
	void hide_score_c() { point_ghostEatc->setOpacity(0); }
	void hide_score_p() { point_ghostEatp->setOpacity(0); }
	void hide_score_i() { point_ghostEati->setOpacity(0); }
	void hide_fruit() { fruit->setOp(); fruit->set_Fruit_iseat(false); }
	void assegnaB();
	void assegnaC();
	void assegnaI() {};
};
