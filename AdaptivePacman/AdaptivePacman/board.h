#ifndef BOARD_H
#define BOARD_H

#include"headers.h"
#include"point.h"
#include"item.h"
#include"agent.h"
#include"pathController.h"

class Board
{
public:

	Board();
	Board(string board);		

	#ifdef gwGraphics
		void drawSquare(int y,int x);	
		void drawSquare(int y,int x, double color[],int shrinkSize);
		void drawObject(const Color & c,const Point<int> & pos,bool circle,double radius);	
		void drawFruit(const Color & c, const Point<int> & pos);
		void drawCircle(const Color & c,const Point<int>  & p,double radius);
		void drawBoard();
	#endif

	bool readBoardFromFile(const string &  file);	
	void boardDisplay(ostream &out);	

	bool validSquare(int y,int x) const; // Check bounds of board.
	bool validSquare(const Point<int> & p) const;

	Pieces getSquare(int y,int x) const;
	Pieces getSquare(const Point<int> & p) const;

	void setSquare(int y,int x,Pieces val);	
	void setSquare(const Point<int> & p,Pieces val);	

	int getMapRowSize() const;
	int getMapColSize() const;

	void resetMap();
	void resetAll();// Return more games to play

	vector<Point<int> > getValidNeighbourSquares(const Point<int> & p,Movement val = NONE,bool initialPoint = false) const;	
	vector<Movement> getValidMovementSquares(const Point<int> & p,Movement val = NONE, bool initialPoint = false) const;	

	bool atIntersection(const Point<int> & p) const;
	bool levelComplete() const;

	double computeBoardScore() const;

	int computePathScore(const Path &p) const;

	void updateFruit(Agent & pacman);

	void findAllPaths(Point<int> p,Path curr, int costOfTravel,Movement dir, vector<Point<int> > beenTo);
	void computePaths(int tc); //Computes all the paths from one Point<int> to another
	

	Point<int> getFruitSquare(const Point<int> & pacPos, int minDist = 0, int maxDist = 1000);

	vector<Point<int> > getNeighboursWithDirection(const Point<int> & p,Movement oppositeDir) const;

	/*Variables */
	Point<int> pacStart,ghostStart;
	vector< vector<Pieces> > map;
	vector< vector<Pieces> > original;
	vector<Point<int> > emptySquares;
	
//	vector< Item > tokens;
//	vector< Item > pp ;
	Item fruit;
	PathController pc;

	int size; // Size to stretch the board by.
	bool stop; // Stop drawing the board the game is over.
};

#endif

