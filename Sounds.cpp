#include "Include.h"
Sounds* Sounds::instance()
{
	static Sounds uniqueInstance;
	return &uniqueInstance;
}


Sounds::Sounds()
{
	sounds["beginning"]			 = new QSound(":/Sounds/beginning.wav");
	sounds["death"]				 = new QSound(":/Sounds/death.wav");
	sounds["eat1"]				 = new QSound(":/Sounds/munch_1.wav");
	sounds["eat2"]				 = new QSound(":/Sounds/munch_2.wav");
	sounds["eatfruit"]			 = new QSound(":/Sounds/eatfruit.wav");
	sounds["hunt"]				 = new QSound(":/Sounds/hunt.wav");
	sounds["eyes"]				 = new QSound(":/Sounds/eyes.wav");
	sounds["eatghost"]			 = new QSound(":/Sounds/eatghost.wav");
	sounds["siren_1"]			 = new QSound(":/Sounds/siren_1.wav");

}

void Sounds::play(const std::string& id)
{
	if (sounds.find(id) != sounds.end())
		sounds[id]->play();
	
}
bool Sounds::isFinished(const std::string& id)
{
	return sounds[id]->isFinished();
}
void Sounds::setLoops(const std::string& id,int num)
{
	sounds[id]->setLoops(num);
}
void Sounds::setLoopsInfinite(const std::string& id)
{
	sounds[id]->setLoops(QSound::Infinite);
}
void Sounds::stop(const std::string& id)
{
	sounds[id]->stop();
}
int Sounds::loop(const std::string& id)
{
	return sounds[id]->loopsRemaining();
}