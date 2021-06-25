#pragma once
#include "Include.h"
class Life : public Inert
{
private:
	// textures
	QPixmap texture_life;
	
	
public:

	Life(QPoint position);
	virtual void animate() {};
	virtual void advance() ;
};