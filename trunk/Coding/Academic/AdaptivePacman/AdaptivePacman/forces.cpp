#include"forces.h"
#include"gameAttributes.h"

Forces::Forces(VisionType vt,int visionLength) : Algo(vt,visionLength)
{
}
ScoreSystem Forces::makeMove(const Board & b,const Agent & me, const Agent & pac,vector<Agent> & ghosts)
{
	if(me.agentType == PACMAN)
		return playerForces(b,pac,ghosts);
	return this->ghostForces(b,me,pac,ghosts);
}

ScoreSystem Forces::playerForces(const Board & b, const Agent &pac, vector<Agent> & ghosts)
{
	//Filters out the last move, unless at intersection
	Movement movem = pac.pos.oppositeDirection(pac.currDir);
	vector<Point<int> > vpts = b.getNeighboursWithDirection(pac.pos,movem);

	if(vpts.size() == 1) //If we aren't at intersection point, continue path. Ie No Decision
		return ScoreSystem(pac.pos,vpts);

	double score = 0;
	ScoreSystem moves(pac.pos,vpts);
	vector<Agent> flock = getVisibleNeibours(pac,ghosts);

	Point<int> avgItemPos = this->getAveragePosItem(b,pac.pos);
	for( unsigned int i = 0;i<vpts.size();i++)
	{
		score = 0;
		score += distanceToItems(b,vpts[i],flock,true); //Towards closest item
		score += towardsGhost(vpts[i],ghosts);  //Towards editable ghosts and away from none editable
		score += (directionGhostGroup(vpts[i],ghosts) * GameAttributes::Instance()->awayGhostCenter);		//Away from center of ghosts or towards same value
		score += directionItemGroup(vpts[i],avgItemPos,true); //Towards center of items		

		//Penalty for same direction
		if(pac.pos.getPointInDirection(movem) == vpts[i])
			score -= 0.20;	

		Path path = b.pc.getPath(pac.pos,vpts[i]);	
		Point<int> pathEnd = path.getPathEnd(pac.pos);

		if(canSeePoint(pac.pos,pathEnd))
		{
			score += distanceToItems(b,pathEnd,flock,true); //Towards closest item
			score += towardsGhost(pathEnd,ghosts);
			score += (directionGhostGroup(pathEnd,ghosts) *  GameAttributes::Instance()->awayGhostCenter);
			score += directionItemGroup(pathEnd,avgItemPos,true);		
			
			score += path.score/120; 
		}

		moves.addPointScore(vpts[i],score);
	}

	moves.randomizeTopScores();
	return moves;
}
double Forces::distanceToItems(const Board &b,const Point<int> & testPos, vector<Agent> & ghosts, bool isPacman) const//Computes score for Token/PP/FRUIT and Area value
{	
	double score = 0;
	int minDistToken = 0,minDistPP = 0;

	double tokenForce,ppForce,fruitForce;
	
	if(isPacman)
	{
		tokenForce = GameAttributes::Instance()->tokenForce;
		ppForce = GameAttributes::Instance()->ppForce;
		fruitForce = GameAttributes::Instance()->fruitForce;
	}
	else
	{
		tokenForce = GameAttributes::Instance()->forceProtectToken;
		ppForce = GameAttributes::Instance()->forceProtectPP;
		fruitForce = GameAttributes::Instance()->forceProtectFruit;
	}


	findClosestItem(testPos,b,minDistToken,minDistPP);	

	score = calcOneOver(minDistToken,tokenForce);

	//Check if already eat PP
	if(ghostsInFlee(ghosts))
		score += calcOneOver(minDistPP,ppForce);

	if(b.fruit.visible)
		score += calcOneOver(b.fruit.pos.manHattan(testPos),fruitForce);

	return score;	
}
double Forces::towardsGhost(const Point<int> & testPos,vector<Agent> & ghosts) //Calculates score for closest ghosts + or - if editable
{
	int badsDistance = findClosestGhost(testPos,ghosts,false); //Attackings
	int goodDistance = findClosestGhost(testPos,ghosts,true); //Editable

	//TODO: Set priority for whether to chase or hide from ghost
	if(goodDistance+1 < badsDistance)
		return calcOneOver(goodDistance,GameAttributes::Instance()->towardsEdGhosts);

	return -calcOneOver(badsDistance,GameAttributes::Instance()->awayGhosts);	
}

double Forces::awayFromGhost(const Point<int> & testPos, vector<Agent> & ghosts) //For Ghost to move away from other ghosts
{
	double score = 0;
	for (unsigned int i = 0; i<ghosts.size(); i++)
		score += calcOneOver(testPos.manHattan(ghosts[i].pos),GameAttributes::Instance()->forceAwayGhost);
	return -score; // We want to move away from each other
}

double Forces::directionGhostGroup(const Point<int> & testPos,vector<Agent> & ghosts) // Towards centroid of group
{
	if(ghosts.empty())
		return 0.0;

	Point<int> avgPos = getAveragePos(ghosts);
	return -calcOneOver(testPos.manHattan(avgPos));
}

Point<int> Forces::getAveragePos(const vector<Agent> & flock) const
{
	Point<int> pos;	
	for (unsigned int i = 0; i<flock.size(); i++)
		pos = pos + flock[i].pos;		
	return pos.divideByScalarRound(flock.size());
}

Point<int> Forces::getAveragePosItem(const Board &b, const Point<int> & currPos) const
{
	Point<int> avg;
	Point<int> next;
	int count = 0;
	for( int i = 0;i<b.getMapRowSize();i++)
	{
		for(int x = 0;x<b.getMapColSize();x++)
		{
			next.setPoint(i,x);
			if(canSeePoint(currPos,next))
			{
				if(b.getSquare(next) == TOKEN || b.getSquare(next) == PP)
				{
					avg = avg + next; //setPoint(avg.getX() + x,avg.getY() + i);
					count++;
				}
			}
		}
	}
	
	if(count > 0)
		return avg.divideByScalarRound(count);
	return avg;
}

double Forces::directionItemGroup(const Point<int> & testPos, const Point<int> & avgItemPos,bool isPacman)
{	
	double t = 0.0;
	if(isPacman)
		t = GameAttributes::Instance()->towardItemCenter;
	else
		t = GameAttributes::Instance()->forceItemCenter;

	return calcOneOver(testPos.manHattan(avgItemPos), t);
}

/******************************GHOST Section ***************************************/


ScoreSystem Forces::ghostForces(const Board &b,const Agent &me, const Agent &pac, vector<Agent> & ghosts)
{
	Movement movem = me.pos.oppositeDirection(me.currDir);
	vector<Point<int> > vpts = b.getValidNeighbourSquares(me.pos,movem);

	if(vpts.size() == 1) //If we aren't at intersection point, continue path. Ie No Decision
		return ScoreSystem(me.pos,vpts);

	double score = 0;
	ScoreSystem moves(me.pos,vpts);
	vector<Agent> flock = getVisibleNeibours(me,ghosts);
	Point<int> avgItemPos = getAveragePosItem(b,me.pos);

	bool canSeePacman = canSeePoint(me.pos,pac.pos);

	for (unsigned int i = 0; i<vpts.size(); i++)
	{
		score = 0;

		if(me.pos.getPointInDirection(movem) == vpts[i])
			score -= 0.20;

		//Towards Pacman
		if(canSeePacman)
		{
			score = getDistanceToPacman(me,vpts[i],pac);
			
			//Towards PacmanDirection
			score += pacDirection(me,vpts[i],pac);
		}
		
		//Towards Editable Ghosts and away from None	
		score += awayFromGhost(vpts[i],flock);  //Negate as default behaviour is for pacman not a ghost.

		//Away from Center of Ghosts
		score += directionGhostGroup(vpts[i],flock) * GameAttributes::Instance()->forceTeamPos;		//Away from center of ghosts

		//TODO Check values below here make sense
		bool addedItemInfo = false;
		Path path = b.pc.getPath(me.pos,vpts[i]);

		Point<int> refPoint = vpts[i]; //Either pathEnd point or next point on path
		Point<int> pathEnd;

		if(!path.isEmpty())
		{
			pathEnd = path.getPathEnd(me.pos);			

			if(canSeePoint(me.pos,pathEnd))
				refPoint = pathEnd;
		}

		if(canSeePacman)
		{		
			if(!path.isEmpty() && path.pointOnPath(pac.pos) /* && no ghost on path */)
			{
				score += 1.0;
				score += pacDirection(me,refPoint,pac); //PacEnd to Pac nextPos based on direction only
			}
			else //He's not on this possible path.
			{				
				score += getDistanceToPacman(me,refPoint,pac);
				score += pacDirection(me,refPoint,pac); //PacEnd to Pac nextPos based on direction only				
			}			
		}
		else
		{		
			score += directionItemGroup(refPoint,avgItemPos,false); //Towards center of items		
			score += distanceToItems(b,refPoint,ghosts,false);  //Towards closest Items
			//ToDo consider pathScore to itemDistance
		}	
		
		moves.addPointScore(vpts[i],score);
	}

	moves.randomizeTopScores();
	return moves;

}

double Forces::getDistanceToPacman(const Agent &me, const Point<int> & testPos, const Agent &pac) const	//Considers if can see Pacman,Editable
{
	if(!canSeeAgent(me,pac)) //Can't see pacman return no score
		return 0.0;	

	double score = calcOneOver(testPos.manHattan(pac.pos) * GameAttributes::Instance()->forcePacPos);

	if(me.currState == FLEE)
		return -score;
	return score;
}
double Forces::getDistanceFromGhosts(const Agent & me,const Point<int> &testPos, vector<Agent> & flock) const
{
	return 0.0;
}
double Forces::protectPieces(const Agent &me, vector<Agent> & flock) const
{
	return 0.0;
}
double Forces::getTeamScore(const Agent &me, vector<Agent> & flock) const
{
	return 0.0;
}
double Forces::pacDirection(const Agent & me,const Point<int> & testPos, const Agent &pac) const
{
	if(!canSeeAgent(me,pac)) //Can't see pacman return no score
		return 0.0;

	Point<int> guessPacNext = pac.pos;
	guessPacNext.updatePointInDirection(pac.currDir);
	double score = calcOneOver(testPos.manHattan(guessPacNext),GameAttributes::Instance()->forcePacDir);

	if(me.currState == FLEE)
		return -score;
	return score;

}

