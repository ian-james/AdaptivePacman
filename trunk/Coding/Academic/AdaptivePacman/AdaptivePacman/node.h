#ifndef NODE_H
#define NODE_H

#include"headers.h"
#include"point.h"

template<class T> class Node;
template<class T> ostream& operator<<(ostream &out,const Node<T> &a);

template<class T>
class Node
{
	friend ostream& operator<< <> (ostream &out,const Node<T> &a);	

public:
	Node() : maxi(INT_MAX), mini(INT_MIN), depth(0)
	{
	}

	Node& operator=(const Node& n)
	{	
		this->maxi = n.getMaxi();
		this->mini = n.getMini();
		this->path = n.getPath();
		this->dept = n.getDepth();		
		
		return *this;
	}

	bool operator==(const Node<T> & b) const
	{
		vector<Point<int> > va = this->getPath(),vb = b.getPath();
		if(va.size() != vb.size()) 
			return false;

		for(unsigned int i = 0;i<va.size();i++)
		{
			if(!(va.at(i) == vb.at(i)))
				return false;
		}
		return true;
	}
	
	bool operator< (const Node<T> & b) const
	{
		vector<Point<int> > va = this->getPath(),vb = b.getPath();
		
		if(va.size() !=  vb.size())
			return va.size() < vb.size();

		for(unsigned int i = 0;i<va.size();i++)
		{
			if(va.at(i) != vb.at(i))
				return va.at(i) < vb.at(i);
		}
		return false;	
	}

	vector<Point<int> > getPath() const;
	void setPath(vector<Point<int> > P);

	T retPathSize() const;
	Point<int> retPathIndex(int i) const;

	T getMaxi() const;
	void compareSetMaxi(T m);
	void setMaxi(T M);

	T getMini() const;
	void compareSetMini(T m);
	void setMini(T M);

	int getDepth() const;
	void setDepth(int D);

private:
	
	vector<Point<int> > path;
	T maxi,mini;
	int depth;
};



template<class T>
ostream& operator<<(ostream &out,const Node<T> & a)
{
	vector<Point<int> > va = a.getPath();
	for(unsigned int i = 0;i<va.size();i++)
		out << va.at(i) << " ";
	return out;
}

template<class T>
vector<Point<int> > Node<T>::getPath() const
{
	return path;
}
template<class T>
void Node<T>::setPath(vector<Point<int> > P)
{
	path = P;
}
template<class T>
T Node<T>::getMaxi() const
{
	return maxi;
}
template<class T>
void Node<T>::setMaxi(T M)
{
	maxi = M;
}
template<class T>
void Node<T>::compareSetMaxi(T M)
{
	maxi = max(maxi,M);
}
template<class T>
T Node<T>::getMini() const
{
	return mini;
}
template<class T>
void Node<T>::setMini(T M)
{
	mini = M;
}
template<class T>
void Node<T>::compareSetMini(T M)
{
	mini = min(mini,M);
}
template<class T>
int Node<T>::getDepth() const
{
	return depth;
}
template<class T>
void Node<T>::setDepth(int D)
{
	depth = D;
}

template<class T>
T Node<T>::retPathSize() const
{
	return this->path.size();
}	

template<class T>
Point<int> Node<T>::retPathIndex(int i) const
{
	Point<int> b(-1,-1);
	if(i >= (int)path.size() || i < 0)
		return b;
	return path[i];
}

#endif

