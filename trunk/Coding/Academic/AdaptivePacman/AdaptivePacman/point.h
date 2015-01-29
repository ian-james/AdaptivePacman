#ifndef Point_H
#define Point_H

#include"headers.h"

template<class T> class Point;
template<class T> ostream& operator<<(ostream &out,const Point<T>& p);
template<class T> istream& operator>>(istream &in,Point<T> & p);
//template<class T> bool operator==(const Point<T> & a) const;
//template<class T> bool operator<(const Point<T> & a, Point<T> b);
//template<class T> Point<T> operator-(const Point<T> & a, const Point<T> & b);
//template<class T> Point<T> operator+(const Point<T> & a, const Point<T> & b);
//template<class T> bool operator!=(const Point<T> & a) const;

template<class T>
class Point
{			
	friend ostream& operator<< <>(ostream &out,const Point<T> & p);
	friend istream& operator>> <>(istream &in,Point<T>& p);
	bool ySort(const Point<T> & a,const Point<T> & b);

public:
	Point(): x(0), y(0)
	{
	}

	Point(T _x, T _y) : x(_x), y (_y)
	{
	}

	Point& operator=(const Point& pt)
	{
		this->x = pt.getX();
		this->y = pt.getY();
		return *this;
	}

	bool operator== (const Point<T> & a) const;
	
	
	bool operator< (const Point<T> & a) const;
	
	bool operator!= (const Point<T> & a) const;
	
	Point<T> operator- (const Point<T> & a);
	Point<T> operator+ (const Point<T> & a);

	
	void setPoint(T x,T y);
	void setX(T dx);
	void setY(T dy);
	T getX() const;
	T getY() const;

	string toString() const;	
	bool getVisible() const;

	double distance(const Point<T> & b) const;
	int manHattan(const Point<T> & b) const;	
	
	Point<T>& divideByScalar(T s);
	Point<T>& multiplyByScalar(T s);
	Point<int>& divideByScalarRound(int s);

	Movement setDirection(Point<T> next) const;
	Movement oppositeDirection(Movement m) const;
	Point<T> getPointInDirection(Movement md) const;	
	void updatePointInDirection(Movement md);

	
	
	bool isBad() const;

private:
	T x,y;
};

template<class T>
ostream& operator<<(ostream &out,Point<T>& p)
{
	out << p.toString();
	return out;
}

template<class T>
istream& operator>>(istream &in,Point<T>& p)
{
	in >> p.x >> p.y;
	return in;
}

template<class T>
bool ySort(const Point<T>  & a, const Point<T> & b)
{
	if(a.getY() == b.getY())
		return a.getX() < b.getX();
	return a.getY() < b.getY();
}

template<class T>
T Point<T>::getX() const
{
	return this->x;
}

template<class T>
 T Point<T>::getY() const
{
	return this->y;
}

template<class T>
void Point<T>::setPoint(T x, T y) 
{
	this->x = x;
	this->y = y;
}

template<class T>
void Point<T>::setX(T dx)
{
	x = dx;
}

template<class T>
void Point<T>::setY(T dy)
{
	y = dy;
}

template<class T>
double Point<T>::distance(const Point<T> &  b) const
{
	return sqrt( double( (this->getX() - b.getX())*(this->getX() - b.getX()) 
		+ (this->getY() - b.getY())*(this->getY() - b.getY())));
}

template<class T>
Point<T> Point<T>::getPointInDirection(Movement md) const
{		
	Point<int> pt(this->x,this->y);
	switch(md)
	{
	case UP:
		pt.setY(pt.getY()-1);
	break;
	case LEFT:
		pt.setX(pt.getX() -1);
	break;
	case DOWN:
		pt.setY(pt.getY() +1);
	break;
	case RIGHT:
		pt.setX(pt.getX() +1);
	break;		
	}
	return pt;
}

template<class T>
void Point<T>::updatePointInDirection(Movement md)
{
		switch(md)
		{
		case UP:
			y += -1;				
		break;
		case LEFT:
			x -= 1;
		break;
		case DOWN:
			y +=1;
		break;
		case RIGHT:
			x+=1;
		break;		
		}
}


template<class T>
Movement Point<T>::setDirection(Point<T> next) const
{	
	Point<T> curr(x,y);
	for(int i = 0;i<4;i++)
	{
		Point<T> temp = getPointInDirection(Movement(i));
		if(temp == next)
			return Movement(i);
	}
	return NONE;
}

template<class T>
Movement Point<T>::oppositeDirection(Movement m) const
{
		switch(m)
			{
			case UP:
				return DOWN;
			break;
			case LEFT:
				return RIGHT;
			break;
			case DOWN:
				return UP;
			break;
			case RIGHT:
				return LEFT;
			break;
			default:
				return NONE;
			break;
			}
}

template<class T>
string Point<T>::toString() const
{
	return "(" + ::toString(this->x) + "," + ::toString(this->y) + ")";
}

template<class T>
Point<T>& Point<T>::divideByScalar(T s)
{
	if(s == 0)
		return *this;

	this->x /= s;
	this->y /= s;	
	return *this;
}

template<class T>
Point<int>& Point<T>::divideByScalarRound(int s)
{
	if(s == 0)
		return *this;

	double rx = (x%s),ry = double(y%s);
	
	divideByScalar(s);	

	if(rx / double(s) >= 0.5)
		this->x++;

	if(ry / double(s) >= 0.5)
		this->y++;

	return *this;
}

template<class T>
int Point<T>::manHattan(const Point<T> & b) const
{
	return abs(this->getX()-b.getX()) + abs(this->getY()-b.getY());
}

template<class T>
bool Point<T>::isBad() const
{
	return (this->getX() == -1 && this->getY() == -1);
}

template<class T>
Point<T>& Point<T>::multiplyByScalar(T s)
{
	this->x = this->x*s;
	this->y = this->y*s;
	return *this;	
}

template<class T>
bool Point<T>::getVisible() const
{
	return true;
}

template<class T>
	bool Point<T>::operator== (const Point<T> & a) const
	{
		return (a.getX() == this->getX() && a.getY() == this->getY());
	}
	
template<class T>
	bool Point<T>::operator< (const Point<T> & a) const
	{
		if(a.getX() == this->getX())
			return a.getY() < this->getY();
		return a.getX() < this->getX();
	}

template<class T>
	bool Point<T>::operator!= (const Point<T> & a) const
	{
		return (a.getX() != this->getX() || a.getY() != this->getY());		
	}

template<class T>
	Point<T> Point<T>::operator-  (const Point<T> & a)
	{
		return Point<T>(this->getX() - a.getX(),this->getY() - a.getY() );
	}

template<class T>
	Point<T> Point<T>::operator+ (const Point<T> & a)
	{
		return Point<T>(this->getX() + a.getX(),this->getY() + a.getY() );
	}

#endif

