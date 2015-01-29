#include"SSSAB.h"
#include"gameAttributes.h"
#include"agent.h"
#include"board.h"
#include "logger.h"

SSSAB::SSSAB(VisionType vt,int visionLength) : Algo(vt,visionLength)
{
	//Initial Vision point still applies..check moves squares vs original vision
	//Remove duplicate location ghosts wasted computation
	//Check if player runs out of visible squares	
}

ScoreSystem SSSAB::makeMove(const Board & b,const Agent & me, const Agent & pac,vector<Agent> & ghosts)
{
	vector<Point<int> > vpts = b.getValidNeighbourSquares(me.pos,me.pos.oppositeDirection(me.currDir));
	pacOrig = me.pos;	

	if(me.isPacman() && vpts.size() != 1)			
	{
		ScoreSystem score = sss(b,pac,this->getVisibleNeibours(me,ghosts));
		if(!score.isEmpty()) //Checks for cases like we end next turn, we've won
			return score;
	}

	return ScoreSystem(vpts);	
}

ScoreSystem SSSAB::sss(Board b,Agent ply, vector<Agent> ghosts)
{
	double g = double(INT_MAX-1.0),val;
	//Get Flock of visible ghosts to send in to algorithim. If using for ghosts check if can see pacman...if not Random decision?
	map<Node<double>,double> table;
	Node<double> n;
	vector<Point<int> > path;

	//Set Score to 0, so it can be accumulated, set speed so it's pacman's turn.
	int oldScore = ply.score;
	ply.score = 0;
	ply.currSpeed = ply.speed;
	
	do
	{		
		val = g;
		g = sssab(val-1,val,0,table,n,b,ply,ghosts,path);

	}while(val != g);	
	
	ScoreSystem moves,extra;
	bool found = false;	
	for(map<Node<double>,double>::iterator ip = table.begin(); ip != table.end() && ip->first.retPathSize() < 2;ip++)
	{
		if(ip->first.retPathSize() == 1)
		{
			if(ip->first.getMaxi() == ip->first.getMini())
			{			
				moves.addPoint(ip->first.retPathIndex(0),ip->second);
				found = true;
			}
			else
				extra.addPoint(ip->first.retPathIndex(0),ip->second);

		}
	}

	for (unsigned int k = 0; k <extra.scoreSystem.size(); k++)
	{
		if(!found)
			moves.addPoint(extra.scoreSystem[k].pt,extra.scoreSystem[k].score);
		else
		{
			bool higher = false;
			for (unsigned int m = 0; m < moves.scoreSystem.size() && !higher; m++)
			{
				if(extra.scoreSystem[k].score > moves.scoreSystem[m].score)
				{
					higher = true;
					moves.addPoint(extra.scoreSystem[k].pt,extra.scoreSystem[k].score);					
				}
			}
		}
	}
	
	ply.score = oldScore;

	moves.randomizeTopScores();	

	return moves;
}

double SSSAB::sssab(double alpha,double beta,int depth, map<Node<double>,double> &table, Node<double> n,Board b,Agent pac,vector<Agent> ghosts, vector<Point<int> > &path)
{	
	map<Node<double>,double>::iterator it = table.find(n);
	if(it != table.end()) //Check to see if we've already been to this node and should get exit
	{
		if(it->first.getMaxi() <= alpha || it->first.getMaxi() == it->first.getMini())
			return it->first.getMaxi();

		if(it->first.getMini() >= beta)
			return it->first.getMini();
	}	

	double maxi = double(INT_MAX);
	bool simmedGame = false;
	//Simulate Game results, if any	

	if(table.size() < MAX_TABLE_SIZE && !isSimFinished(b,pac,ghosts,depth,n,maxi))
	{	
		removeDuplicateOpponents(ghosts); //Remove based on duplicate position
		int nextAgentTurn = this->simForcedMoves(b,pac,ghosts,maxi,n);

		bool done = (nextAgentTurn==-2);
		if(!done)
		{
			Agent *active = &pac;

			if(nextAgentTurn >= 0) 
				active = &ghosts[nextAgentTurn];
			else
			{
				pac.steps++;
				depth++;
				ghosts = excludeGhosts(b,pac,ghosts); //Remove extra ghosts				
			}		

			if(!isSimFinished(b,pac,ghosts,depth,n,maxi))
			{		
				simmedGame = true;
				vector<Point<int> > vpts = setupMoveChoices(b,active,maxi,pac.pos);		

				double val;
				Point<int> lastPos = active->pos;
				vector<Point<int> > origPath = path; //Store original path, as may grow by more than one
				bool movedOnce = false;
				for(unsigned int i = 0;i < vpts.size() && table.size() < MAX_TABLE_SIZE && ((active->isPacman() && (maxi < alpha)) || (!active->isPacman() && (maxi >= beta)));i++)
				{
					//TODO: make sure pacman can see the point going to, handle case where he can't so estimate solution
					if(canSeePoint(this->pacOrig,vpts[i]))
					{
						movedOnce = true;

						active->setNextPos(vpts.at(i));

						path.push_back(vpts.at(i));

						n.setPath(path);					
						val = sssab(alpha,beta,depth,table,n,b,pac,ghosts,path);
						active->pos = lastPos; //Because lastPos was reset in nextPos

						path = origPath;

						if(active->isPacman())
						{
							maxi = max(val,maxi);
							alpha = max(maxi,alpha);
						}
						else
						{
							maxi = min(val,maxi);
							//beta = min(maxi,beta);
						}
					}
				}

			
			
				if(maxi <= beta)			
					n.setMaxi(maxi);			
				if(maxi >= alpha)
					n.setMini(maxi);
			}
		}
	}

	n.setPath(path);

	if(it == table.end())
		table.insert(pair<Node<double>,double> (n,maxi));
	else
	{
		//Hack in order to change key, although still equivalent gets rid of const problem
		table.erase(it);
		table.insert(pair<Node<double>,double> (n,maxi));		
	}
	return maxi;
}

int SSSAB::getNextPlayerTurn(Agent &p, vector<Agent> &ghosts)
{
	int playDiff = p.speed - p.currSpeed;

	int gLow = 0, gmin = 1000000;

	for(unsigned int y = 0;y<ghosts.size();y++)
	{
		//If ghost is dead or outside of interest range skip
		if(ghosts[y].currState == DEAD)// || GameAttributes::Instance()->ignoreDistance < p.pos.manHattan(ghosts[i].pos))
				continue;

		if(ghosts[y].speed - ghosts[y].currSpeed < gmin)
		{
			gmin = ghosts[y].speed - ghosts[y].currSpeed;
			gLow = y;
		}

	}	

	int minSpeed = min(playDiff,gmin);

	if(minSpeed != 0)
	{
		for(unsigned int y = 0;y<ghosts.size();y++)
			ghosts[y].currSpeed += max(minSpeed,1);

		p.currSpeed += max(1,minSpeed);
	}

	if(playDiff == minSpeed) //Player moves ahead of all others
	{
		p.currSpeed = 0;
		return -1;
	}

	ghosts[gLow].currSpeed = 0;	
	return gLow;
}

SSSAB::SIMSTATES SSSAB::simGame(Board &b,Agent &ply, vector<Agent> &ghosts)
{
	if(ply.steps == GameAttributes::Instance()->MAXSTEPS)
		return MAX_SIM_STEPS;

	//Player Collision with Item
	handleItemCollision(b,ply,ghosts);	

	//Check Level Complete
	if(b.levelComplete())
		return LEVEL_COMPLETE;

	if(handleAllAgentCollision(ply,ghosts))	
		return PLAYER_DEATH;

	return CONTINUE;
}

vector<Point<int> >  SSSAB::setupMoveChoices(Board const &b, Agent *active, double &maxi, Point<int> pacPos)
{
	vector<Point<int> > vpts = b.getNeighboursWithDirection(active->pos,active->pos.oppositeDirection(active->currDir));

	if(active->isPacman())
	{
		maxi = double(INT_MIN+1);		
		sortByValue(vpts,b);
	}
	else
	{
		maxi = double(INT_MAX);		
		sortByDistance(vpts,pacPos);
	}

	vpts = this->inOriginalSight(vpts);
	return vpts;
}

bool SSSAB::isSimFinished(Board &b,Agent &ply, vector<Agent> &ghosts,int depth, Node<double> & n,double &maxi,bool visionFinished )
{
	SIMSTATES sim = simGame(b,ply,ghosts);

	if(sim == CONTINUE && visionFinished)
		sim = this->VISION_END;

	switch(sim)
	{
	case LEVEL_COMPLETE:
		maxi = GameAttributes::Instance()->successVal + ply.score;
		break;		
	case PLAYER_DEATH:
		maxi = -GameAttributes::Instance()->pacmanVal + ply.score;
		break;
	case MAX_SIM_STEPS:
		maxi = ply.score;
		break;
	case VISION_END:
		maxi = computeGameScore(b,ply);
		break;
	case CONTINUE:	
		if(depth > GameAttributes::Instance()->depthOfSearch)		
			maxi = computeGameScore(b,ply);
		else 
			return false;
		break;	}

	n.setMaxi(maxi);
	n.setMini(maxi);	

	return true;
}

double SSSAB::closestItemScore(const Point<int> & currPos,const Board  &b) const
{
	int dist;
	vector<Point<int> > vec;
	queue<Point<int> > ptsList;
	Point<int> next;	

	ptsList.push(currPos);
	int minDistToken = 100, minDistPP = 100;

	const int size = b.getMapRowSize()*b.getMapColSize();
	vector< vector<bool> > beenTo;
	
	for(int i = 0;i<b.getMapRowSize();i++)
	{
		vector<bool> vb;
		for(int y = 0;y<b.getMapColSize();y++)
			vb.push_back(false);
		beenTo.push_back(vb);
	}
	

	beenTo[currPos.getY()][currPos.getX()] = true;

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

		if(minDistToken < 100 || minDistPP < 100)
			break;

		vec = neighbourSquaresInSight(b,next,currPos);
		for (unsigned int i = 0; i<vec.size(); i++)
		{			
			if(!beenTo[vec[i].getY()][vec[i].getX()])
			{
				beenTo[vec[i].getY()][vec[i].getX()] = true;
				ptsList.push(vec[i]);
			}
		}
	}
	
	//Total distance setup for two points
	int total = b.getMapRowSize()+b.getMapColSize();

	//We shouldn't be longer than board, otherwise we didn't find either...odd
	double bestDist = std::min(total,std::min(minDistToken, minDistPP));	

	double val = 1.0 - (bestDist/(double)total);	
	if(val < 0)
		val = 0;

/*
	std::string vals = "Value of closest Item is "  + toString(val);
	Logger *log = Logger::Instance();
	log->writeToFile(vals); 
*/

	
	return val;
}

double SSSAB::computeGameScore(const Board & b, const Agent & ply) const
{
	//Trying to just maintain score as we go, as opposed to computing maxScore	
	return ply.score + closestItemScore(ply.pos,b);	
}


void SSSAB::handleItemCollision(Board &b,Agent & pac,vector<Agent> &bads)
{
	Pieces pval = b.getSquare(pac.pos);

	if(pval == WALL || pval == EMPTY)
		return ;		

	if(pval == PP)
	{	
		for (unsigned int i = 0; i<bads.size(); i++)
		{
			if(bads[i].currState != DEAD)
				bads[i].setCurrentState(FLEE);		
		}
	}	

	//Count Score
	int temp = GameAttributes::Instance()->getSingleSquareCost(pval);	
	pac.score += temp;
	b.setSquare(pac.pos,EMPTY);
	b.emptySquares.push_back(pac.pos);

	if(b.fruit.visible && pac.pos == b.fruit.pos)
	{
		pac.score += b.fruit.pointsPerItem;		
		b.fruit.visible = false;
	}
}

bool SSSAB::handleAllAgentCollision(Agent &ply,vector<Agent> &bads)
{ 
	for (unsigned int i = 0; i<bads.size(); i++)
	{
		if(bads[i].currState != DEAD)
		{
			handleAgentCollision(ply,bads[i]);
			if(ply.currState == DEAD)			
				return true;			
		}
	}
	return false;
}

void SSSAB::handleAgentCollision(Agent &pac,Agent &ghost)
{
	if(pac.pos == ghost.pos)
	{
		if(ghost.currState == FLEE)
		{	
			ghost.setCurrentState(DEAD);
			ghost.pos = GameAttributes::Instance()->g[ghost.gameIndex].pos;
			pac.score  +=  GameAttributes::Instance()->ghostVal;
		}		
		else			
			pac.setCurrentState(DEAD);	
	}	
}

int SSSAB::simForcedMoves(Board &b, Agent &pac, vector<Agent> &bads,double &maxi,Node<double> &n)
{	
	int nextAgentTurn = getNextPlayerTurn(pac,bads);
		
		//Set Active Player
	Agent *active = &pac;
	if(nextAgentTurn >= 0) 
		active = &bads[nextAgentTurn];		

	vector<Point<int> > vpts = setupMoveChoices(b,active,maxi,pac.pos);		
	while(vpts.size() < 2)
	{	
		//if(!vpts.empty())
		if(!vpts.empty())
			active->setNextPos(vpts[0]);
		else
		{
			if(isSimFinished(b,pac,bads,0,n,maxi,true) || active->isPacman())
				return -2;
			else
				excludeSpecificGhost(bads,nextAgentTurn);
		}
		//path.push_back(vpts[0]);

		if(isSimFinished(b,pac,bads,0,n,maxi)) //Depth = Zero because depth will actually be checked outside of here
			return -2;
		
		nextAgentTurn = getNextPlayerTurn(pac,bads);
		
		//Set Active Player
		active = &pac;
		if(nextAgentTurn >= 0) 
			active = &bads[nextAgentTurn];		

		vpts = setupMoveChoices(b,active,maxi,pac.pos);			
	}
	return nextAgentTurn;
}

void SSSAB::outputPath(const vector<Point<int> > & path, double score) const
{
	for (unsigned int i = 0; i<path.size(); i++)
		cout << " " << path[i].toString();
	
}

void SSSAB::sortByDistance(vector<Point<int> > &vpts, const Point<int> & pacPos)
{
	ScoreSystem scores(vpts);	

	for (unsigned int i = 0; i<vpts.size(); i++)
		scores.addPointScore(vpts[i],pacPos.manHattan(vpts[i]));

	scores.randomizeTopScores();

	vpts = scores.getMoves();
}

void SSSAB::sortByValue(vector<Point<int> > &vpts, const Board & b)
{
	ScoreSystem scores(vpts);

	GameAttributes *ga = GameAttributes::Instance();
	for (unsigned int i = 0; i<vpts.size(); i++)
		scores.addPointScore(vpts[i],ga->getSingleSquareCost(b.getSquare(vpts.at(i))));

	scores.randomizeTopScores();
	vpts = scores.getMoves();
}

vector<Agent> SSSAB::opponentsInDistance(const Board &b, const Agent &pac,const vector<Agent> &ghosts) const
{
	//Exclude ghosts that can't no be gotten to in path moves

	vector<Point<int> > neighs = b.getValidNeighbourSquares(pac.pos);
	vector<Path> paths = b.pc.getAllPathFromPoint(pac.pos,neighs);

	int errorMoves = 1; // SHould be min of 1 as ghosts go after, could be higher to allow a little more room
	vector<Agent> flock;
	vector<bool> inFlock;
	int maxSize =  ghosts.size();

	for(short j = 0;j<maxSize;j++)
		inFlock.push_back(false);

	for (unsigned int i = 0; i<paths.size() && maxSize > 0; i++)
	{
		Point<int> pathEnd = paths[i].getPathEnd(pac.pos);
		int pathLength = paths[i].getPathLength()-1;

		for (unsigned int y = 0; y<ghosts.size() && maxSize > 0; y++)
		{
			if(!inFlock[y] )
			{
				if(ghosts[y].currPath.isEmpty() )
				{
					flock.push_back(ghosts[y]);
					maxSize--;
					inFlock[y] = true;
				}
				else if(pac.pos.manHattan(ghosts[y].pos) < 12)
				{
					Point<int> ghostEnd = ghosts[y].currPath.getPathEnd(ghosts[y].getPathInterstion());
					int ghDist = ghosts[y].distLeftOnPath();

					if(pathLength+1 >= ghDist + ghostEnd.manHattan(pathEnd)) // Close enough to final point to affect outcome of game 
					{					
							flock.push_back(ghosts[y]);
							maxSize--;
							inFlock[y] = true;
					}
				}
			}			
		}
	}

	return flock;
}

//Examines only in following on path.
bool SSSAB::noGhostOnPath(const Board &b,const Point<int> & intersect, const Point<int> & first, const vector<Agent> &flock) const
{
	Path p = b.pc.getPath(intersect,first);
	
	for (unsigned int i = 0; i<flock.size(); i++)
	{
		if(flock[i].currPath == p)
		{
			if(flock[i].currPath.myPath[1] == p.myPath[1]) //Check if you're going the same direction on path
				return true;
		}
	}
	return false;
}

vector<Agent> SSSAB::excludeGhosts(const Board &b,const Agent &pac, const vector<Agent> & ghosts) const
{	
	//noGhostOnPath
	//cout << "Start Excluding\n";

	//Check if ghost is currently visible to start point
	vector<Agent> ans;
	for (unsigned int i = 0; i<ghosts.size(); i++)
	{
		if(canSeePoint(this->pacOrig,ghosts[i].pos) )
		{
			if(!findPath(ghosts[i].currPath,ans))
				ans.push_back(ghosts[i]);
		}
	}

	ans = this->opponentsInDistance(b,pac,ghosts);

	//compareFlock(ghosts,ans);
	//cout << "End Excluding\n";
	return ans;
}



bool SSSAB::findPoint(const Point<int> & pos, vector<Agent> &flock) const
{
	for (unsigned int i = 0; i<flock.size(); i++)
	{
		if(flock[i].pos == pos)
			return true;
	}
	return false;
}

bool SSSAB::findPath(const Path & p,const  vector<Agent> & flock) const
{
	for (unsigned int i = 0; i<flock.size(); i++)
	{
		if(!flock[i].currPath.isEmpty() && p.sameDirectionOnSamePath(flock[i].currPath) )
			return true;
	}	
	return false;
}

vector<Point<int> > SSSAB::inOriginalSight(const vector<Point<int> > &  vpts) const
{
	vector<Point<int> > vis;
	for (unsigned int i = 0; i<vpts.size(); i++)
	{
		if(this->canSeePoint(vpts[i],this->pacOrig))
			vis.push_back(vpts[i]);
	}	

	return vis;
}

void SSSAB::excludeSpecificGhost(vector<Agent> &bads,int index)
{
	if(!bads.empty())
	{		
		vector<Agent>::iterator pi = bads.erase(bads.begin()+index);
		bads.erase(pi,bads.end()); //Delete any duplicates		
	}
}


void  SSSAB::compareFlock(const vector<Agent> &ls, const vector<Agent> &rs) const
{
	for (unsigned int i = 0; i<ls.size(); i++)
	{
		bool found = false;
		for (unsigned int y = 0; y < rs.size() && !found; y++)
		{
			if(ls[i].gameIndex == rs[y].gameIndex)
				found = true;
		}

	}
}

