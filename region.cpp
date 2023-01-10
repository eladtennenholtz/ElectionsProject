#include "region.h"
#include "Utils.h"

namespace Voting {

		int Region::counter = 0;

		Region::Region() {			
		}

		Region::Region(const string&name, int size) {
			this->numOfDelegetors = size;
			this->serialNumber = ++counter;
			this->name = name;			
			this->counterPeople = 0;
			this->voteSum = 0;
			this->precentOfVoters = 0;
		}

		Region::Region(const Region & other){

			name = nullptr;
			*this = other;
		}


	Region::~Region() {
		free();
	}
	void Region::free() {
		auto curr = civList.begin();
		while (curr != nullptr)
		{
			delete* curr;
			curr++;
		}
		auto curr1 = selectedRepresentorList.begin();
		while (curr1 != nullptr)
		{
			delete* curr1;
			curr1++;
		}		
	}
	

	void Region::operator=(const Region& other) {		
		name = other.name;
		serialNumber = other.serialNumber;
		numOfDelegetors = other.numOfDelegetors;
		counterPeople = other.counterPeople;
		voteSum = other.voteSum;
		precentOfVoters = other.precentOfVoters;
		civList = other.civList;
		selectedRepresentorList = other.selectedRepresentorList;		
	}

	void Region::addCivilian(const Civilian & c){
		NodeCivilianList* item = new NodeCivilianList;
		item->civilian = new Civilian(c);
		civList.push_back(item);		
	}
	
	void Region::addCivilianToTail(Civilian * civilian){
		NodeCivilianList * item = new NodeCivilianList;
		item->civilian = civilian;
		this->setCounterPeople(1);
		civList.push_back(item);		
	}

	bool Region::setCounterPeople(const int people){
		if (people >= 0) {
			this->counterPeople++;
			return true;
		}
		else {
			return false;
		}
	}

	bool Region::setVoteNum(const int voteNum){
		if (voteNum >= 0) {
			this->voteSum = voteNum;
			return true;
		}
		else {
			return false;
		}
	}

	bool Region::setPrecentOfVoters(double precentOfVoters){

		if (precentOfVoters >= 0 && precentOfVoters <= 100) {
			this->precentOfVoters = precentOfVoters;
			return true;
		}
		return false;
	}

	ostream& operator<<(ostream& os, const Region& region)
	{
		
		os << "------------------------------------------------------------" << endl;
		os << "Region number:" << region.serialNumber << endl << " name: " << region.name << endl;		
		region.dispType(os);
		os << "number of delegators " << region.numOfDelegetors << endl;
		return os;
	}

	Region::Region(ifstream& elecFile, CivilianList* v, int* sizeArr, int*** delegs)  {
		//NodeCivilianList* curr = civList.headCivilian;		
		int civilianId, sizeOfpeople;		
		counterPeople = 0;

		elecFile.read(rcastc(&sizeOfpeople), sizeof(int));
		*sizeArr = sizeOfpeople;
		int* res = new int[sizeOfpeople];
		for (int i = 0; i < sizeOfpeople; i++)
		{
			elecFile.read(rcastc(&civilianId), sizeof(int));
			res[i] = civilianId;
		}
		**delegs = res;
		elecFile.read(rcastc(&voteSum), sizeof(int));
		elecFile.read(rcastc(&precentOfVoters), sizeof(float));
		elecFile.read(rcastc(&serialNumber), sizeof(int));
		elecFile.read(rcastc(&numOfDelegetors), sizeof(int));	
		
		int nameLen;
		elecFile.read(rcastc(&nameLen), sizeof(int));
		char* temp = new char[nameLen + 1];
		elecFile.read(rcastc(temp), sizeof(char) * nameLen);
		temp[nameLen] = '\0';
		this->name = temp;
		delete[] temp;	
		
	}
	 
	void Region::saveToFile(ofstream& outElecFile) {
		try {
			saveToFileCivilianId(outElecFile);

			outElecFile.write(rcastcc(&voteSum), sizeof(int));
			outElecFile.write(rcastcc(&precentOfVoters), sizeof(float));
			outElecFile.write(rcastcc(&serialNumber), sizeof(int));
			outElecFile.write(rcastcc(&numOfDelegetors), sizeof(int));

			int nLen = name.size();
			outElecFile.write(rcastcc(&nLen), sizeof(int));
			outElecFile.write(name.c_str(), sizeof(char) * nLen);
			if (outElecFile.good() == false) {
				throw ("*** error writing Region ***");
			}
		}
		catch (const char* ex) {
			throw ex;
		}		
	}

	void Region::saveToFileCivilianId(ofstream& outElecFile) {
		auto curr = civList.begin();
		int id;
		outElecFile.write(rcastcc(&counterPeople), sizeof(int));
		for (int i = 0; i < counterPeople; i++) {
			id = (*curr)->civilian->getId();
			outElecFile.write(rcastcc(&id), sizeof(int));
			curr++;
		}
	}






}