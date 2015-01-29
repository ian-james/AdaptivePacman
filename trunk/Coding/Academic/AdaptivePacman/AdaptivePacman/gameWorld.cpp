#include"gameWorld.h"

extern GameWorld gw;

GameWorld::GameWorld()
{
	this->runGraphics = true;
	firstTimeLoop = true;
	size = 10;
	this->stopDraw = false;
}

#ifdef gwGraphics

void drawString(int x,int y,string line)
{	
	glRasterPos2f(x,y);
	for(unsigned int i = 0;i<line.size();i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,line[i]);
}

void GameWorld::initWindow(int argc,char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000,850);
	glutCreateWindow("Pacman");

	//init
	{
		glClearColor(0,0,0,0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
		glMatrixMode(GL_PROJECTION);		
//		glDisable(GL_TEXTURE);
		glLoadIdentity();		
		glOrtho(0,215,-150,20,-1,1);		
	}
}


void GameWorld::Keyboard(int key, int x, int y)
{	
	if(key == GLUT_KEY_F1)
	{
		cout << "OUTPUT START\n";
		cout << "Curr Game = " << GameAttributes::Instance()->currGame << "\n";
		cout << "Pacman lives = " << myGame.pacman.score << "\n";
		cout << "Pacman Steps = " << myGame.pacman.steps << "\n";
	}
	else if(key == GLUT_KEY_F5)
	{
		this->stopDraw = !stopDraw;
	}
	else if(key == GLUT_KEY_F9)
	{
		exit(1);
	}
}

void GameWorld::drawWorld()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	drawTGAMEINFO();

	myGame.b.drawBoard();
	drawAgent(myGame.pacman);
	for (unsigned int i = 0; i<myGame.bads.size(); i++)
		drawAgent(myGame.bads[i]);

	glutSwapBuffers();
}

bool GameWorld::graphicMainLoop()
{
	GameState ans = myGame.runGame();
	if(ans != ST_ALIVE)	
	{
		if(!myGame.resetNextLife(ans))
			return  myGame.resetGame();					
		return false;
	}
	
	drawWorld();	
	return false;
}	

void GameWorld::drawAgent(const Agent &a) const
{
	if(a.getIsDead())
		return;

	Color c = a.getStateColor();
	Point<int> pos = a.pos;	
	glBegin(GL_POLYGON);
		glColor3f(c.r,c.g,c.b);
		glVertex2f(pos.getX()*size,-pos.getY()*size); //Bottom left
		glVertex2f(pos.getX()*size + size,-pos.getY()*size);		//Top Left 
		glVertex2f(pos.getX()*size,-pos.getY()*size + size); 
		glVertex2f(pos.getX()*size + size,-pos.getY()*size +size);			
		glVertex2f(pos.getX()*size,-pos.getY()*size);
		glColor3f(c.retR,c.retG,c.retB);
	glEnd();	
}

void GameWorld::drawTGAMEINFO() const
{
	char line[80];
	sprintf_s(line,"GAMEID = %d, Game Number: %d Pacman Score: %d Steps: %d   Lives: %d",GameAttributes::Instance()->GAMEID,GameAttributes::Instance()->currGame, myGame.pacman.score,myGame.pacman.steps,myGame.pacman.lives);
	glColor3f(1,1,1);
	drawString(75,15,line);	
}
#endif

bool GameWorld::mainLoop()
{
	bool done = false;
	while(!done)
	{
		GameState ans =  myGame.runGame();

		if(ans != ST_ALIVE)	
		{
			if(!myGame.resetNextLife(ans))
			{
				done = myGame.resetGame();					
				cout << "Finished Game Before " << GameAttributes::Instance()->currGame << "\n";
			}
		}
	}
	return true; 
}

