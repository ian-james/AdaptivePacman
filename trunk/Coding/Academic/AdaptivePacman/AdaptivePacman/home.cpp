
#include"gameWorld.h"
GameWorld gw;

#ifdef gwGraphics

void Render();
void Keyboard(int key,int x,int y);
void myTimer(int value);

void Render()
{
	if(gw.graphicMainLoop()) //Done
		exit(0);
}

void Keyboard(int key,int x,int y)
{
	gw.Keyboard(key,x,y);
}

void myTimer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(1000/60, myTimer, 1);	
}
#endif

int main(int argc,char **argv)
{
	if(argc < 8)
	{
		// Use Graphics (y|n)
		// AttsID
		// nGHOSTS
		// nGAMES
		// maxSteps
		// boardFile
		// pathAttsFile
		// pathResFile 
	
		
		cout << "Usage: Param1: y|n. Should display graphics.\nParam2: GameID, This should be the ID for the variable file.\n"
		<< "Param3: Num of Ghosts\nParam4: Num Games\nParam5: Max Simulation Steps\nParam6: board file\nParam 7: attsfile\nParam 8: resFile\n";
		return 0;
	}

	const int IDNUM = atoi(argv[2]);
	gw.myGame.setGameID(IDNUM);

	int seed = -1;//atoi(argv[7]);
	(seed< 0)?init():init(seed);
	
	//Set whether we are using Graphics, this will be overridden by gwGraphics

	gw.myGame.init(atoi(argv[3]),atoi(argv[4]),atoi(argv[5]),argv[6],argv[7],argv[8]);

	gw.runGraphics = false;
	gw.myGame.useGraphics = false;
	#ifdef gwGraphics
	if(strcmp(argv[1],"y") == 0) 
		gw.runGraphics = true;
		gw.myGame.useGraphics = true;
	#endif

	if(gw.runGraphics)
	{	
		#ifdef gwGraphics
		gw.initWindow(argc,argv);			
		glutDisplayFunc(Render);	
		glutSpecialFunc(Keyboard);	
		glutTimerFunc(1000/60,myTimer,1);
		glutMainLoop();
		#else
			gw.mainLoop();		
		#endif

	}	
	else
	{
		gw.mainLoop();
	}
}

