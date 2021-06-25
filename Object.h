#pragma once
#include "Include.h"
#include <iostream>

#include <QGraphicsPixmapItem>


class Object :public QGraphicsPixmapItem
{

public:

	Object();

	// Animazione ( =compute next texture )
	virtual void animate() = 0;

	// Advance ( =compute next location/state )
	virtual void advance() = 0;

	
};