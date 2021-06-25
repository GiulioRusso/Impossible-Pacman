#pragma once
#include "Include.h"

class Boxx : public Inert
{
private:

	QPixmap texture_box;
	QPixmap text[3];
public:
	Boxx(QPoint position);

	
	virtual void animate() ;

	virtual void advance() {};
};
