#ifndef ITEM_H
#define ITEM_H

#include"headers.h"
#include"point.h"
#include"color.h"

class Item
{	
	friend ostream& operator<<(ostream &out,Item& f);
	friend istream& operator>>(istream &in,Item& f);
	friend bool operator==(Item a, Item b);
	friend bool operator<(Item a, Item b);

public:
	Item();
	Item(Pieces type);
	Item(Pieces type,int PointsPer);
	Item(Pieces type,int PointsPer, bool visible);

	Item(Point<int> p,int pf,Pieces itype,bool visible);

	void setupItem(Pieces type);
	void setupTimedVisible();

	void setIncTime(int ict);

	void autoIncCurrTime();
	void resetVisible();

	Point<int> pos;
	int pointsPerItem,currTime;
	int incTimeVal;
	bool visible;	
	Pieces itype;
	bool timedItem;
};
#endif

