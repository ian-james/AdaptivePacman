#ifndef headers_H
#define headers_H

#pragma once

#include<iostream>
#include<fstream>
#include<cstdio>
#include<string>
#include<cmath>
#include<vector>
#include<map>
#include<set>
#include<queue>
#include<algorithm>
#include<iterator>
#include<functional>
#include<ctime>
#include<stdarg.h>	
#include<sstream>
#include<numeric>
#include<cstdlib>


#define SHORTCOMMENT
#define DEBUG_MODE

using namespace std;

inline static void init()
{
	srand( time(NULL));
}

inline static void init(unsigned int seed)
{
	srand(seed);
}

inline static double randomRange(int maxVal,int lowVal = 0)
{//Random number in the range
	return lowVal + rand() * (double)(maxVal - lowVal) / RAND_MAX;
}

enum AgentType {PACMAN,GHOST};
enum AgentState {FLEE,CHASE,WANDER,DEAD};
enum Movement {NONE = -1, UP = 0, DOWN, LEFT, RIGHT};
enum Pieces{EMPTY = 'E',WALL = 'W' ,TOKEN = 'T',FRUIT = 'F',PP = 'P'};

enum GameState { ST_ALIVE, ST_COMP, ST_DEAD};

enum VisionType {OMNI,SURROUND,DIRECT};
enum MoveType {SQUARE,PATH};
enum Algorithm {SSS,FORCES,RAND,FLOCK};
	
inline static string boolToString(bool b)
{
	if(b)
		return "true";
	return "false";
}

template<typename T>
string toString(T val)
{
   ostringstream o;
   if (!(o << val))
     return "BAD";
   return o.str();
}

inline static void outputError(string s,bool exitProgram)
{
	#ifdef DEBUG_MODE		
		cerr << "Error: " << s << endl;
		if(exitProgram)
			exit(-1);
	#endif
}

#endif

