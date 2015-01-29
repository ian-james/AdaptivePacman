#ifndef NFILECONTROLLER_H
#define NFILECONTROLLER_H

#include "headers.h"

class NFC
{
public:
	NFC();
	bool initController();
	bool createFiles();
	string returnStrOnEitherSideOfValue(string file,string &left,string& right);
	bool loadFileNames();
	bool writeFileNames(); 	
	bool closePrograms();
	bool openFiles(string filename);
	bool writeFile(int id,string left,string val,string right);
	bool changeable(int index,string val);
	bool change();

	string dbFile;
	int indexToChange,randomIndex,idIndex;
	bool range;
	multimap<int,pair<string,bool> > indexValues;
	vector<string> vals;
	vector<string> fileVals;
	vector<string> filenames;
	int nfiles;
	ofstream out;
};

#endif
