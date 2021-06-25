#pragma once

#include "Include.h"

class Blinky : public Ghosts
{
	QPixmap texture_Blinky[2][5][2];
public:

	Blinky(QPoint position);

	virtual void animate();
	virtual void advance() ;


};