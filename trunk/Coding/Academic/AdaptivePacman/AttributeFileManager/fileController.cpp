#include"fileController.h"

NFC::NFC(){
	dbFile = "dbFile";
	indexToChange = 0;
}

bool NFC::initController(){

	out.open(dbFile.c_str());
	return out.good();

}

bool NFC::loadFileNames(){

	ifstream ins(dbFile.c_str());
	if(!ins){
		cerr << "Error: unable to open dbFile " << dbFile << endl;
		return false;
	}
	
	string line;
	ins >>  nfiles;
	filenames.clear();

	int i = 0;
	while(ins >> line && i < nfiles){
	 filenames.push_back(line);
	 i++;
	}

	ins.close();

	return true;
}

string NFC::returnStrOnEitherSideOfValue(string file,string& left, string& right){

	int i =0;
	string line,val;
	left.clear();
	right.clear();

	fileVals.clear();
	ifstream ins(file.c_str());
	if(!ins){
		cout << "We couldn't open " << file << endl;
		return "ERR";
	}

	string sep = " ";
	while(ins >> line){
		fileVals.push_back(line);
		
		if(ins.peek() == '\n')
			sep = "\n";
		else
			sep = " ";
	
		if(i == indexToChange)
			val = line;
		else if(i<indexToChange)
			left += (line + sep);
		else		
			right += (line + sep);
		i++;
	}

	ins.close();
	return val;
}

bool NFC::writeFile(int id,string left,string val,string right){

	string nfile = "GA" + toString(id) + ".txt";
	ofstream out(nfile.c_str());
	if(!out) 
		return false;

	if(left.empty())
		 out << val << " " << right;
	else
		out <<left << val << " " << right;


	filenames.push_back(nfile);
	out.close();
	return true;
}

bool NFC::createFiles(){

	string left,right,val;
	int filenum = filenames.size(),num = filenum;
	for(int i = 0;i<filenum;i++){
		
		val = returnStrOnEitherSideOfValue(filenames.at(i),left,right);
		if(val == "ERR" || !change())
			continue;

		for(unsigned int y = 0;y<vals.size();y++){
			
			if(fileVals.at(indexToChange) == vals.at(y)) continue;
			if(!writeFile(num,left,toString(vals.at(y)),right))
				return false;
			num++;	
		}
	}
	return true;
}

bool NFC::writeFileNames(){

	out << filenames.size() << endl;
	copy(filenames.begin(),filenames.end(),ostream_iterator<string> (out,"\n"));
	return true;
}

bool NFC::closePrograms(){
	out.close();	
	return true;
}

bool NFC::changeable(int index,string val){

	if(indexValues.empty()){
		return true;
	}
	multimap<int,pair<string,bool> >::iterator pi = indexValues.find(index);	
	bool notEqual = false;


	string actual = fileVals.at(index);
	
	while(pi!=indexValues.end() && pi->first == index){
	
		if(pi->second.first == actual && pi->second.second)//Equal
		{
			return true;
		}
		
		if(!pi->second.second){//Not EQUAL 
			if(pi->second.first == actual){
				return false;
			}
			notEqual = true;
		}
		pi++;
	}

	return notEqual;
}

bool NFC::change(){

	multimap<int,pair<string,bool> >::iterator pi = indexValues.begin();

	while(pi != indexValues.end()){
	
		if(!changeable(pi->first,pi->second.first))
			return false;
		pi++;
	}
	return true;
}
