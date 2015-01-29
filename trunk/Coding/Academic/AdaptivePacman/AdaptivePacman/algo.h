#ifndef ALGO_H 
#define ALGO_H 

#include"headers.h"
#include"point.h"
#include"color.h"
#include"scoreSystem.h"
#include"board.h"
#include"agent.h"

class Algo
{	
public:	
	Algo(VisionType vt,int visionLength);
	virtual ScoreSystem makeMove(const Board & b,const Agent & me, const Agent & pac,vector<Agent> & ghosts);	

	bool canSeePoint(const Point<int> & currPos, const Point<int> & target) const;	
	bool canSeeAgent(const Agent & me, const Agent & other) const;
	
	vector<Agent> getVisibleNeibours(const Agent & me, const vector<Agent> & others) const;	

	void removeDuplicateOpponents(vector<Agent> &vags);

	void findClosestItem(const Point<int> & currPos,const Board & b, int &minDistToken, int & minDistPP) const;
	int findClosestGhost(const Point<int>  & currPos, const vector<Agent> &  ghosts, bool editable) const;

	bool ghostsInFlee( const vector<Agent>& ghosts) const;


	int computePathScore(const Board & b,const Path &p) const;
	int computePathScoreVisible(const Board & b, const Path & p, const Point<int> & viewPt) const;

	void updatePathsFromIntersection(const Point<int> & intersection,Board & b);	

	double calcOneOver(double val,double force = 1.0) const;

	int computeBoardScore(const Board & b,const Agent & me) const;


	vector<Point<int> > neighbourSquaresInSight(const Board & b,const Point<int> & currPos, const Point<int>  & visionPoint) const;

	int visionLength;
	VisionType vt;	
};
#endif

