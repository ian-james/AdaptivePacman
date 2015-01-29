#include"algo.h"

Algo::Algo(VisionType vt,int visionLength)
{
	this->vt = vt;	
	this->visionLength = visionLength;		
}
ScoreSystem Algo::makeMove(const Board &  b,const Agent & me,const Agent & pac,vector<Agent> & ghosts)
{
	vector<Point<int> > squares = b.getNeighboursWithDirection(me.pos,me.pos.oppositeDirection(me.currDir));	//Get Valid Neighbour squares exclude last Square..opposite of current direction	
	ScoreSystem randScore(me.pos, squares);

	if(squares.size() == 1)
		return randScore;
	
	randScore.randomizeTopScores();
	return randScore;
}

bool Algo::canSeePoint(Point<int> const & currPos,Point<int> const & target) const
{
	switch (vt)
	{	
	case SURROUND:
			return (currPos.manHattan(target) <= visionLength);			
		break;
	case DIRECT:
				return (currPos.getX() == target.getX() && abs(currPos.getY() - target.getY()) <= visionLength ||
					currPos.getY() == target.getY() && abs(currPos.getX() - target.getX()) <= visionLength);
		break;
	};
	
	return true;
}

bool Algo::canSeeAgent(const Agent & me,const Agent & other) const
{
	return canSeePoint(me.pos,other.pos);
}

vector<Agent> Algo::getVisibleNeibours(const Agent & me, const vector<Agent> & others) const
{
	vector<Agent> vags;
	for(unsigned int i = 0;i<others.size();i++)
	{
		if(!others[i].getIsDead() && me.gameIndex != others[i].gameIndex)
		{
			if(canSeeAgent(me,others[i]))
				vags.push_back(others[i]);
		}
	}
	return vags;
}

void Algo::removeDuplicateOpponents(vector<Agent> &vags)
{
	sort(vags.begin(),vags.end());
	vector<Agent>::iterator newEnd = unique(vags.begin(),vags.end());
	vags.erase(newEnd,vags.end()); //Delete any duplicates
}


void Algo::findClosestItem(const Point<int> & currPos, const Board & b, int &minDistToken, int & minDistPP) const
{
	int dist;
	vector<Point<int> > vec;
	queue<Point<int> > ptsList;
	ptsList.push(currPos);
	Point<int> next;	

	const int size = b.getMapRowSize()*b.getMapColSize();
	bool *beenTo = new bool[size];
	
	for(int i = 0;i<size;i++)
		beenTo[i] = false;

	beenTo[currPos.getY() * b.getMapColSize() + currPos.getX()] = true;

	minDistToken = minDistPP = 100;

	while(!ptsList.empty())
	{
		next = ptsList.front();
		ptsList.pop();

		if(b.getSquare(next) == TOKEN && minDistToken == 100)
		{
			dist = next.manHattan(currPos);
			if(dist < minDistToken)
				minDistToken = dist;
		}
		else if(b.getSquare(next) == PP && minDistPP == 100)
		{
			dist = next.manHattan(currPos);
			if(dist < minDistPP)
				minDistPP = dist;
		}

		if(minDistToken < 100 && minDistPP < 100)
		{
			delete [] beenTo;
			return;
		}

		vec = neighbourSquaresInSight(b,next,currPos);
		for (unsigned int i = 0; i<vec.size(); i++)
		{
			int arrIndex = vec[i].getY() * b.getMapColSize() + vec[i].getX();
			if(!beenTo[arrIndex])
			{
				beenTo[arrIndex] = true;
				ptsList.push(vec[i]);
			}
		}
	}

	delete [] beenTo;
}
int Algo::findClosestGhost(const Point<int>  & currPos,const  vector<Agent> &  ghosts, bool editable) const //Should be visible ghosts only
{
	int minDist = 100,dist;
	for (unsigned int i = 0; i<ghosts.size(); i++)
	{
		if(editable && ghosts[i].currState == FLEE || !editable && ghosts[i].currState != FLEE)
		{		
			dist = ghosts[i].pos.manHattan(currPos);
			if(dist < minDist)
				minDist = dist;
		}
	}
	return minDist;
}

bool Algo::ghostsInFlee(const vector<Agent> & ghosts) const
{
	for (unsigned int i = 0; i<ghosts.size(); i++)
	{
		if(ghosts[i].currState == DEAD)
			continue;

		if(ghosts[i].currState == FLEE)
			return false;
	}
	return true;
}	


//Finds the value of a path.
int Algo::computePathScore(const Board & b,const Path & p) const
{
	int score = 0;
	for (unsigned int i = 0; i<p.myPath.size(); i++)
		score += GameAttributes::Instance()->getSingleSquareCost(b.getSquare(p.myPath[i]));
	return score;
}

int Algo::computePathScoreVisible(const Board & b, const Path  & p, const Point<int> & viewPt) const
{
	int score = 0;
	for(unsigned int i = 0;i<p.myPath.size();i++)
	{
		if(canSeePoint(viewPt,p.myPath[i]))
			score += GameAttributes::Instance()->getSingleSquareCost(b.getSquare(p.myPath[i]));
	}
	return score;
}


void Algo::updatePathsFromIntersection(const Point<int> & intersection,Board &b) 
{
	//Updates all paths connected to an intersection point
	vector<Point<int> > vpts = b.getValidNeighbourSquares(intersection); //Get first steps from intersection point	

	b.pc.updateAllPaths(b, intersection, vpts);
}


double Algo::calcOneOver(double val,double force) const
{
	return (1/(val+1.0) * force);
}

int Algo::computeBoardScore(const Board & b,const Agent & me) const 
{
	int score =0;
	Point<int> cp;	
	
	for(int i = 0;i<b.getMapRowSize();i++)
	{
		for(int y = 0;y<b.getMapColSize();y++)
		{
			cp = Point<int>(y,i);	
			if(canSeePoint(me.pos,cp))			
				score += GameAttributes::Instance()->getSingleSquareCost(b.getSquare(cp));			
		}
	}
	return  score;
}


vector<Point<int> > Algo::neighbourSquaresInSight(const Board & b,const Point<int> & currPos, const Point<int>  &  visionPoint) const
{
	Movement myArray[] = {UP,DOWN,LEFT,RIGHT};

	Point<int> dp;
	vector<Point<int> > vp;
	for (unsigned int i = 0; i<4; i++)
	{	
		dp = currPos;
		dp.updatePointInDirection(myArray[i]);

		if(b.validSquare(dp) && b.getSquare(dp) != WALL && canSeePoint(currPos,visionPoint))
			vp.push_back(dp);
	}	
	return vp;
}

