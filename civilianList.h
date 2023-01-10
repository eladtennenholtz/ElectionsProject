#pragma once
#include"List.h"
#include"civilian.h"

namespace Voting {
	
	class CivilianList
	{
	private:
		struct CivilianNode {
			Civilian* civilian;
			bool vote = false;			
		};
		List< CivilianNode*> civList;

	public:
		CivilianList();
		~CivilianList();
		
		void displayCivilians();
		bool updateCivilian(int id, Civilian** civilian);
		Civilian* getCivilian() const{			
			auto civNode = civList.getTail();
			return (*civNode).data->civilian;
		}
		void addNodeToList(const Civilian& c);
		Civilian* vote(int partyNum, int id);
		friend ostream& operator<<(ostream&, const CivilianList&);
		Civilian* checkCivilian(int id);
		CivilianNode* getHead() const { return (CivilianNode*)civList.getHead(); }
		void setHead(CivilianNode* inhead);
		CivilianList(ifstream& in);
		void operator=(const CivilianList&);		
		void saveToFile(ofstream& outElecFile);
		void free();
	};
}