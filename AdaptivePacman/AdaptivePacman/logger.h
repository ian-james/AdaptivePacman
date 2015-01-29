#ifndef LOGGER
#define LOGGER

#include"headers.h"
#include"point.h"

class Logger
{
public:
	static Logger* Instance();

	void openFile(string outputFile);		
	void closeFile();

	void initStrings();
	
	int gameID;	
	int life;
	int timeSinceEaten;

	enum Stats { CLOSE_CALLS,REP_SQR,SCORE,FRUIT_CREATED,FRUIT_EATEN,STEPS,LVL_COMP,TOKENS,PPS,GHOSTS,ALL};
	const char* statNames[ALL];


	void incPair(Stats s,int value = 1);
	void setPair(Stats s,int value = 0);
	pair<int,int> getPair(Stats s);

	void writeToFile(string s);
	void outputXMLValue(const std::string & str, const std::string & value);
	void outputXMLStep(const int & step, const int & cc, const int & rep);
	void outputXMLFruit(bool eat,int step, int dist);
	void outputStat(const std::string & str, const std::string & value);

	void resetLifeInfo();
	void resetGameInfo();

	void outputLife();
	void outputGame();
	

	//First = curr live, second is total for game
	pair<int,int> arrStats[ALL];


	bool ate;	

private:
	Logger();
	~Logger();
	Logger(const Logger&);
	Logger& operator=(const Logger&);

	string outFile;
	ofstream out;
	bool initFile;
	bool init;
};
#endif

