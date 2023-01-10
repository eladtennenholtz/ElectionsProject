#pragma once
#include "list.h"
#include <iostream>
#include "civilian.h"

using namespace std;

namespace Voting {
	class Civilian;
	class CivilianList;
	class Region
	{
	protected:
		struct NodeCivilianList { Civilian* civilian; };
		struct NodeSelectedRepresentor { NodeCivilianList* civilian;};
			   		 	  
		int counterPeople = 0; 
		int voteSum = 0; 
		double precentOfVoters = 0.0;
		int numOfDelegetors = 0;
		int serialNumber = 0;
		string name;
		List<NodeCivilianList*> civList;		
		List<NodeSelectedRepresentor*> selectedRepresentorList;
	public:
		static int counter;
		Region();
		Region(const string&name, int size);
		Region(const Region& other);
		virtual ~Region();

		const string & getName()const { return name; }
		int getNumOfDelegators()const { return numOfDelegetors;}
		int getCounterPeople()const { return counterPeople; }
		int getvoteNum()const { return voteSum; }
		void operator=(const Region&);
		void addCivilian(const Civilian&civilian);
		friend ostream& operator<<(ostream&, const Region&);
		void addCivilianToTail(Civilian* civilian);
		int getSerialNumber()const { return serialNumber; }
		bool setCounterPeople(const int people);
		bool setVoteNum(const int voteNum);
		bool setPrecentOfVoters(double precentOfVoters);
		int getPrecentOfVoters()const { return (int)precentOfVoters; }
		Region(ifstream& elecFile,CivilianList* cList, int* sizeArr, int*** arr);
		virtual void dispType(ostream& os)const {};
		virtual void saveToFile(ofstream& outElecFile);
		void saveToFileCivilianId(ofstream& outElecFile);
		void free();
	};

}