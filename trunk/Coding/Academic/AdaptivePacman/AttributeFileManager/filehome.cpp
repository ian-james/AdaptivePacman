#include"gameWorld.h"
#include"fileController.h"

extern GameWorld gw;

int const FILEDEBUG = 0;

void displayDebug(string line){

	if(FILEDEBUG == 0)
		std::cout << line << endl;
}

int main(int argc,char **argv)
{
	NFC fc;
	
	if(argc < 5){
	
		displayDebug("Error Incorrect Number of parameters");
		return 1;
	}
	else{

		fc.dbFile = argv[1];
		string option(argv[3]);
		fc.indexToChange = atoi(argv[2]);
		displayDebug("We retrieved indexToChange = " +toString(fc.indexToChange));
		displayDebug("We retrieved option = " + option);
		int i = 4;
		if(option == "SPEC"){
			
			while(strcmp(argv[i],"END") != 0)
				fc.vals.push_back(argv[i++]);	
			i++;
		}
		else{
			double start = atof(argv[i++]);
			double end = atof(argv[i++]);
			double inc = atof(argv[i++]);
		
			for(start;start <= end;start += inc)
				fc.vals.push_back(toString(start));
		}
		displayDebug("We have i = " + toString(i) + " argc = " + toString(argc));
		if(i<argc){
			displayDebug("We looking for another parameters");
			bool equal = true;
			if(strcmp(argv[i++],"neq") == 0)
				equal = false;
		
			displayDebug("We are passed the first section");
			int ind;
			string va;
			while(strcmp(argv[i],"-1") != 0){
				ind = atoi(argv[i++]);
				va = argv[i++];
				cout << "We are inserting " << ind << " and value " << va << endl;
				fc.indexValues.insert(make_pair(ind,make_pair(va,equal)));	
			}
		}
	
	}

	if(!fc.loadFileNames()){
		displayDebug("Load Files Failed");
		return 1;
	}
	else
		displayDebug("Load Files complete");
	if(!fc.initController()) {
		displayDebug("Init Failed");
		return 1;
	}
	else
		displayDebug("Init complete");
	if(!fc.createFiles()) {
		displayDebug("Create Failed");
		return 1;
	}
	else
		displayDebug("Create complete");
	if(!fc.writeFileNames()){ 
		displayDebug("Write File Failed");
		return 1;
	}
	else
		displayDebug("Write Files complete");

	fc.closePrograms();
}
