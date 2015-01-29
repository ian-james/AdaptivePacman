#ifndef FLOCK_H 
#define FLOCK_H 

#include"algo.h"

class Flock : public Algo
{	
public:	
	Flock(VisionType vt,int visionLength);
	virtual ScoreSystem makeMove(const Board & b,const Agent & me, const Agent & pac,vector<Agent> & ghosts);

	Point<int> getAveragePos(const vector<Agent> & flock) const;
	Point<int> getAverageAlignment(const vector<Agent> & flock) const;
	double getAverageSeperation(const Point<int> & testPos,vector<Agent> & flock) const ;
	double getSeperationFromPacman(AgentState currState,const Point<int> & pacPos, const Point<int> & testPos) const;

	ScoreSystem flockingAlgorithm(const Board & b,const Agent &me, const Agent & pac,vector<Agent> & ghosts);

	double bellCurve(double maxiVal,double x) const;	

};
#endif

