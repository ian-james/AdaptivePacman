#ifndef SSSAB_H
#define SSSAB_H

#include"headers.h"
#include"algo.h"
#include"node.h"
#include"scoreSystem.h"

class SSSAB : public Algo
{
public:
	enum SIMSTATES{PLAYER_DEATH,LEVEL_COMPLETE, MAX_SIM_STEPS,CONTINUE,VISION_END};

	SSSAB(VisionType vt,int visionLength);
	virtual ScoreSystem makeMove(const Board & b,const Agent & me, const Agent & pac,vector<Agent> & ghosts);	

	double sssab(double alpha,double beta,int depth, map<Node<double>,double> &table,Node<double> n,Board b,Agent pac,vector<Agent> ghosts, vector<Point<int> > &path); //The actual recursive algorithm
	ScoreSystem sss(Board b,Agent ply, vector<Agent> ghosts); //Initialize the algorithm

	int getNextPlayerTurn(Agent &p, vector<Agent> &ghosts);
	SIMSTATES simGame(Board &b,Agent &ply,vector<Agent> &ghosts);

	vector<Point<int> > setupMoveChoices(Board const & b, Agent *active,double &maxi,Point<int> pacPos);
	bool isSimFinished(Board &b,Agent &ply, vector<Agent> &ghosts,int depths, Node<double> & n,double &maxi, bool visionFinished = false);

	double computeGameScore(const Board & b, const Agent & ply) const;

	//Collisions copied form GAMECONTROLLER
	void handleItemCollision(Board &b,Agent & pac,vector<Agent> &bads);
	void handleAgentCollision(Agent &pac,Agent &ghost);
	bool handleAllAgentCollision(Agent &ply,vector<Agent> &bads); //Return Pacman Dead

	int simForcedMoves(Board &b, Agent &pac, vector<Agent> &bads,double &maxi,Node<double> &n);

	void outputPath(const vector<Point<int> > & path,double score) const;

	void sortByDistance(vector<Point<int> > &vpts, const Point<int> & pacPos);
	void sortByValue(vector<Point<int> > &vpts, const Board & b);

	vector<Agent> opponentsInDistance(const Board &b,const Agent &pac,const vector<Agent> &ghosts) const;

	bool noGhostOnPath(const Board &b,const Point<int> & intersect, const Point<int> & first, const vector<Agent> &flock) const;

	vector<Agent> excludeGhosts(const Board &b,const Agent &pac, const vector<Agent> & ghosts) const;


	bool findPoint(const Point<int>  &pos, vector<Agent> &flock) const;
	bool findPath(const Path &p, const vector<Agent> &flock) const;

	vector<Point<int> > inOriginalSight(const vector<Point<int> > &  vpts) const;

	void excludeSpecificGhost(vector<Agent> &bad,int index);


	double closestItemScore(const Point<int>  & currPos,const Board  &b) const;

		
	int visionLength;
	VisionType vt;	
	MoveType  moveType;

	Point<int> pacOrig;

	const static int MAX_TABLE_SIZE = 1000;
	void  compareFlock(const vector<Agent> &ls, const vector<Agent> &rs) const;
};
#endif

