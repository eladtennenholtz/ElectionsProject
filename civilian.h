#pragma once
#include "Utils.h"
#include <fstream>
#include"region.h"

using namespace std;
namespace Voting {
	class Region;
	class Civilian {
		
	private:
		string civilName;
		int id;
		int birthYear;
		Region * region;		
		bool isVote;
		int regionNum;

	public:
		Civilian();
		Civilian(const string& civilName, int id, int birthYear, Region* region);
		Civilian(const Civilian &other);
		~Civilian();
		Civilian(ifstream& in);
		string getcivilName()const { return civilName; }
		int getId()const { return id; }
		int getBirthYear()const { return birthYear; }
		bool setCitName(const string& civilName);
		bool setId(const int id);
		bool setBirthYear(const int birthYear);
		void operator=(const Civilian&);
		friend ostream& operator<<(ostream&, const Civilian&);		
		bool setIsVote(const bool isVote);
		bool getIsVote() const { return isVote; }
		Region* getRegion() const { return region; };
		void saveToFile(ofstream& outElecFile);
		int getRegionNum() const { return regionNum; }
		void setRegionPointer(Region* reg);
	};

}