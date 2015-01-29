#ifndef FORCES_H 
#define FORCES_H 

#include"algo.h"

class Forces : public Algo
{	
public:	
	Forces(VisionType vt,int visionLength);
	virtual ScoreSystem makeMove(const Board & b,const Agent & me, const Agent & pac,vector<Agent> & ghosts);	

	ScoreSystem playerForces(const Board &b, const Agent &pac, vector<Agent> & ghosts);	
	double towardsGhost(const Point<int> & testPos,vector<Agent> & ghosts); //Calculates score for closest ghosts + or - if editable
	double awayFromGhost(const Point<int> & testPos, vector<Agent> & ghosts); //For Ghost to move away from other ghosts
	double directionGhostGroup(const Point<int> & testPos,vector<Agent> & ghosts); // Towards centroid of group
	double directionItemGroup(const Point<int> & testPos,const Point<int> & avgItemPos, bool isPacman);
	double distanceToItems(const Board &b, const  Point<int> & testPos, vector<Agent> & ghosts, bool isPacman) const; //Computes score for Token/PP/FRUIT and Area value

	Point<int> getAveragePos(const vector<Agent> & flock) const;
	Point<int> getAveragePosItem(const Board & b,const Point<int> & currPos) const;

	ScoreSystem ghostForces(const Board & b,const Agent & me,const Agent &pac, vector<Agent> & ghosts);	
	double getDistanceToPacman(const Agent & me, const Point<int> & testPos,const Agent &pac) const;	//Considers if can see Pacman, last saw, Editable
	double getDistanceFromGhosts(const Agent & me,const Point<int> &testPos, vector<Agent> & flock) const;
	double protectPieces(const Agent &me, vector<Agent> & flock) const;
	double getTeamScore( const Agent &me, vector<Agent> & flock) const; //Similar to seperation for flocking
	double pacDirection(const Agent &me,  const Point<int> & testPos, const Agent & pac) const;
	
};
#endif

