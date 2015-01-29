#include"item.h"
Item::Item()
{
	//Default is towards a power pellet or a token, not a fruit
	this->pointsPerItem = 10;
	this->visible = true;
	this->incTimeVal = 0;
	currTime = 0;
}
Item::Item(Point<int> p,int pf,Pieces itype, bool visible)
{
	setupItem(itype);
	this->pos = p;
	this->pointsPerItem = pf;	
	this->visible = visible;
}
Item::Item(Pieces type,int PointsPer)
{
	setupItem(type);
	this->pointsPerItem = PointsPer;
}

Item::Item(Pieces type,int PointsPer, bool visible)
{
	setupItem(type);
	this->pointsPerItem = PointsPer;		
	this->visible = visible;
}


void Item::setupItem(Pieces type)
{
	this->itype = type;
	currTime = 0;

	switch(type)
	{
	case PP:
	case TOKEN:
		timedItem = false;
		visible = true;
	case FRUIT:
		visible = false;
		timedItem = true;
		incTimeVal = 1;
	break;
	}
}

bool operator==(Item a, Item b)
{
	return a.pos == b.pos;
}

bool operator<(Item a, Item b)
{
	return a.pos < b.pos;
}

ostream& operator<<(ostream &out,Item& f)
{
	out << f.pointsPerItem << endl; 
	return out;
}

istream& operator>>(istream &in,Item& f)
{
	in >> f.pointsPerItem; 
	return in;
}

void Item::resetVisible()
{
	visible = true;
}
void Item::autoIncCurrTime()
{
	this->currTime += incTimeVal;
}

void Item::setupTimedVisible() //Ends or starts it
{
	if(timedItem)
	{
		visible = !visible;
		currTime = 0;
	}
}

