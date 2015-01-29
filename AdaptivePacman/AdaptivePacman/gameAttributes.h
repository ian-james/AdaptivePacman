#ifndef GAMEATTRIBUTES_H
#define GAMEATTRIBUTES_H

#include"headers.h"
#include"agent.h"
#include"item.h"
#include"point.h"

#include"node.h"

class GameAttributes
{
	friend ostream& operator<<(ostream &out,GameAttributes& gs);
	friend istream& operator>>(istream &in,GameAttributes& gs);

public:

	static GameAttributes* Instance();
	
	void setConstants(int nghosts,int ngames,int maxSteps,string boardFile);	
	void editAttributes();

	int getSingleSquareCost(Pieces p) const;

private:
	GameAttributes();	
	~GameAttributes();	
	GameAttributes(const GameAttributes&);
	GameAttributes& operator=(const GameAttributes&);

//private:
public:
	bool displayInfo;
	int NGHOSTS,NGAMES,MAXSTEPS;
	int GAMEID;
	int currGame,currSteps;
	string boardFile; //The board file name

	int stateTime,stateVisionLength,stateVisionType,stateAlgo; /*new stuff to decrease comnbinatorical explosion*/
	int pacmanVisionLength;
	int timeFruit;
	double fruitFreq;

	double pacmanVal,ghostVal,fruitVal,ppVal,tokenVal,successVal; //Value of eating each;
	int fleeTime,deadTime,chaseTime;

	int minFruitDistance,maxFruitDistance;

	//Vision Distance
	int nStepsSurround,nStepsDirect; 	

	//minimax stuff
	int depthOfSearch; //Depth of search for sss and minmax	

	//flocking stuff	
	double seperationForce,alignForce,cohForce, hungerForce;//forces to head toward each of these values	
	int maxSeperation;		

	//These are forces for evaluating each algorithm
	double forceProtectToken,forceProtectPP,forceProtectFruit,forceItemCenter;
	double forcePacPos,forceAwayGhost,forceTeamPos,forcePacDir; 

	//Enter Pacman forces
	double fruitForce,tokenForce,ppForce, towardsEdGhosts, awayGhosts, awayGhostCenter, towardItemCenter; 

	Item ft;
	Item t;
	Item pp;
	Agent p;
	vector<Agent> g;

	//Debug
};
#endif

