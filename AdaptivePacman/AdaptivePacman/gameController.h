#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include"board.h"
#include"agent.h"
#include"pathController.h"
#include"gameAttributes.h"
#include"algo.h"

class GameController
{
public:
	
	GameController();
	~GameController();
	void setGameID(int attsID);

	void init(int nghosts,int ngames,int maxSteps,string boardFile,string afile,string resFile);

	void setupBoard();
	void setupAttributes(); //Load Atts File
	bool resetGame(); //Return done simulation
	void setGameAttributes();

	bool resetNextLife(GameState gs); //return true whether lives are left
	GameState runGame();
	
	void writeStat(string resInfo);
	void handleAgentCollision(Agent &pac,Agent &ghost);
	void handleItemCollision(Agent &  pac);

	bool handleAllAgentCollision();

	bool handleAllCollisions(bool pacMoved);

	GameState agentMove(Board  &board,Agent &me, Agent & pac, vector<Agent> & ghosts); //Game Over
	Algo* setupAlgorithm(const Agent &agent,Algorithm agentAlgorithm) const;

	int closeCalls() const;

//private:
	Board b;
	vector<Agent> bads;
	Agent pacman;	

	bool globalDebug;
	double initialScore;	
	string attsFile;	
	bool useGraphics;
	int gameID;
};

#endif

