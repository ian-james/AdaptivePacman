#include"color.h"

Color::Color()
{
	//Default Draw Color is Red
	r = 1;
	g = b =	retR = retG = retB = 0;	
	colorStr = "red";	
	retColorStr = "black";
}

Color::Color(string set,string ret)
{
	setColor(set);
	setRetColor(ret);
}

void Color::setColor(double r,double g,double b)
{ 
	this->r = r;
	this->g = g;
	this->b = b;
}
void Color::setRetColor(double r,double g,double b)
{
	this->retR = r;
	this->retG = g;
	this->retB = b;
}

void Color::setColor(string s)
{
	if(s == "red")
		setColor(1,0,0);
	else if(s == "blue")
		setColor(0,0,1);
	else if(s == "green")
		setColor(0,1,0);
	else if(s == "yellow")
		setColor(1,1,0);
	else if(s == "orange")
		setColor(1,0.5,0);
	else if(s == "black")
		setColor(0,0,0);
	else if(s == "white")
		setColor(1,1,1);
	else if(s == "pink")
		setColor(1,0,1);
	else if(s == "cyan")
		setColor(0,1,1);
	else if(s == "grey")
		setColor(0.5,0.5,0.5);

	this->colorStr = s;
}

void Color::setRetColor(string s)
{
	
	if(s == "red")
		setRetColor(1,0,0);
	else if(s == "blue")
		setRetColor(0,0,1);
	else if(s == "green")
		setRetColor(0,1,0);
	else if(s == "yellow")
		setRetColor(1,1,0);
	else if(s == "orange")
		setRetColor(1,0.5,0);
	else if(s == "black")
		setRetColor(0,0,0);
	else if(s == "white")
		setRetColor(1,1,1);
	else if(s == "pink")
		setRetColor(1,0,1);
	else if(s == "cyan")
		setRetColor(0,1,1);
	else if(s == "grey")
		setRetColor(0.5,0.5,0.5);

	retColorStr = s;
}

string Color::getColorString(bool getRetColor)  const
{
	if(getRetColor)
		return this->retColorStr;
	return this->colorStr;
}

void Color::setColorStr(string s,bool setRetColor)
{
	if(setRetColor)
		return this->setRetColor(s);
	return setColor(s);
}
	
