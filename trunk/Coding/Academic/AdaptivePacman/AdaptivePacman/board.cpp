#include"board.h"
#include"logger.h"

Board::Board()
{
	size = 10;
	stop = false;
}

#ifdef gwGraphics
void Board::drawFruit(const Color & c,const Point<int> & pos)
{
	glPushMatrix();
		glBegin(GL_POLYGON);
			glColor3f(c.r,c.g,c.b);
			glVertex2f(pos.getX()*size + size/2,-pos.getY()*size);
			glVertex2f(pos.getX()*size + size,-pos.getY()*size + size/2);		

			glVertex2f(pos.getX()*size + size/2,-pos.getY()*size + size);

			glVertex2f(pos.getX()*size,-pos.getY()*size + size/2);
			glVertex2f(pos.getX()*size + size/2,-pos.getY()*size);
			glColor3f(c.retR,c.retG,c.retB);
		glEnd();	
	glPopMatrix();
}
void Board::drawObject(const Color & c,const Point<int> &  pos,bool circle,double radius)
{		
	if(circle)
		drawCircle(c,pos,radius);
	else
	{	
		glBegin(GL_POLYGON);
			glColor3f(c.r,c.g,c.b);
			glVertex2f(pos.getX()*size,pos.getY()*size);
			glVertex2f(pos.getX()*size + size,pos.getY()*size);		
			glVertex2f(pos.getX()*size + size,pos.getY()*size + size);
			glVertex2f(pos.getX()*size,pos.getY()*size + size);
			glVertex2f(pos.getX()*size,pos.getY()*size);
			glColor3f(c.retR,c.retG,c.retB);
		glEnd();
	}
}
void Board::drawCircle(const Color & c, const Point<int>  & p,double radius)
{
	glBegin(GL_LINE_LOOP);
	glColor3f(c.r,c.g,c.b);
	for(double angle = 0;angle < 360;angle++)
		glVertex2f(p.getX()*size + size/2 + sin(angle) * radius, p.getY()*size + size/2 + cos(angle) * radius);
	glColor3f(c.retR,c.retG,c.retB);
	glEnd();
}

void Board::drawSquare(int y,int x)
{
	glColor3f(1.0,1.0,1.0);
	glBegin(GL_POLYGON);
		glVertex2f(x*size,y*size);
		glVertex2f(x*size + size,y*size);
		glVertex2f(x*size + size,y*size + size);
		glVertex2f(x*size,y*size + size);
		glVertex2f(x*size,y*size);
	glEnd();		
	glColor3f(0,0,0);
}

void Board::drawSquare(int y,int x, double color[],int shrinkSize)
{
	glColor3f(color[0],color[1],color[2]);
	glBegin(GL_POLYGON);
		glVertex2f(x*size + shrinkSize,y*size+shrinkSize);
		glVertex2f(x*size + size - shrinkSize,y*size+shrinkSize);
		glVertex2f(x*size + size-shrinkSize,y*size + size-shrinkSize);
		glVertex2f(x*size+shrinkSize,y*size + size-shrinkSize);
		glVertex2f(x*size +shrinkSize,y*size+shrinkSize);
	glEnd();		
	glColor3f(0,0,0);
}

void Board::drawBoard()
{
	for (int i = 0; i<(int)map.size(); i++)
	{
		for(int y = 0;y<(int)map[i].size();y++)
		{
			switch(map[i][y]) //Switch on how to draw the object
			{
			case TOKEN:
				drawObject(Color("white"),Point<int>(y,-i),true,3);
				break;
			case PP:				
				drawObject(Color("orange"),Point<int>(y,-i),true,4);
				break;
			case WALL:
				drawObject(Color("grey"),Point<int>(y,-i),false,10);
				break;
			case EMPTY:
				break;
			default:
				drawObject(Color("blue"),Point<int>(y,-i),false,10); //Show any errors in level design
			}
		}
	}	

	if(fruit.visible)
		drawFruit(Color("red"),fruit.pos);
}

#endif

bool Board::readBoardFromFile(const string &  file)
{
	ifstream in(file.c_str());
	if(!in)
	{
		cerr << "ERROR::Reading board\n";
		in.clear();
		in.close();
		return false;
	}
	else
	{		
		in >> pacStart >> ghostStart; in.ignore(100,'\n');				
		char ch;		
		vector<Pieces> v;	
		while(!in.eof())
		{
			in.get(ch);

			if(ch == '\n')
			{
				original.push_back(v);
				v.clear();
			}			
			else
			{						
				v.push_back(Pieces(ch));				
			}
		}
	}
	in.close();
	map = original;		
	return true;
}



void Board::boardDisplay(ostream &out)
{
	for(unsigned int i = 0;i<map.size();i++)
	{
		for(unsigned int t =0 ;t < map[i].size();t++)
			out << map[i][t] << " ";
		out << endl;
	}
}


int Board::getMapRowSize() const
{
		return this->map.size();		
}
int Board::getMapColSize() const
{
		return map[0].size();
}

bool Board::validSquare(int y, int x) const
{
	return (y >=0  && y < (int)map.size() && x >= 0 && x < (int)map[y].size());	
}

void Board::setSquare(int y,int x,Pieces val)
{
	map[y][x] = val;
}

void Board::setSquare(const Point<int> & pos,Pieces val)
{
	setSquare(pos.getY(),pos.getX(),val);	
}

Pieces Board::getSquare(int y,int x) const
{
	if(validSquare(y,x))
		return map[y][x];
	return EMPTY; 
}

Pieces Board::getSquare(const Point<int> & p) const
{
	if(validSquare(p.getY(),p.getX()))
		return map[p.getY()][p.getX()];
	return EMPTY;
}

void Board::resetMap()
{
	this->map = original;
	emptySquares.clear();

	fruit.visible = false;	
}

vector<Point<int> > Board::getValidNeighbourSquares(const Point<int> & p,Movement val,bool initialPoint) const
{
	Movement myArray[] = {UP,DOWN,LEFT,RIGHT};

	Point<int> dp;
	vector<Point<int> > vp;
	for(int i = 0;i<4;i++)
	{
		if(val == myArray[i])
			continue;

		dp = p;
		dp.updatePointInDirection(myArray[i]);

		if(validSquare(dp) && getSquare(dp) != WALL)
			vp.push_back(dp);
	}

	if(initialPoint)
		vp.push_back(p);
	return vp;
}

vector<Movement> Board::getValidMovementSquares(const Point<int> &  p,Movement val, bool initialPoint) const
{//VAL Allows you to skip a direction if you don't want to include it, initial point considers current positino a move.
	Movement myArray[] = {UP,DOWN,LEFT,RIGHT};

	Point<int> dp;
	vector<Movement> vp;
	for(int i = 0;i<4;i++)
	{
		if(val == myArray[i])
			continue;

		dp = p;
		dp.updatePointInDirection(myArray[i]);

		if(validSquare(dp) && getSquare(dp) != WALL)
			vp.push_back(myArray[i]);
	}

	if(initialPoint)
		vp.push_back(NONE);	
	return vp;
}

vector<Point<int> > Board::getNeighboursWithDirection(const Point<int> & p,Movement oppositeDir) const
{
	if(atIntersection(p))
		return getValidNeighbourSquares(p);
	return getValidNeighbourSquares(p,oppositeDir);
}


bool Board::atIntersection(const Point<int> & p) const
{
	return (getValidNeighbourSquares(p).size() > 2);
}

bool Board::validSquare(const Point<int> & p) const
{
	return validSquare(p.getY(),p.getX());
}

bool Board::levelComplete() const
{
	for (unsigned int i = 0; i<map.size(); i++)
	{
		for (unsigned int y = 0; y<map[i].size(); y++)
		{
			if(map[i][y] == TOKEN || map[i][y] == PP)		
				return false;
		}
	}
	return true;
}
void Board::findAllPaths(Point<int> p,Path curr, int costOfTravel,Movement dir,vector<Point<int> > beenTo)
{
	//Taking out the up option will decrease the search space significantly, means we have to  include paths based on found end Points though
	curr.myPath.push_back(p);
	int val = costOfTravel + GameAttributes::Instance()->getSingleSquareCost(getSquare(p));
	curr.score += val;

	if(curr.myPath.size() > 1 && atIntersection(p)) //Stop if we are at another intersection
	{	
		if(find(beenTo.begin(),beenTo.end(),p) == beenTo.end()) //We haven't been to this Point<int> at all
		{				
			curr.initScore = curr.score;
			pc.addPathToGraph(Path(curr));
		}
		return;
	}	
	
	vector<Point<int> > vp = getValidNeighbourSquares(p,UP); //Ignore Up movements already called

	if(dir == NONE) //This gets the ball rolling
	{		
		for(unsigned int i = 0;i<vp.size();i++)
		{			
			if(validSquare(vp[i]))
			{
				dir = p.setDirection(vp[i]);								
				findAllPaths(vp[i],curr,costOfTravel,dir,beenTo);				
			}
		}

	}
	else
	{
		Point<int> pt = p.getPointInDirection(dir);

		if(validSquare(pt.getY(),pt.getX()) && getSquare(pt) != WALL)
			findAllPaths(pt,curr,costOfTravel,dir,beenTo);
		else
		{ //We have to change directions to make it around this path		

			for(unsigned int i = 0;i<vp.size();i++)
			{				
				if(vp[i] != p)
				{			
					Movement temp = p.setDirection(vp[i]);
					if(temp != NONE && temp != p.oppositeDirection(dir))
					{
						dir = temp;
						findAllPaths(vp[i],curr,costOfTravel,dir,beenTo);
					}
				}
			}
		}
	}
}
void Board::computePaths(int tc)
{
	Point<int> cp;	
	vector<Point<int> > beenTo;
	for(int i = 0;i<getMapRowSize();i++)
	{
		for(int y = 0;y<getMapColSize();y++)
		{
			cp = Point<int>(y,i);			
			if(getSquare(cp) != WALL && atIntersection(cp))
			{
				beenTo.push_back(cp);				
				findAllPaths(cp,Path(),tc,NONE,beenTo);					
			}
		}
	}
}

void Board::resetAll()
{
	//Do more here to return everything to 0	
	this->resetMap();
	pc.resetPathScores();
}

Point<int> Board::getFruitSquare(const Point<int> & pacPos, int minDist, int maxDist) 
{	
	random_shuffle(emptySquares.begin(),emptySquares.end());
	for(unsigned int i = 0;i<emptySquares.size();i++)
	{
		int dist = pacPos.manHattan(emptySquares[i] );
		if(dist >= 0 && dist <= maxDist)
			return emptySquares[i];
	}
	
	return Point<int> (-1,-1);
}

void Board::updateFruit(Agent &pacman)
{
	if(!fruit.visible)
	{
		//Actually only values are 0.005 and 0.001
		int numToProduce  = GameAttributes::Instance()->fruitFreq >= 0.002f ? 6 : 3;	
		int stepsWith= GameAttributes::Instance()->MAXSTEPS - 50; //Give at least 50 steps to get the item
		stepsWith = max(1,stepsWith);
		int stepMod =  stepsWith / numToProduce;
		
		if(pacman.steps > 0 && pacman.steps%stepMod == 0)
		//if(randomRange(1,0) <= GameAttributes::Instance()->fruitFreq)
		{
			Point<int> pt = getFruitSquare(pacman.pos,GameAttributes::Instance()->minFruitDistance,GameAttributes::Instance()->maxFruitDistance);
			if(!pt.isBad())
			{
				Logger::Instance()->outputXMLFruit(false,pacman.steps, pt.manHattan(pacman.pos));
				Logger::Instance()->incPair(Logger::FRUIT_CREATED);
				fruit = Item(pt,GameAttributes::Instance()->fruitVal,FRUIT,true);		
			}
		}
	}
	else
	{
		if(fruit.pos == pacman.pos)
		{
			Logger::Instance()->incPair(Logger::FRUIT_EATEN);
			Logger::Instance()->outputXMLFruit(true,pacman.steps,0);
			Logger::Instance()->incPair(Logger::SCORE,fruit.pointsPerItem);
			Logger::Instance()->ate = true;		
			
			fruit.visible = false;
			return;
		}

		//Check Timer
		fruit.autoIncCurrTime();

		if(fruit.currTime >= GameAttributes::Instance()->timeFruit)					
			fruit.visible = false;		
	}
}

double Board::computeBoardScore() const
{
	double score = 0;
	for (unsigned int i = 0; i<map.size(); i++)
	{
		for (unsigned int y = 0; y<map[i].size(); y++)
			score += GameAttributes::Instance()->getSingleSquareCost(map[i][y]);		
	}
	return score;
}

int Board::computePathScore(const Path &p) const
{
	int score = 0;
	for (unsigned int i = 0; i<p.myPath.size(); i++)
		score += GameAttributes::Instance()->getSingleSquareCost(getSquare(p.myPath[i]));
	return score;
}
