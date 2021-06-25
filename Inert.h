#pragma once
#include "Include.h"

class Inert : public Object
{
protected:
	int animationCounterInert;
	bool impossible;

public:
	
	Inert();

	// Animazione ( =compute next texture )
	virtual void animate() ;

	// Advance ( =compute next location/state )
	virtual void advance()=0;

	virtual void setImpossible(bool _impossible) { impossible = _impossible; }

};