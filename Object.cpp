#include "Include.h"

Object::Object() : QGraphicsPixmapItem(0)
{
	// Aggiungo scena
	Game::instance()->getScene()->addItem(this);

}