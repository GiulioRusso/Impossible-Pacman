#pragma once
#include "Include.h"
#include <QPixmap>

class Sprites
{
private:
	Sprites();

	QPixmap		 title_screen;
	QPixmap		 allPacman;
	QPixmap      pacman;
	QPixmap		 cher;
	QPixmap		 gameover;
	QPixmap		 rank;
	QPixmap		 ai, ai3;
	QPixmap		 pause;

public:
	//Singleton
	static Sprites* instance();

	// Estrae le texture dalle sprites dato un determinato id
	QPixmap get(const std::string& id);
};