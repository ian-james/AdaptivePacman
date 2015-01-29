#include"flock.h"
#include"gameAttributes.h"

Flock::Flock(VisionType vt,int visionLength) : Algo(vt,visionLength)
{
}
ScoreSystem Flock::makeMove(const Board & b,const Agent & me, const Agent & pac,vector<Agent> & ghosts)
{
	return flockingAlgorithm(b,me,pac,ghosts);
}

Point<int> Flock::getAveragePos(const vector<Agent> &  flock) const //Pass in ignore current GameIndex;
{
	Point<int> pos;	
	for (unsigned int i = 0; i<flock.size(); i++)
		pos = pos + flock[i].pos;		
	return pos.divideByScalarRound(flock.size());
}

Point<int> Flock::getAverageAlignment(const vector<Agent> & flock) const
{
	//Alignment would be nextSquare on path or continuing direction. Otherwise direction of pacman if we can see) if not random next possible square
	Point<int> alignPos;
	for (unsigned int i = 0; i<flock.size(); i++)
	{
		Point<int> lastPos = flock[i].lastPoint;
		lastPos.updatePointInDirection(lastPos.setDirection(flock[i].pos));		
		alignPos = alignPos + lastPos; //Add Direction ghost is heading in ...not objective
	}
	
	return alignPos.divideByScalarRound(flock.size());	
}

double Flock::getAverageSeperation(const Point<int> & testPos,vector<Agent> & flock) const
{
 	double score = 0;
	for (unsigned int i = 0; i<flock.size(); i++)
	{
		int seperation = flock[i].pos.manHattan(testPos);		
		score += bellCurve(GameAttributes::Instance()->maxSeperation,seperation); // 0 and maxSeperation receive low values anything with range receives positive...outside maxSeperation < 0
	}

	score /= double(flock.size()); //normalize
	return score;
}

double Flock::getSeperationFromPacman(AgentState currState,const Point<int> & pacPos, const Point<int> & testPos)  const
{
 	if(currState == FLEE)
			return -1.0/(testPos.manHattan(pacPos) + 1.0); //Score higher for moving away if in flee	
	return 1.0/(testPos.manHattan(pacPos) + 1.0); //Increase score for moving towards editable pacman
}

ScoreSystem Flock::flockingAlgorithm(const Board &b, const Agent &me,const Agent &pac,vector<Agent> &ghosts)
{
	
	vector<Point<int> > squares = b.getValidNeighbourSquares(me.pos,me.pos.oppositeDirection(me.currDir));	//Get Valid Neighbour squares exclude last Square..opposite of current direction
	ScoreSystem moves(me.pos,squares);

	if(squares.size() == 1)
		return moves;

	double score;	
	vector<Agent> flock = getVisibleNeibours(me,ghosts);

	if(flock.empty()) //Check if we can't see any of our flock, random default
		return moves;

	Point<int> avgPos = getAveragePos(flock); //Get Average position of flock
	Point<int> avgAli = getAverageAlignment(flock); //get average alignment of flock
	
	bool canSeePacman = canSeeAgent(me,pac);

	GameAttributes * ga = GameAttributes::Instance();

	for (unsigned int i = 0; i<squares.size(); i++)
	{
		score = 0;		
		if(!flock.empty())
		{
			score +=  (1.0/(squares[i].manHattan(avgPos) + 1.0) + ga->cohForce); // Calculate score for distance based on mono descending from Average position
			score +=  (1.0/(squares[i].manHattan(avgAli) + 1.0) + ga->alignForce); // Calculate score for alignment similarly.
			score += (getAverageSeperation(squares[i],flock) * ga->seperationForce);	
		}
		if(canSeePacman)	
			score += (getSeperationFromPacman(me.currState,pac.pos, squares[i]) + ga->hungerForce); //Only done if you can see pacman		

		//TODO: Any additional forces could go here, if desired.
		moves.addPointScore(squares[i],score);
	}

	moves.randomizeTopScores();
	return moves;
}

double Flock::bellCurve(double maxiVal,double x) const
{
	double a = -4/(maxiVal*maxiVal), b = -a*maxiVal;
	return (a*x*x + b*x);
}



