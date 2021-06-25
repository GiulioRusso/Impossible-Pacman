#pragma once

#include "Include.h"

class Pinky : public Ghosts
{
private:

	QPixmap texture_Pinky[2][5][2];
	int plast_node ;
	bool pgoout ;

public:
	
	Pinky(QPoint position);

	virtual void animate();
	virtual void advance();
};