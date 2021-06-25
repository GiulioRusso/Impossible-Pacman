#include "Include.h"
Boxx::Boxx(QPoint position) : Inert()
{
	//Imposto texture
	texture_box = Sprites::instance()->get("box");
	//Ai

	text[0] = Sprites::instance()->get("boxAi");
	text[1] = Sprites::instance()->get("box");
	text[2] = Sprites::instance()->get("boxAi3");


	setPixmap(texture_box);
	setPos(0,20);
	setZValue(1);

}
void Boxx::animate()
{
	Inert::animate();

	if (!impossible)
		setPixmap(texture_box);
	if (impossible)
	{
		setPixmap(text[(animationCounterInert / 8) % 3]);
	}

}