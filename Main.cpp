#include <iostream>
#include <QApplication>

#include "Include.h"

int main(int argc,char* argv[])
{
	//Creo Applicazione Qt
	QApplication app(argc, argv);

	//Instanzio musica e sprites
	Sprites	::instance();
	Sounds	::instance();

	//Creo e mostro il gioco
	Game	::instance()->show();

	return app.exec();

}