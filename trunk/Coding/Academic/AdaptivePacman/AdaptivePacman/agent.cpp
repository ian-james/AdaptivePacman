#include"agent.h"
#include"scoreSystem.h"

Agent::Agent(AgentType at)
{
	agentType = at;	
	speed = 3; 
	score = steps = currSpeed = 0;	
	lives = 3;	
	currState = WANDER;		
	oppEaten=tokenEaten=ppEaten=fruitEaten=fruitMissed = 0;
	
	pathPos = 0;
	pathDir = 0;
	gameIndex=(at == PACMAN)?-1:1;

	currDir = NONE;
	currStateSteps = 0;	

	for(int i = 0;i<4;i++)
	{
		stateAlgo[i] = RAND;	
		stateVisionLength[i] = 3;
		stateMaxSteps[i]=15;
		stateVisionType[i] = OMNI;	
		stateColor[i].setColorStr("red",false);
	}
	
}
Agent::Agent(AgentType at,Point<int> pos,int speed)
{
	agentType = at;	
	this->pos = pos;
	this->speed = speed;
	speed = 3; 
	score = steps = currSpeed = 0;	
	lives = 3;
	currDir = NONE;
	currStateSteps = 0;

	for(int i = 0;i<4;i++)
	{
		stateAlgo[i] = RAND;
		stateVisionLength[i] = 3;
		stateMaxSteps[i]=15;
		stateVisionType[i] = OMNI;		
		stateColor[i].setColorStr("red",false);
	}
}

Agent::~Agent()
{
}

bool Agent::operator<(const Agent & a) const
{
	return this->pos < a.pos;
}

bool Agent::operator==(const Agent & a) const
{
	return (a.pos == this->pos && a.currState == this->currState);
}


istream& operator>>(istream &in,Agent & g)
{
	int agt; 
	in >> agt; g.agentType = AgentType(agt);
	in >> g.speed >> g.lives;
	
	string color;
	int ialgo, ivisionType;
	for(int i = 0;i<4;i++)
	{
		in >> ialgo >> g.stateVisionLength[i] >> g.stateMaxSteps[i] >> ivisionType >> color;
		
		g.stateAlgo[i] = Algorithm(ialgo);
		g.stateVisionType[i] = VisionType(ivisionType);	
		g.stateColor[i].setColorStr(color,false);	
	}

	return in;
}

ostream& operator<<(ostream &out,const Agent& g)
{
	out << int(g.agentType) << " " << g.speed << " " <<  g.lives << "\n";

	for(int i = 0;i<4;i++)
	{
		out << " " << int(g.stateAlgo[i]) << " " << g.stateVisionLength[i] << " " << g.stateMaxSteps[i] << " " << int(g.stateVisionType[i]) << " " 
			 << g.stateColor[i].getColorString(false) << "\n";		
	}

	return out;
}

bool Agent::myMove() //Return whether it's your move
{
	if(currSpeed >= speed)
	{
		currSpeed = 0;
		return true;
	}
	currSpeed++;
	return false;
}

Point<int> Agent::chooseMove(const ScoreSystem &movesScore)
{
	//Choose moves based on score and probabilty.
	return movesScore.scoreSystem.front().pt;	
}

bool Agent::getIsDead() const
{
	return (currState == DEAD);
}
void Agent::incSteps(int t)
{
	steps += t;
}
void Agent::autoDecrementCurrLives()
{
	lives--;
}

void Agent::setCurrentState(AgentState ags)
{	
	currState = ags;
	currStateSteps = 0;
}

Color Agent::getStateColor() const
{
	return stateColor[currState];
}

void Agent::reset(bool decrementLife)
{
	currSpeed = 0;
	score = 0;
	currState = WANDER;
	currStateSteps = 0;
	steps = 0;
	oppEaten = tokenEaten = ppEaten = fruitEaten = fruitMissed = 0; 

	if(this->isPacman())
		pos = lastPoint = Point<int> (9,12);
	else
		pos = lastPoint = Point<int> (10,6);

	currDir = NONE;	
	pathPos = 0;
	pathDir = 0;
	currPath = Path();

	if(decrementLife)
		autoDecrementCurrLives();
}

string Agent::getAgentStr() const
{
	if(agentType == PACMAN)
		return "PAC";
	return ("GH" + toString(gameIndex));
}

void Agent::setNextPos(const Point<int> & nextPos)
{
	currDir = pos.setDirection(nextPos);
	lastPoint = pos;
	pos = nextPos;	
}

bool Agent::isPacman() const
{
	return (agentType == PACMAN);
}

bool Agent::incAgentState() //Returns whether to take your turn
{
	if(!myMove())
		return false;
	
	currStateSteps++;

	if(currStateSteps >= stateMaxSteps[currState])	
		setCurrentState(WANDER);	

	if(getIsDead())	
		return false;		

	steps++;

	return true;
}

bool Agent::shouldChase() const
{
	if(currState == DEAD || gameIndex < 0)
		return false;

	return (currState == WANDER || currState == CHASE);
}

void Agent::setPathInfo(const Path & p)
{
	currPath = p;
	pathDir = (p.myPath[1] == pos)?1:-1; //set direction to 1 (forward) or -1 (backward) traversal of path.
	pathPos = (pathDir == 1)?1:(p.myPath.size()-2); //set to the first step in either direction
}

void Agent::incPathPos()
{
	if(!currPath.isEmpty())
	{
		pathPos += pathDir;
		Point<int> pt = currPath.getPoint(pathPos);
	
		if(!pt.isBad())
			setNextPos(pt);	
	}
}

int Agent::distLeftOnPath() const
{
	if(pathDir > 0)
		return currPath.myPath.size() - pathPos - 1;
	return pathPos;
}

Point<int> Agent::getPathInterstion() const
{
	if(pathDir > 0)
		return currPath.myPath.front();
	return currPath.myPath.back();
}

void Agent::setStateSteps(int num)
{
	for(int i = 0;i<4;i++)
		stateMaxSteps[i] = num;
}
void Agent::setStateVisionLength(int num)
{
	for(int i = 0;i<4;i++)
		stateVisionLength[i] = num;
}
void Agent::setStateVisionType(VisionType vt)
{
	for(int i = 0;i<4;i++)
		stateVisionLength[i] = vt;
	
}
void Agent::setStateAlgorithm(Algorithm algo)
{
	for(int i = 0;i<4;i++)
		stateAlgo[i] = algo;
}


Algorithm Agent::getCurrAlgo() const 
{
	if(isPacman())
		return stateAlgo[WANDER]; //Always wander,unless multi-algo
	return stateAlgo[currState];
}

