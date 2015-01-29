#include"gameController.h"
#include"logger.h"
#include"flock.h"
#include"forces.h"
#include"SSSAB.h"

GameController::GameController()
{
	useGraphics = true;
	globalDebug = false;	
}

GameController::~GameController()
{
}

void GameController::setGameID(int attsID)
{
	gameID = attsID;
}

void GameController::init(int nghosts,int ngames,int maxSteps,string boardFile,string afile,string resFile)
{
	attsFile = afile;

	Logger* log = Logger::Instance();
	log->openFile(resFile);
	GameAttributes::Instance()->setConstants(nghosts,ngames,maxSteps,boardFile);
	setupAttributes();
	setupBoard();
	setGameAttributes();
}

void GameController::setupBoard()
{
	//Sets all attributes to their values
	if(b.readBoardFromFile(GameAttributes::Instance()->boardFile))
		b.computePaths(0);	
	else
	{
		Logger::Instance()->closeFile();
		outputError("Reading Board!",true);
	}

	//set starting points
	GameAttributes::Instance()->p.pos = b.pacStart;
	GameAttributes::Instance()->p.lastPoint = b.pacStart;
	for (unsigned int i = 0; i<GameAttributes::Instance()->g.size(); i++)
	{
		GameAttributes::Instance()->g[i].pos = b.ghostStart;
		GameAttributes::Instance()->g[i].lastPoint = b.ghostStart;
	}		

	b.fruit = GameAttributes::Instance()->ft;
}
void GameController::setupAttributes()
{
	ifstream ins(attsFile.c_str());
	if(!ins)
	{
		ins.clear();
		ins.close();

		outputError("Creating default Atts File",false);

		ofstream out(attsFile.c_str());
		if(out)
		{
			out << *(GameAttributes::Instance());
			out.close();
		}
		Logger::Instance()->closeFile();
		outputError("Reading Attributes!",true);
	}

	ins >> *(GameAttributes::Instance());

	ins.close();
}

void GameController::setGameAttributes()
{
	Logger *log = Logger::Instance();
	log->outputXMLValue("GAME",toString(GameAttributes::Instance()->currGame));	
	
	pacman = Agent(GameAttributes::Instance()->p);
	log->life = pacman.lives;

	bads.clear();
	for (unsigned int i = 0; i<GameAttributes::Instance()->g.size(); i++)
		bads.push_back(Agent(GameAttributes::Instance()->g[i]));

	if(pacman.lives < 0)
		pacman.lives = GameAttributes::Instance()->p.lives;

	log->outputXMLValue("LIFE",toString(pacman.lives));
	
}

bool GameController::resetGame() //true endGame
{
	Logger::Instance()->outputGame();

	b.resetAll();

	GameAttributes::Instance()->currGame += 1;
	Logger::Instance()->gameID = GameAttributes::Instance()->currGame;	

	if(GameAttributes::Instance()->currGame >= GameAttributes::Instance()->NGAMES)
		return true;

	setGameAttributes();

	Logger::Instance()->life = pacman.lives;
	Logger::Instance()->resetGameInfo();

	return false;
}

bool GameController::resetNextLife(GameState gs) //
{
	int pacSteps = pacman.steps;
	if(gs == ST_COMP)
	{
		b.resetAll();
		pacman.reset(false);
	}
	else
		pacman.reset(true);


	if(gs == ST_COMP)
	{
		pacman.steps = pacSteps;
		Logger::Instance()->incPair(Logger::LVL_COMP);
	}
	else
	{
		Logger::Instance()->outputLife();
		Logger::Instance()->outputXMLValue("LIFE",toString(pacman.lives));
		Logger::Instance()->resetLifeInfo();
	}

	pacman.pos = GameAttributes::Instance()->p.pos;	
	

	for (unsigned int i = 0; i<bads.size(); i++)
	{
		bads[i].reset();
		bads[i].pos = GameAttributes::Instance()->g[i].pos;
	}	
	
	return (pacman.lives >= 0);
}
	
GameState GameController::runGame() //True end game
{
	GameState gameOver = ST_ALIVE;
	do
	{
		gameOver = agentMove(this->b,pacman,pacman,this->bads); 				

		for (unsigned int i = 0; i<bads.size() && gameOver == ST_ALIVE; i++)
			gameOver = agentMove(this->b,bads[i],pacman,this->bads);		

 	}while(gameOver == ST_ALIVE && !useGraphics);

	return gameOver;
}
void GameController::handleAgentCollision(Agent &pac,Agent &ghost)
{
	if(pac.pos == ghost.pos)
	{
		if(ghost.currState == FLEE)
		{	
			ghost.setCurrentState(DEAD);
			ghost.pos = GameAttributes::Instance()->g[ghost.gameIndex].pos;
			Logger::Instance()->incPair(Logger::GHOSTS);
			Logger::Instance()->incPair(Logger::SCORE,GameAttributes::Instance()->ghostVal);			
		}
		else		
		{
			pacman.setCurrentState(DEAD);
		}
	}	
}

bool GameController::handleAllAgentCollision()
{ 
	for (unsigned int i = 0; i<bads.size(); i++)
	{
		if(bads[i].currState != DEAD)
		{
			handleAgentCollision(pacman,bads[i]);
			if(pacman.currState == DEAD)			
				return true;			
		}
	}
	return false;
}

int GameController::closeCalls() const
{
	int c = 0;
	for (unsigned int i = 0; i<bads.size(); i++)
	{
		if(bads[i].shouldChase() && bads[i].pos.manHattan(pacman.pos) <= 2)
			c++;		
	}
	return c;
}

bool GameController::handleAllCollisions(bool pacMoved)
{
	if(pacMoved)
	{
		handleItemCollision(pacman);
		b.updateFruit(pacman);
	}

	return	handleAllAgentCollision();
}

void GameController::handleItemCollision(Agent & pac)
{
	Pieces pval = b.getSquare(pac.pos);
	Logger::Instance()->ate = true;

	if(pval == WALL || pval == EMPTY)
	{
		Logger::Instance()->incPair(Logger::REP_SQR);
		Logger::Instance()->ate = false;
		return ;	
	}	

	if(pval == PP)
	{	
		Logger::Instance()->incPair(Logger::PPS);

		for (unsigned int i = 0; i<bads.size(); i++)
		{
			if(bads[i].currState != DEAD)
				bads[i].setCurrentState(FLEE);		
		}
	}
	else
		Logger::Instance()->incPair(Logger::TOKENS);

	//Count Score and log
	int temp = GameAttributes::Instance()->getSingleSquareCost(pval);
	Logger::Instance()->incPair(Logger::SCORE,temp);
	
	if(!(pval == WALL || pval == EMPTY))
		b.emptySquares.push_back(pac.pos);
	b.setSquare(pac.pos,EMPTY);

}

GameState GameController::agentMove(Board & board,Agent &me, Agent &pac, vector<Agent> & ghosts) //  false pacman alive, true = continue playing
{	
	if(me.isPacman() && me.getIsDead())
		return ST_DEAD;

	if(!me.incAgentState())
		return ST_ALIVE;	

	bool atIntersect = board.atIntersection(me.pos);

	Algo *algo = setupAlgorithm(me,me.getCurrAlgo());

	if(me.shouldChase() && algo->canSeePoint(me.pos,pac.pos))
		me.setCurrentState(CHASE);

	if(atIntersect || me.currPath.isEmpty())
	{
		if(me.isPacman() && !me.currPath.isEmpty())
		{
			me.currPath.score = algo->computePathScore(board,me.currPath);
			algo->updatePathsFromIntersection(me.pos,board);
		}

		ScoreSystem movesScore = algo->makeMove(b,me,pac,ghosts);
		Point<int> nextPos = me.chooseMove(movesScore);
		me.setNextPos(nextPos);	
	
		me.setPathInfo(board.pc.getPath(me.lastPoint,me.pos));	
	}
	else
	{
		me.incPathPos();	
	}

	delete algo;


	if(me.isPacman())
	{			
		Logger *log = Logger::Instance();
		log->incPair(Logger::STEPS,1);
		log->ate = false;

		handleItemCollision(me);
		b.updateFruit(me);

		int cc = this->closeCalls();
		log->incPair(Logger::CLOSE_CALLS,cc);
		
		log->outputXMLStep(me.steps,this->closeCalls(),log->ate?0:1);
		
		if( b.levelComplete())
		{			
			log->incPair(Logger::SCORE,GameAttributes::Instance()->successVal);	
			return ST_COMP;
		}
		else		
			handleAllAgentCollision();

		if(!me.getIsDead() && me.steps >= GameAttributes::Instance()->MAXSTEPS)
		{
			//Output Simulation Stopped at Max
			Logger::Instance()->incPair(Logger::SCORE,GameAttributes::Instance()->successVal/2);
			Logger::Instance()->outputXMLValue("GAME","FIN_MAX_STEPS");
			return ST_DEAD;	
		}

	}
	else
		handleAgentCollision(pac,me);

	if(pac.getIsDead()) 
		return ST_DEAD;

	return ST_ALIVE;
}

Algo* GameController::setupAlgorithm(const Agent &agent,Algorithm agentAlgorithm) const
{
	Algo* next = NULL;
	VisionType vt = agent.stateVisionType[agent.currState];
	int visLen = agent.stateVisionLength[agent.currState];


	switch(agentAlgorithm)
	{
	case RAND:
		next = new Algo(vt,visLen);
	break;
	case FLOCK:
		next = new Flock(vt,visLen);
	break;
	case FORCES:
		next = new Forces(vt,visLen);
	break;
	case SSS:
		next = new SSSAB(vt,visLen);
	break;
	}

	return next;
}

