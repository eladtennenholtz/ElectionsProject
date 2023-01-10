#pragma once
#include "civilian.h"
#include "regionList.h"
#include <fstream>


namespace Voting {

	class Party {
	private:
		int serialNumber;
		string name;
		Civilian * candidate;
		RegionList reglist;

	public:
		static int counter;
		Party(const string& _name, int _id, Civilian* civilian, int numofRegions);		
		Party(const Party& other);
		~Party();
		Party(ifstream& in, CivilianList* civilList);

		string getName() const { return name; }
		int getSerialNumber() const { return serialNumber; }
		Civilian *getCandidate() const { return candidate; }
		void operator=(const Party& other);
		friend ostream& operator<<(ostream&, const Party&);
		RegionList& getRegList() { return reglist; }	
		void saveToFile(ofstream& outElecFile);		
	};

}