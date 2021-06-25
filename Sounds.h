#pragma once

#include <QSound>
#include <QSoundEffect>

#include "Include.h"

//Classe di tipo Singleton
class Sounds
{
private:
	
	std::map<std::string, QSound*> sounds;

	Sounds();

public:
	//Singleton
	static Sounds* instance();
	
	//Aziona il suono dato un id
	void play				(const std::string& id);
	bool isFinished			(const std::string& id);
	void setLoops			(const std::string& id,int num);
	void stop				(const std::string& id);
	int  loop				(const std::string& id);
	void setLoopsInfinite	(const std::string& id);

};