#include"scoreInfo.h"

bool ScoreInfo::operator<(const ScoreInfo & a) const
{
	return this->score < a.score;
}

bool ScoreInfo::operator==(const ScoreInfo & a) const
{
	return this->pt == a.pt;
}

ScoreInfo::ScoreInfo(const Point<int> &pt,double score)
{
	this->pt = pt;
	this->score = score;
}
	
ScoreInfo::ScoreInfo(Movement dir,double score)
{
	this->dir = dir;
	this->score;
}
	

	


