#ifndef PATH_H
#define PATH_H

#include"point.h"

class Path
{
	friend ostream& operator<<(ostream &out, const Path & p);
	
public:
	Path();	

	bool operator==(const Path & a) const;
	bool operator<(const Path & a) const;			

	Point<int> getPoint(int currIndex);

	bool pointOnPath(const Point<int> & pt) const;

	void resetScore();
	
	void resetAll();
	void reversePath();	

	string toString() const;	

	bool isEmpty() const;

	int getPathDir( const Point<int> & intPt) const;

	Point<int> getPathEnd(const Point<int> & intPt) const;
	Point<int> getPathStart(const Point<int> & intPt) const;

	int getPathLength() const;

	bool sameDirectionOnSamePath(const Path & other) const;



//Variables
	vector<Point<int> > myPath;	
	int score,initScore;
};



#endif

