#pragma once
#include"List.h"
#include<fstream>
#include "delegateList.h"

namespace Voting {
	class RegionList
	{
	private:
		struct NodePerRegion {
			DelegateList repList;
			
		};
		List<NodePerRegion*> regList;
	public:
		RegionList();
		~RegionList();
		RegionList(RegionList& other);
		void addNode(int num);
		void operator=(const RegionList& other);
		void addNodeToList(const NodePerRegion* node);
		void addDelegate(Civilian*civilian, int regionNum);
		friend ostream& operator<<(ostream&, const RegionList&);
		void displayDelegates(int numDelegates,int regionNum);
		void saveToFile(ofstream& outElecFile);		
		void loadFromFile(ifstream& in, CivilianList* civilList);
		void addNodeToTailFromFile(NodePerRegion* node);
		void free();
	};
}

