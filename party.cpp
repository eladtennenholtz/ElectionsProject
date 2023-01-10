#include "party.h"
#include "Utils.h"
namespace Voting
{
	
	int Party::counter = 0;
	Party::Party(const string&_name, int _id,Civilian *_civilian,int _numofRegions) {
		this->serialNumber = ++counter;
		this->name = _name;
		this->candidate = _civilian;
		reglist.addNode(_numofRegions);
	}

	

	Party::Party(const Party & other){
		name = nullptr;
		*this = other;
	}	

	void Party::operator=(const Party& other) {		
		name = other.name;
		serialNumber = other.serialNumber;
		candidate = other.candidate;
		reglist = other.reglist;	
	}

	Party::~Party() {		
	}	

	Party::Party(ifstream& in, CivilianList* civilList) {		
		int civilId;
		Civilian* civil = nullptr;
		in.read(rcastc(&serialNumber), sizeof(int));

		int nameLen;
		in.read(rcastc(&nameLen), sizeof(int));
		char* temp = new char[nameLen + 1];
		in.read(rcastc(temp), sizeof(char) * nameLen);
		temp[nameLen] = '\0';
		this->name = temp;
		delete[] temp;
		
		in.read(rcastc(&civilId), sizeof(int));		
		civilList->updateCivilian(civilId, &civil);
		candidate = civil;
		reglist.loadFromFile(in, civilList);
	}

	void Party::saveToFile(ofstream& outElecFile) {
		try {
			outElecFile.write(reinterpret_cast<const char*>(&serialNumber), sizeof(serialNumber));
			int nLen = name.size();
			outElecFile.write(reinterpret_cast<const char*>(&nLen), sizeof(int));
			outElecFile.write(name.c_str(), sizeof(char) * nLen);
			int id = candidate->getId();
			outElecFile.write(reinterpret_cast<const char*>(&id), sizeof(int));
			reglist.saveToFile(outElecFile);
			if (outElecFile.good() == false) {
				throw ("*** error writing Party ***");
			}
		}
		catch (const char* ex) {
			throw ex;
		}
	}

	ostream& operator<<(ostream& os, const Party& party) {
		os << "------------------------------------------------" << endl;
		os << "party number: " << party.serialNumber << " name: " << party.name << endl << "party candidate name: " << party.candidate->getcivilName() << endl<< party.reglist;
		return os;
	}
	
	

	

}