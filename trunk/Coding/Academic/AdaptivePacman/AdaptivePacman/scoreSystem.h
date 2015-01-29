#ifndef SCORESYSTEM
#define SCORESYSTEM

#include"scoreInfo.h"

class ScoreSystem
{
	public:
		ScoreSystem(); 
		ScoreSystem(vector<Point<int> > & moves);
		ScoreSystem(const Point<int> & mainPt,vector<Point<int> > & moves);

		void addPointScore(const Point<int> & pt,double score = 0);
		void addPointScore(Movement m,double score = 0);

		void addPoint(const Point<int> & pt,double score);

		void randomizeTopScores();
		bool isEmpty() const;

		vector<Point<int> > getMoves();

//private:
		vector<ScoreInfo> scoreSystem;
		Point<int> origPt;
};

#endif

