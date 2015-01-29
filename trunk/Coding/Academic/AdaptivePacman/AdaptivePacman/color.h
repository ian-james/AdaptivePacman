#ifndef COLOR_H
#define COLOR_H

#include"headers.h"

class Color
{
public:
	Color();	
	Color(string set,string ret = "black");

	void setColor(double r,double g,double b);
	void setRetColor(double r,double g,double b);

	void setRetColor(string s);
	void setColor(string s);

	string getColorString(bool getRetColor) const;
	void setColorStr(string s,bool setRetColor);
	

	string colorStr,retColorStr;

	double r,g,b;
	double retR,retG,retB;
	
};
#endif
