#include "civilian.h"
#include "region.h"

namespace Voting {
	Civilian::Civilian(){
	}

	Civilian::Civilian(const string& name, int _id, int _birthYear, Region* _region) {

		civilName = name;
		id = _id;
		birthYear = _birthYear;		
		region = _region;
		isVote = false;
	}

	Civilian::Civilian(const Civilian & other){
		*this = other;
	}

	Civilian::~Civilian() {		
	}


	bool Civilian::setCitName(const string& civilName) {
		
		this->civilName = civilName;
		return true;
	}

	bool Civilian::setId(const int id) {
		this->id = id;
		return true;
	}

	bool Civilian::setBirthYear(const int year)
	{
		birthYear = year;
		return true;
	}

	
	void Civilian::operator=(const Civilian &other) {		
		civilName =other.civilName;
		id = other.id;
		birthYear = other.birthYear;
		region = other.region;
		isVote = other.isVote;
	}	
	
	bool Civilian::setIsVote(const bool isVote) {
		this->isVote = isVote;
		return true;
	}

	
	std::ostream& operator<<(std::ostream& os,  Civilian const& civilian)
	{
		os << "--------------------------------------" << endl;
		os << "Id:" << civilian.getId()<<endl;
		os << "Name:" << civilian.getcivilName()<<endl;		
		os << "Year of birth: " << civilian.getBirthYear()<<endl;
		os << "Region: " << civilian.getRegion()->getName()<<endl;
		
		return os;
	}

	Civilian::Civilian(ifstream& in) {
		int nameLen;
		in.read(rcastc(&nameLen), sizeof(int));
		char* temp = new char[nameLen + 1];		
		in.read(rcastc(temp), sizeof(char) * nameLen);
		temp[nameLen] = '\0';
		this->civilName = temp;
		delete[] temp;
		in.read(rcastc(&id), sizeof(int));
		in.read(rcastc(&birthYear), sizeof(int));
		in.read(rcastc(&regionNum), sizeof(int));
		in.read(rcastc(&isVote), sizeof(bool));
	}

	void Civilian::saveToFile(ofstream& outElecFile) {
		try {
			int nLen = civilName.size();
			outElecFile.write(reinterpret_cast<const char*>(&nLen), sizeof(int));
			outElecFile.write(civilName.c_str(), sizeof(char) * nLen);
			outElecFile.write(reinterpret_cast<const char*>(&id), sizeof(int));
			outElecFile.write(reinterpret_cast<const char*>(&birthYear), sizeof(int));
			regionNum = region->getSerialNumber();
			outElecFile.write(reinterpret_cast<const char*>(&regionNum), sizeof(int));
			outElecFile.write(reinterpret_cast<const char*>(&isVote), sizeof(bool));
			if (outElecFile.good() == false) {
				throw ("*** error writing civilian ***");
			}
		}
		catch (const char* ex) {
			throw ex;
		}

		
	}

	void Civilian::setRegionPointer(Region* reg)
	{
		region = reg;
	}
}