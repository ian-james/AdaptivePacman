#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include"gameController.h"

class GameWorld
{
public:
	GameWorld();

	//Drawing stuff about the world.

	#ifdef gwGraphics

	void drawWorld();
	void setupBoard();
	void Keyboard(int key,int x,int y);
	void initWindow(int argc, char **argv);
	
	void drawAgent(const Agent & a) const;
	void drawTGAMEINFO() const;

	bool graphicMainLoop();

	#endif

	bool mainLoop();

//private:
	GameController myGame;
	bool runGraphics; //Run Graphics or command line;
	bool firstTimeLoop;
	bool stopDraw; //Pause Drawing after
	double size; //ScaleSize 
};
#endif

