#pragma once
#include"List.h"
#include <fstream>
#include "civilian.h"
#include"civilianList.h"

namespace Voting {
	class DelegateList {
	private:
		struct NodeRegionDelegateList {
			Civilian* civilian;			
		};
		List< NodeRegionDelegateList*> delegList;	
	public:
		DelegateList();		
		~DelegateList();
		DelegateList(ifstream& in, CivilianList* civilianList);
		void operator=(const DelegateList& other);		
		friend ostream& operator<<(ostream&, const DelegateList&);
		void display(int numDelegates);
		void addNodeToList(NodeRegionDelegateList* node);
		void addDelegateToList(Civilian* civilian);
		void saveToFile(ofstream& outElecFile);		
		void addDelegatorToTailFromFile(NodeRegionDelegateList* node);
		void free();
	};

}