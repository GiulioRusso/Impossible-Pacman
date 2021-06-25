#pragma once

#include <QPoint>
#include <QPixmap>
#include <QBitmap>
#include <QColor>
#include <string>

#include "Include.h"


enum Direction		 {UNKNOWN,RIGHT,LEFT,UP,DOWN};
enum State	     	 {SCATTER,CHASE,VULNERABLE,EATEN,AI};
struct Point
{
	int x;
	int y;
};

// return true if the given 'str' ends with 'end'
static bool strends(std::string const& str, std::string const& end)
{
	if (str.length() >= end.length())
		return (0 == str.compare(str.length() - end.length(), end.length(), end));
	else
		return false;
}

// load texture with transparency using the given color as mask
static QPixmap loadTextureTransparent(const std::string& file, QColor mask_color = Qt::black)
{
	QPixmap pixmap(file.c_str());
	pixmap.setMask(pixmap.createMaskFromColor(mask_color));
	return pixmap;
}

// load texture from image file 
// transparent pixel are identified with the given color
// if an image format not supporting transparency is used (e.g. like .bmp)
static QPixmap loadTexture(const std::string file, QColor mask_color = Qt::black)
{
	if (strends(file, ".bmp"))
		return loadTextureTransparent(file, mask_color);
	else
		return QPixmap(file.c_str());
}