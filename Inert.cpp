#include "Include.h"
Inert::Inert() : Object()
{
	animationCounterInert = 0;
	impossible = false;
}
void Inert::animate()
{
	animationCounterInert++;
	
}
