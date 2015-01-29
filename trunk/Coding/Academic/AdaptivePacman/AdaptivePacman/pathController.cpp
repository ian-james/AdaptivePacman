#include "pathController.h"
#include "board.h"

PathController::PathController()
{
}

Intersection PathController::getFrontIntersection(const Path & p)
{
	return Intersection(p.myPath[0],p.myPath[1]);
}

Intersection PathController::getEndIntersection(const Path &p)
{
	return Intersection(p.myPath.back(),p.myPath[p.myPath.size()-2]);
}

void PathController::addPathToGraph(const Path &p)
{
	if(!p.isEmpty() && getPath(getFrontIntersection(p)).isEmpty()) //Add if new
	{
		gameGraph.insert(make_pair(getFrontIntersection(p),p));
		Path rp = p;
		rp.reversePath();
		gameGraph.insert(make_pair(getEndIntersection(p),rp));			
	}
}

Path PathController::getPath(const Point<int> & intersection,const Point<int> & first) const
{
	map<Intersection,Path>::const_iterator pi = gameGraph.find(Intersection(intersection,first));
	if(pi == gameGraph.end())
		return Path();
	return pi->second;
}

Path PathController::getPath(const Intersection & pt) const
{ 
	return getPath(pt.intersect,pt.firstStep);
}

void PathController::resetPathScores()
{
	map<Intersection,Path>::iterator i = gameGraph.begin();

	for(i;i != gameGraph.end();i++)	
		i->second.resetScore();	
}

vector<Path> PathController::getAllPathFromPoint(const Point<int> & intersection, const vector<Point<int> > &neighbours) const
{
	vector<Path> vp;
	for (unsigned int i = 0; i<neighbours.size(); i++)
	{
		Path p = this->getPath(intersection,neighbours[i]);
		if(!p.isEmpty())
			vp.push_back(p);
	}
	return vp;
}

void PathController::outputAllPaths()
{	
	map<Intersection,Path>::iterator i = gameGraph.begin();
	for(; i != gameGraph.end(); i++)	
		cerr << i->second << endl;	
}

void PathController::updateAllPaths(const Board & b, const Point<int> &intersection, const vector<Point<int> > & neighbours)
{
	for (unsigned int i = 0; i<neighbours.size(); i++)
	{
		Intersection inter(intersection,neighbours[i]);

		map<Intersection,Path>::iterator ip = gameGraph.find(inter);			
		ip->second.score = b.computePathScore(ip->second);
	}
}


/*******************************************************HELPER CLASS *******************************/

Intersection::Intersection(const Point<int> & intersectionPt,const Point<int> & firstStep)
{
	intersect = intersectionPt;
	this->firstStep = firstStep;
}

bool Intersection::operator==(const Intersection & a) const
{
	return (a.intersect == this->intersect && a.firstStep == this->firstStep);
}
bool Intersection::operator<(const Intersection & a) const 
{
	if(a.intersect ==  this->intersect)
		return a.firstStep < this->firstStep;
	return a.intersect < this->intersect;
}


/*******************************************************HELPER CLASS *******************************/

