#ifndef SCOREINFO
#define SCOREINFO

#include"point.h"

class ScoreInfo
{
public:
	bool operator<(const ScoreInfo & a) const;
	bool operator==(const ScoreInfo & a) const;

	ScoreInfo(const Point<int> & pt,double score = 0.0);
	ScoreInfo(Movement dir,double score = 0.0);			

	Point<int> pt;
	Movement dir;
	double score;
};

#endif

