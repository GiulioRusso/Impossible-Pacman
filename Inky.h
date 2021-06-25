#pragma once

#include "Include.h"

class Inky : public Ghosts
{
private:
	QPixmap	 texture_Inky[2][5][2];
	int		 ilast_node;

	bool arrivatoI;
	bool igoout;
	Blinky* blinky;
public:

	Inky(QPoint position);

	virtual void animate() ;
	virtual void advance() ;
};