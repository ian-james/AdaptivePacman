#include"scoreSystem.h"

ScoreSystem::ScoreSystem()
{
}

ScoreSystem::ScoreSystem(const Point<int> & mainPt,vector<Point<int> > & moves)
{
	origPt = mainPt;
	
	for (unsigned int i = 0; i<moves.size(); i++)
	{			
		scoreSystem.push_back(ScoreInfo(moves[i]));
		scoreSystem[i].dir = origPt.setDirection(moves[i]);
	}	
}

ScoreSystem::ScoreSystem(vector<Point<int> > & moves)
{
	for (unsigned int i = 0; i<moves.size(); i++)
		scoreSystem.push_back(ScoreInfo(moves[i]));
}

void ScoreSystem::addPointScore(const Point<int> & pt,double score)
{
	for (unsigned int i = 0; i<scoreSystem.size(); i++)
	{
		if(scoreSystem[i].pt == pt)
			scoreSystem[i].score += score;
	}	
}

void ScoreSystem::addPointScore(Movement m,double score)
{
	for (unsigned int i = 0; i<scoreSystem.size(); i++)
	{
		if(scoreSystem[i].dir == m)
			scoreSystem[i].score += score;
	}
}

void ScoreSystem::addPoint(const Point<int> & pt,double score)
{
	bool found = false;
	for (unsigned int i = 0; i<scoreSystem.size(); i++)
	{
		if(scoreSystem[i].pt == pt)
			scoreSystem[i].score += score;
	}

	if(!found)
		scoreSystem.push_back(ScoreInfo(pt,score));

}


void ScoreSystem::randomizeTopScores()
{
	if(!scoreSystem.empty())
	{
		sort(scoreSystem.begin(),scoreSystem.end());
		reverse(scoreSystem.begin(),scoreSystem.end());

		int countTop = 1;
		double highScore = scoreSystem.front().score;
		for (unsigned int i = 1; i<scoreSystem.size(); i++)
		{
			if(scoreSystem[i].score == highScore)
				countTop++;
			else
				break;
		}
		random_shuffle(scoreSystem.begin(),scoreSystem.begin()+countTop);
	}
}

		
vector<Point<int> > ScoreSystem::getMoves()
{
	vector<Point<int> > sc;

	for (unsigned int i = 0; i<scoreSystem.size(); i++)
		sc.push_back(scoreSystem[i].pt);

	return sc;
}

bool ScoreSystem::isEmpty() const
{
	return this->scoreSystem.empty();
}



