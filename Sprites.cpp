#include "Include.h"
#include <QBitmap>

//Ghosts
static QRect red_ghosts(2, 65, 15, 15);
static QRect pink_ghosts(2, 81, 15, 15);
static QRect heavenly_ghosts(2, 97, 15, 15);
static QRect orange_ghosts(2, 113, 15, 15);
static QRect eyes_ghost(584, 81, 15, 15);

//AI
static QRect blinkyAi(456, 64, 15, 15);
static QRect inkyAi(456, 96, 15, 15);
static QRect clydeAi(456, 112, 15, 15);
static QRect pinkyAi(456, 80, 15, 15);
static QRect ghostVulnerableAi(584, 64, 15, 15);

Sprites* Sprites::instance()
{
	static Sprites uniqueInstance;
	return &uniqueInstance;
}

// Funzione di utilita'
QRect moveBy(QRect rect, int x, int y, int dx = 15, int dy = 15, int border_x = 1, int border_y = 1)
{
	rect.moveTo(QPoint(rect.x() + x * dx + x * border_x, rect.y() + y * dy + y * border_y));
	return rect;
}
Sprites::Sprites()
{
	title_screen = loadTexture(":/Sprites/title.png");
	allPacman	 = loadTexture(":/Sprites/Allpacman.png", QColor(0, 0, 0));
	pacman		 = loadTexture(":/Sprites/characters.png", QColor(0, 0, 0));
	cher		 = loadTexture(":/Sprites/Cherry.png", QColor(0,0,0));
	gameover     = loadTexture(":/Sprites/scene-go.png");
	rank         = loadTexture(":/Sprites/rank.png");
	ai			 = loadTexture(":/Sprites/ai.png", QColor(0, 0, 0));
	ai3			 = loadTexture(":/Sprites/ai3.png", QColor(0, 0, 0));
	pause 		 = loadTexture(":/Sprites/pause.png");
}
QPixmap Sprites::get(const std::string& id)
{
	//Schermata di gioco iniziale
	if (id == "title-screen")
		return title_screen.copy(QRect(0, 0, 224, 288));

	//Box
	else if (id == "box")
		return allPacman.copy(QRect(228, 0, 224 , 248));
	//Ai
	else if (id == "boxAi")
		return ai.copy(QRect(228, 0, 224, 248));
	else if (id == "boxAi3")
		return ai3.copy(QRect(228, 0, 224, 248));
	
	//Food
	else if (id == "food")
		return allPacman.copy(QRect(11, 11, 2, 2));
	else if (id == "big-food")
		return allPacman.copy(QRect(8, 24, 8, 8));

	//Pacman
	//left
	else if (id == "pacman-sx-0")
		return pacman.copy(QRect(18, 17, 15, 15));
	else if (id == "pacman-sx-1")
		return pacman.copy(QRect(2, 17, 15, 15));
	else if (id == "pacman-sx-2")
		return pacman.copy(QRect(34, 1, 15, 15));
	else if (id == "pacman-sx-3")
		return pacman.copy(QRect(2, 17, 15, 15));
	//right
	else if (id == "pacman-dx-0")
		return pacman.copy(QRect(18, 1, 15, 15));
	else if (id == "pacman-dx-1")
		return pacman.copy(QRect(2, 1, 15, 15));
	else if (id == "pacman-dx-2")
		return pacman.copy(QRect(34, 1, 15, 15));
	else if (id == "pacman-dx-3")
		return pacman.copy(QRect(2, 1, 15, 15));
	//up
	else if (id == "pacman-up-0")
		return pacman.copy(QRect(18, 33, 15, 15));
	else if (id == "pacman-up-1")
		return pacman.copy(QRect(2, 33, 15, 15));
	else if (id == "pacman-up-2")
		return pacman.copy(QRect(34, 1, 15, 15));
	else if (id == "pacman-up-3")
		return pacman.copy(QRect(2, 33, 15, 15));
	//down
	else if (id == "pacman-down-0")
		return pacman.copy(QRect(18, 49, 15, 15));
	else if (id == "pacman-down-1")
		return pacman.copy(QRect(2, 49, 15, 15));
	else if (id == "pacman-down-2")
		return pacman.copy(QRect(34, 1, 15, 15));
	else if (id == "pacman-down-3")
		return pacman.copy(QRect(18, 49, 15, 15));
	//Morte Pacman
	else if (id == "pacman-dead-0")
		return pacman.copy(QRect(50, 1, 15, 15));
	else if (id == "pacman-dead-1")
		return pacman.copy(QRect(66, 1, 15, 15));
	else if (id == "pacman-dead-2")
		return pacman.copy(QRect(82, 1, 15, 15));
	else if (id == "pacman-dead-3")
		return pacman.copy(QRect(98, 1, 15, 15));
	else if (id == "pacman-dead-4")
		return pacman.copy(QRect(114, 1, 15, 15));
	else if (id == "pacman-dead-5")
		return pacman.copy(QRect(130, 1, 15, 15));
	else if (id == "pacman-dead-6")
		return pacman.copy(QRect(146, 1, 15, 15));
	else if (id == "pacman-dead-7")
		return pacman.copy(QRect(162, 1, 15, 15));
	else if (id == "pacman-dead-8")
		return pacman.copy(QRect(178, 1, 15, 15));
	else if (id == "pacman-dead-9")
		return pacman.copy(QRect(194, 1, 15, 15));
	else if (id == "pacman-dead-10")
		return pacman.copy(QRect(210, 1, 15, 15));
	
	//Fruit
	else if (id == "Cherry")
		return cher;

	//Ghost Spaventati
	else if (id == "ghost-vulnerable-0")
			return pacman.copy(moveBy(red_ghosts, 8, 0));
	else if (id == "ghost-vulnerable-1")
		return pacman.copy(moveBy(red_ghosts, 9, 0));

	else if (id == "ghost-finish-vulnerable-0")
		return pacman.copy(moveBy(red_ghosts, 10, 0));
	else if (id == "ghost-finish-vulnerable-1")
		return pacman.copy(moveBy(red_ghosts, 11, 0));
	//Ghost Spaventati Ai
	else if (id == "ghost-vulnerable-ai-0")
		return ai.copy(ghostVulnerableAi);
	else if (id == "ghost-vulnerable-ai-1")
		return ai.copy(moveBy(ghostVulnerableAi, 1, 0));

	else if (id == "ghost-finish-vulnerable-ai-0")
		return ai.copy(moveBy(ghostVulnerableAi, 2, 0));
	else if (id == "ghost-finish-vulnerable-ai-1")
		return ai.copy(moveBy(ghostVulnerableAi, 3, 0));

	//FoodAi
	else if (id == "foodAi")
		return ai.copy(QRect(11, 11, 2, 2));
	else if (id == "big-foodAi")
		return ai.copy(QRect(8, 24, 8, 8));

	//Ghosts Eyes
	else if (id == "eyes-dx")
		return allPacman.copy(eyes_ghost);
	else if (id == "eyes-sx")
		return allPacman.copy(moveBy(eyes_ghost, 1, 0));
	else if (id == "eyes-down")
		return allPacman.copy(moveBy(eyes_ghost, 3, 0));
	else if (id == "eyes-up")
		return allPacman.copy(QRect(618, 81, 15, 15));

	//Blinky
	else if (id == "red-ghost-dx-0")
		return pacman.copy(red_ghosts);
	else if (id == "red-ghost-dx-1")
		return pacman.copy(moveBy(red_ghosts, 1, 0));
	else if (id == "red-ghost-sx-0")
		return pacman.copy(moveBy(red_ghosts, 2, 0));
	else if (id == "red-ghost-sx-1")
		return pacman.copy(moveBy(red_ghosts, 3, 0));
	else if (id == "red-ghost-up-0")
		return pacman.copy(moveBy(red_ghosts, 4, 0));
	else if (id == "red-ghost-up-1")
		return pacman.copy(moveBy(red_ghosts, 5, 0));
	else if (id == "red-ghost-down-0")
		return pacman.copy(moveBy(red_ghosts, 6, 0));
	else if (id == "red-ghost-down-1")
		return pacman.copy(moveBy(red_ghosts, 7, 0));

	//Pinky
	else if (id == "pink-ghost-dx-0")
		return pacman.copy(pink_ghosts);
	else if (id == "pink-ghost-dx-1")
		return pacman.copy(moveBy(pink_ghosts, 1, 0));
	else if (id == "pink-ghost-sx-0")
		return pacman.copy(moveBy(pink_ghosts, 2, 0));
	else if (id == "pink-ghost-sx-1")
		return pacman.copy(moveBy(pink_ghosts, 3, 0));
	else if (id == "pink-ghost-up-0")
		return pacman.copy(moveBy(pink_ghosts, 4, 0));
	else if (id == "pink-ghost-up-1")
		return pacman.copy(moveBy(pink_ghosts, 5, 0));
	else if (id == "pink-ghost-down-0")
		return pacman.copy(moveBy(pink_ghosts, 6, 0));
	else if (id == "pink-ghost-down-1")
		return pacman.copy(moveBy(pink_ghosts, 7, 0));
	//Inky
	else if (id == "heavenly-ghost-dx-0")
		return pacman.copy(heavenly_ghosts);
	else if (id == "heavenly-ghost-dx-1")
		return pacman.copy(moveBy(heavenly_ghosts, 1, 0));
	else if (id == "heavenly-ghost-sx-0")
		return pacman.copy(moveBy(heavenly_ghosts, 2, 0));
	else if (id == "heavenly-ghost-sx-1")
		return pacman.copy(moveBy(heavenly_ghosts, 3, 0));
	else if (id == "heavenly-ghost-up-0")
		return pacman.copy(moveBy(heavenly_ghosts, 4, 0));
	else if (id == "heavenly-ghost-up-1")
		return pacman.copy(moveBy(heavenly_ghosts, 5, 0));
	else if (id == "heavenly-ghost-down-0")
		return pacman.copy(moveBy(heavenly_ghosts, 6, 0));
	else if (id == "heavenly-ghost-down-1")
		return pacman.copy(moveBy(heavenly_ghosts, 7, 0));
	//Clyde
	else if (id == "orange-ghost-dx-0")
		return pacman.copy(orange_ghosts);
	else if (id == "orange-ghost-dx-1")
		return pacman.copy(moveBy(orange_ghosts, 1, 0));
	else if (id == "orange-ghost-sx-0")
		return pacman.copy(moveBy(orange_ghosts, 2, 0));
	else if (id == "orange-ghost-sx-1")
		return pacman.copy(moveBy(orange_ghosts, 3, 0));
	else if (id == "orange-ghost-up-0")
		return pacman.copy(moveBy(orange_ghosts, 4, 0));
	else if (id == "orange-ghost-up-1")
		return pacman.copy(moveBy(orange_ghosts, 5, 0));
	else if (id == "orange-ghost-down-0")
		return pacman.copy(moveBy(orange_ghosts, 6, 0));
	else if (id == "orange-ghost-down-1")
		return pacman.copy(moveBy(orange_ghosts, 7, 0));
	//Ai Ghosts
	 //blinky ai
	 else if (id == "blinkydx-0")
		 return ai.copy(blinkyAi);
	 else if (id == "blinkydx-1")
		 return ai.copy(moveBy(blinkyAi, 1, 0));
	 else if (id == "blinkysx-0")
		 return ai.copy(moveBy(blinkyAi, 2, 0));
	 else if (id == "blinkysx-1")
		 return ai.copy(moveBy(blinkyAi, 3, 0));
	 else if (id == "blinkyup-0")
		 return ai.copy(moveBy(blinkyAi, 4, 0));
	 else if (id == "blinkyup-1")
		 return ai.copy(moveBy(blinkyAi, 5, 0));
	 else if (id == "blinkydown-0")
		 return ai.copy(moveBy(blinkyAi, 6, 0));
	 else if (id == "blinkydown-1")
		 return ai.copy(moveBy(blinkyAi, 7, 0));
	 //clyde ai
	 else if (id == "clydedx-0")
		 return ai.copy(clydeAi);
	 else if (id == "clydedx-1")
		 return ai.copy(moveBy(clydeAi, 1, 0));
	 else if (id == "clydesx-0")
		 return ai.copy(moveBy(clydeAi, 2, 0));
	 else if (id == "clydesx-1")
		 return ai.copy(moveBy(clydeAi, 3, 0));
	 else if (id == "clydeup-0")
		 return ai.copy(moveBy(clydeAi, 4, 0));
	 else if (id == "clydeup-1")
		 return ai.copy(moveBy(clydeAi, 5, 0));
	 else if (id == "clydedown-0")
		 return ai.copy(moveBy(clydeAi, 6, 0));
	 else if (id == "clydedown-1")
		 return ai.copy(moveBy(clydeAi, 7, 0));

	 //inky ai
	 else if (id == "inkydx-0")
		 return ai.copy(inkyAi);
	 else if (id == "inkydx-1")
		 return ai.copy(moveBy(inkyAi, 1, 0));
	 else if (id == "inkysx-0")
		 return ai.copy(moveBy(inkyAi, 2, 0));
	 else if (id == "inkysx-1")
		 return ai.copy(moveBy(inkyAi, 3, 0));
	 else if (id == "inkyup-0")
		 return ai.copy(moveBy(inkyAi, 4, 0));
	 else if (id == "inkyup-1")
		 return ai.copy(moveBy(inkyAi, 5, 0));
	 else if (id == "inkydown-0")
		 return ai.copy(moveBy(inkyAi, 6, 0));
	 else if (id == "inkydown-1")
		 return ai.copy(moveBy(inkyAi, 7, 0));

	 //pinky ai
	 else if (id == "pinkydx-0")
		 return ai.copy(pinkyAi);
	 else if (id == "pinkydx-1")
		 return ai.copy(moveBy(pinkyAi, 1, 0));
	 else if (id == "pinkysx-0")
		 return ai.copy(moveBy(pinkyAi, 2, 0));
	 else if (id == "pinkysx-1")
		 return ai.copy(moveBy(pinkyAi, 3, 0));
	 else if (id == "pinkyup-0")
		 return ai.copy(moveBy(pinkyAi, 4, 0));
	 else if (id == "pinkyup-1")
		 return ai.copy(moveBy(pinkyAi, 5, 0));
	 else if (id == "pinkydown-0")
		 return ai.copy(moveBy(pinkyAi, 6, 0));
	 else if (id == "pinkydown-1")
		 return ai.copy(moveBy(pinkyAi, 7, 0));

	
	//scene gameover
	else if (id == "title-gameover")
		return gameover.copy(QRect(0, 0, 224, 288));

	else if (id == "title-rank")
		return rank.copy(QRect(0, 0, 224, 288));

	else if (id == "pausa")
		return pause.copy(QRect(0, 0, 125, 32));
		
	else
		return QPixmap();

}