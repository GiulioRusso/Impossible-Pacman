#pragma once

#include "Include.h"
class Clyde : public Ghosts
{
private:
	QPixmap texture_Clyde[2][5][2];
	int clast_node;
	
	bool arrivatoC;
	bool cgoout;

public:

	Clyde(QPoint position);
	virtual void animate() ;
	virtual void advance() ;
};