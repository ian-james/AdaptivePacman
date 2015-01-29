#include"path.h"		
Path::Path()
{	
	initScore = score = 0;
}

bool Path::operator==(const Path & a) const
{
	if(a.isEmpty() || this->isEmpty())
		return false;

	if(a.myPath.size() != this->myPath.size())
		return false;

	if(a.myPath[0] == this->myPath[0]) //Check intersection point and first step.
		return a.myPath[1] == this->myPath[1];
	else if(a.myPath[0] == this->myPath.back()) //Check to see if one path is reverse of other
		return a.myPath[1] == this->myPath[this->myPath.size()-2];	

	return false;
}

bool Path::sameDirectionOnSamePath(const Path & other) const
{
	if(*this == other)	
		return (other.myPath[0] == this->myPath[0] && other.myPath[1] == this->myPath[1] );
	return false;
}

bool operator<(Path a, Path b)
{
	return a.score < b.score;	
}

ostream& operator<<(ostream &out, const Path &  p)
{	
	out << p.toString();
	return out;
}
void Path::resetScore()
{
	score = initScore;
}
void Path::resetAll()
{
	initScore = score = 0;
	myPath.clear();
}

void Path::reversePath()
{
	reverse(myPath.begin(),myPath.end());
}

string Path::toString() const
{
	string p = "First= " + myPath[0].toString() + " , Last= " + myPath.back().toString() + " , Score=" + ::toString(this->score);
	return p;
}

bool Path::pointOnPath( const Point<int> &pt) const
{
	for(unsigned int i = 0;i<this->myPath.size();i++)
	{
		if(myPath.at(i) == pt)
			return true;
	}
	return false;
}


bool Path::isEmpty() const
{
	return myPath.empty();
}


int Path::getPathDir( const Point<int> & intPt) const
{
	if(myPath.empty()) //Error 
		return 0;

	if(intPt == myPath[0])
		return 1;
	else if(intPt == myPath.back())
		return -1;
	return 0;
}


Point<int> Path::getPathEnd( const Point<int> & intPt) const
{
	return (intPt == myPath[0])?myPath.back():myPath[0];
}
Point<int> Path::getPathStart(const Point<int> & intPt) const
{
	return (intPt == myPath[0])?myPath[0]:myPath.back();
}

int Path::getPathLength() const
{
	return myPath.size();
}

Point<int> Path::getPoint(int currIndex)
{
	if(currIndex >= 0 && currIndex < (int) myPath.size())
		return myPath[currIndex];
	return Point<int> (-1,-1);
}
