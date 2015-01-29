#ifndef AGENT_H
#define AGENT_H

#include"headers.h"
#include"point.h"
#include"color.h"
#include"path.h"
#include"scoreSystem.h"

class Agent
{
	friend ostream& operator<<(ostream &out, const Agent & g);
	friend istream& operator>>(istream &in, Agent & g);
	
public:	
	Agent(AgentType at = GHOST);
	Agent(AgentType  at,Point<int> pos,int speed);	
	bool operator<(const Agent & a) const;
	bool operator==(const Agent & a) const;

	~Agent();

	bool myMove();
	Point<int> chooseMove(const ScoreSystem & movesScore);

	void autoDecrementCurrLives();
	void setCurrentState(AgentState ags);
	bool getIsDead() const;
	void incSteps(int t = 1);

	Color getStateColor() const;

	void reset(bool decrementLife = false);

	string getAgentStr() const;

	void setNextPos(const Point<int> & nextPos);

	bool isPacman() const;

	bool incAgentState(); //Returns whether pacman is dead

	bool shouldChase() const;	

	void setPathInfo(const Path & p);
	void incPathPos();

	int distLeftOnPath() const;

	Point<int> getPathInterstion() const; //Where agent intersected first intersected with path

	void setStateSteps(int num);
	void setStateVisionLength(int num);
	void setStateVisionType(VisionType vt);
	void setStateAlgorithm(Algorithm algo);


	Algorithm getCurrAlgo() const ;

	std::string retTypeInfo() const;
	

//class Variabls
	int lives, score,speed,currSpeed; //For use in time to move	
	Point<int> pos,lastPoint; 
	AgentState currState;	
	AgentType agentType;
	Movement currDir;

	int currStateSteps; //Time in each state

	//Variable for each state {WANDER,CHASE,FLEE,DEAD} Used to init algorithm
	int stateMaxSteps[4];
	int stateVisionLength[4];
	VisionType stateVisionType[4];	
	Algorithm stateAlgo[4];	
	Color stateColor[4];

	int steps;
	int oppEaten,tokenEaten,ppEaten,fruitEaten,fruitMissed; //Round Info

	Path currPath;
	int pathPos;
	int pathDir;

	int gameIndex;//ID for GHOSTs
};
#endif

