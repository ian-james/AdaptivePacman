#include"logger.h"


Logger::Logger()
{
	initFile = false;
	ate = false;
	init = false;
	initStrings();
}

Logger* Logger::Instance()
{
	static Logger logger;
	return &logger;
}
void Logger::initStrings()
{
	if(!init)
	{
		int c = 0;
		
		statNames[c++] = "CC";
		statNames[c++] = "RSQ";
		statNames[c++] = "Sc";
		statNames[c++] = "FC";
		statNames[c++] = "FE";
		statNames[c++] = "St";
		statNames[c++] = "LVL_COMP";
		statNames[c++] = "TO";
		statNames[c++] = "PP";
		statNames[c++] = "GH";

		init = true;
	}
}

void Logger::openFile(string outputFile)
{
	if(!initFile)
	{
		out.open(outputFile.c_str());
		if(!out)
		{
			closeFile();
			outputError("Error opening logging file = '" + outputFile + "'\n",true);	
		}		

		outFile = outputFile;
		initFile = true;
	}
}

void Logger::writeToFile(string s)
{
	out << s << "\n";
}

Logger::~Logger()
{
	closeFile();
}

void Logger::closeFile()
{
	if(initFile)
		out << "END";
	out.clear();
	out.close();
	initFile = false;
}

void Logger::outputXMLValue(const std::string & str, const std::string & value)
{
	out << "<P=" << str << "=" << value << ">\n";
}

void Logger::outputStat(const std::string & str, const std::string & value)
{
	out << "<STAT=" << str << "=" << value << ">\n";
}

void Logger::outputXMLStep(const int & step,const int & cc,const int & rep)
{
	out << "<St=" << step << " cc=" << cc << " rep=" << rep <<  ">\n";
}

void Logger::incPair(Logger::Stats s,int value)
{
	if(s >= 0 && s < this->ALL)
	{
		arrStats[int(s)].first += value;
		arrStats[int(s)].second += value;
	}
}

void Logger::setPair(Logger::Stats s,int value)
{
	if(s >= 0 && s < this->ALL)
	{
		arrStats[int(s)].first = value;
		arrStats[int(s)].second = value;
	}
}

pair<int,int> Logger::getPair(Logger::Stats s)
{
	return arrStats[int(s)];
}

void Logger::resetLifeInfo()
{
	for(int i  = 0;i<ALL;i++)
		arrStats[i].first = 0;
}

void Logger::resetGameInfo()
{
	for(int i  = 0;i<ALL;i++)	
		this->setPair(Logger::Stats(i),0);	
}

void Logger::outputXMLFruit(bool eat, int step, int dist)
{
	out << "<P=FRUIT St=" << step  << "  Ate=" << (eat?1:0) << " Dist=" << dist << ">\n";
}


void Logger::outputLife()
{
	for(Stats s = CLOSE_CALLS;s < ALL;s= Stats(int(s)+1))
		outputStat(this->statNames[int(s)],toString(this->getPair(s).first));
}
void Logger::outputGame()
{
	for(Stats s = CLOSE_CALLS;s < ALL;s= Stats(int(s)+1))
		outputStat(this->statNames[int(s)],toString(this->getPair(s).second));
}

