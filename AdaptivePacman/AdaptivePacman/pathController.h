#ifndef PATHCONTROLLER_H
#define PATHCONTROLLER_H

//#include"board.h"
#include"path.h"
#include"gameAttributes.h"

class Board;
class Intersection;

class Intersection
{
	
public:
	Intersection(const Point<int> & intersectionPt, const Point<int> & firstStep);

	bool operator==(const Intersection & a) const;
	bool operator<(const Intersection & a) const;

	//private:
	Point<int> intersect,firstStep;
};

class PathController
{	
public:
	PathController();		

	Intersection getFrontIntersection(const Path & p);
	Intersection getEndIntersection(const Path & p);

	void addPathToGraph(const Path & p);

	Path getPath(const Point<int> & intersection,const Point<int> & first) const;
	Path getPath(const Intersection & pt) const;
	
	void resetPathScores();

	vector<Path> getAllPathFromPoint(const Point<int> & intersection, const  vector<Point<int> > &neighbours) const;	

	void updateAllPaths(const Board & b, const Point<int> &intersection, const vector<Point<int> > & neighbours);

	void outputAllPaths();
	
//private:
	map<Intersection,Path> gameGraph;
};
#endif

