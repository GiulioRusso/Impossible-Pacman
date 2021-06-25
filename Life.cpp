#include "Include.h"
Life::Life(QPoint position) :Inert()
{
	// textures
	texture_life = Sprites::instance()->get("pacman-sx-0");

	setPixmap(texture_life);
	setPos(position - QPoint(pixmap().width(), pixmap().height()));

	// set a high z-value so it is displayed on top of other objects
	setZValue(1);
}
void Life::advance()
{

}