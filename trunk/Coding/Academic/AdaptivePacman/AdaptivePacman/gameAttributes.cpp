#include"gameAttributes.h"

GameAttributes* GameAttributes::Instance()
{
	static GameAttributes ga;
	return &ga;
}

GameAttributes::~GameAttributes()
{	
}


istream& operator>>(istream &in,GameAttributes& gs)
{
	in >> gs.stateTime >> gs.stateVisionLength >> gs.stateVisionType >> gs.stateAlgo >> gs.pacmanVisionLength;
	in >> gs.timeFruit >> gs.fruitFreq >>  gs.minFruitDistance  >> gs.maxFruitDistance;
	in >> gs.pacmanVal >> gs.ghostVal >> gs.fruitVal >> gs.ppVal >> gs.tokenVal >> gs.successVal;

	//minimax stuff
	in >> gs.depthOfSearch;

	//flocking stuff
	in >> gs.seperationForce >> gs.alignForce >> gs.cohForce >> gs.hungerForce  >> gs.maxSeperation; //forces to head toward each of these values

	//These are forces for evaluating each algorithm
	in >> gs.forcePacPos  >> gs.forceAwayGhost >> gs.forceProtectToken >> gs.forceProtectPP >> gs.forceProtectFruit >> gs.forceTeamPos >> gs.forcePacDir;
	in >> gs.fruitForce >> gs.tokenForce >> gs.ppForce >>  gs.towardsEdGhosts >>  gs.awayGhosts >>  gs.awayGhostCenter >>  gs.towardItemCenter;

	//Setting Object values.
	gs.pp = Item(PP,gs.ppVal);
	gs.ft = Item(FRUIT,gs.fruitVal,false);	
	gs.t = Item(TOKEN,gs.tokenVal);

	//Pacman info
	gs.p = Agent(PACMAN);
	gs.p.gameIndex = -1;
	in >> gs.p;
	gs.p.setStateVisionLength(gs.pacmanVisionLength);
	gs.p.setStateSteps(gs.stateTime);

	//Ghost info
	for(int i=0;i<gs.NGHOSTS;i++)
	{
		Agent tg = Agent(GHOST);
		in >> tg;
		tg.gameIndex = i;
		tg.setStateSteps(gs.stateTime);
		tg.setStateVisionLength(gs.stateVisionLength);
		tg.setStateVisionType(VisionType(gs.stateVisionType));
		tg.setStateAlgorithm(Algorithm(gs.stateAlgo));
		gs.g.push_back(tg);
	}
	return in;
}
ostream& operator<<(ostream &out,GameAttributes& ga)
{
	out << ga.stateTime << " " << ga.stateVisionLength << " " << ga.stateVisionType <<  " " << ga.stateAlgo << " " << ga.pacmanVisionLength  << "\n";
	out << ga.timeFruit << " " << ga.fruitFreq << " " <<   ga.minFruitDistance  << " " << ga.maxFruitDistance << "\n";
	out << ga.pacmanVal << " " << ga.ghostVal << " " << ga.fruitVal << " " << ga.ppVal << " " << ga.tokenVal << " " << ga.successVal<< "\n";
	//moutimax stuff
	out << ga.depthOfSearch << "\n"; 	

	//flockoutg stuff
	out  << ga.seperationForce << " " << ga.alignForce << " " << ga.cohForce << " " << ga.hungerForce  << " " << ga.maxSeperation << "\n"; //forces to head toward each of these values

	//These are forces for evaluatoutg each algorithm
	out << ga.forcePacPos  << " " << ga.forceAwayGhost << " " << ga.forceProtectToken << " " << ga.forceProtectPP << " " << ga.forceProtectFruit << " " << ga.forceTeamPos << " " << ga.forcePacDir << "\n";	

	out << ga.fruitForce<< " " << ga.tokenForce<< " " << ga.ppForce<< " " << ga.towardsEdGhosts<< " " << ga.awayGhosts<< " " << ga.awayGhostCenter<< " " << ga.towardItemCenter << "\n"; 

	//Pacman outfo
	out << ga.p << "\n";

	//Ghost outfo	
	for(int i=0;i<ga.NGHOSTS;i++)	
		out << ga.g[i] << "\n";
	
	return out;
}
GameAttributes::GameAttributes()
{
	displayInfo = false;
	NGHOSTS = 4;NGAMES = 3;MAXSTEPS = 300;
	currGame = 0 ;currSteps  =0;
	boardFile = "level.txt";
		
	stateTime = 20;
	stateVisionLength = 5; 	
	stateVisionType = 1;
	stateAlgo = 2;
	timeFruit = 15;
	fruitFreq = 0.001;

	pacmanVal = 100;ghostVal = 100 ;fruitVal = 150 ;ppVal = 10;tokenVal = 5;successVal= 500; //Value of eating each;
	fleeTime = deadTime = chaseTime = stateTime;

	//Vision Distance
	nStepsSurround = nStepsDirect = stateVisionLength; 	

	//minimax stuff
	depthOfSearch = 1; 

	//flocking stuff
	seperationForce = alignForce = cohForce = 1.0;
	hungerForce = 1.0; //forces to head toward each of these values
	maxSeperation = 5;

	minFruitDistance = 1;
	maxFruitDistance = 100;

	//These are forces for evaluating each algorithm
	forcePacPos = forceAwayGhost = forceProtectToken = forceProtectPP = forceProtectFruit = forceTeamPos = forcePacDir = 1.0;	
	fruitForce=tokenForce=ppForce= towardsEdGhosts= awayGhosts= awayGhostCenter= towardItemCenter = 1.0;

	// UnComment for Default player settings
	/* = Agent(PACMAN);
	p.gameIndex = -1;	
	
	for(int i=0;i<NGHOSTS;i++)
	{
		Agent tg = Agent();
		tg.gameIndex = i;
		g.push_back(tg);
	}*/
}
void GameAttributes::setConstants(int nghosts,int ngames,int maxSteps,string boardFile)
{
	NGHOSTS = nghosts;
	NGAMES = ngames;
	MAXSTEPS = maxSteps;
	this->boardFile = boardFile;
}

void GameAttributes::editAttributes()
{
	displayInfo = !displayInfo;	
}

int GameAttributes::getSingleSquareCost(Pieces p) const
{
	switch(p)
	{
	case PP:
		return pp.pointsPerItem;
		break;
	case TOKEN:
			return t.pointsPerItem;
		break;
	}
	return 0;
}
