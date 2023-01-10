#include "regionList.h"
namespace Voting {

	RegionList::RegionList() {
	}

	RegionList::~RegionList()	{	
		free();
	}

	void RegionList::free() {
		auto curr = regList.begin();
		while (curr != nullptr)
		{
			delete* curr;
			curr++;
		}		
	}

	void RegionList::addNode(int num){
		while (num>=1) {
			NodePerRegion* item = new NodePerRegion();
			regList.push_back(item);			
			num--;			
		}
	}

	void RegionList::operator=(const RegionList & other){
		this->regList = other.regList;		
	}

	void RegionList::addNodeToList(const NodePerRegion* node){
		NodePerRegion * item = new NodePerRegion;
		regList.push_back(item);
	}

	void RegionList::addDelegate(Civilian * civilian, int regionNum){
		NodePerRegion* curr = *regList.findByIndex(regionNum);		
		curr->repList.addDelegateToList(civilian);
	}

	void RegionList::displayDelegates(int numDelegates, int regionNum) {
		NodePerRegion* curr = *regList.findByIndex(regionNum);		
		curr->repList.display(numDelegates);	
	}
	
	ostream & operator<<(ostream &os, const RegionList & l) {		
		int counter = 1;
		auto curr = l.regList.begin();
		while (curr != l.regList.end()) {
			os << "------------------------------------------------------" << endl;			
			os << "region number: " << counter++ << " - " << (*curr)->repList << endl;			
			curr++;
		}
		return os;
	}

	void RegionList::loadFromFile(ifstream& in, CivilianList* civilList) {		
		auto curr = regList.getHead();
		DelegateList* repList;
		int size;
		in.read(rcastc(&size), sizeof(int));
		for (int i = 0; i < size; i++) {
			repList = new DelegateList(in, civilList);
			curr->data = new NodePerRegion{ *repList};
			addNodeToTailFromFile(curr->data);
		}
	}

	void RegionList::addNodeToTailFromFile(NodePerRegion* node)
	{
		regList.push_back(node);		
	}


	RegionList::RegionList(RegionList& other) {
		this->regList = other.regList;
	}	

	void RegionList::saveToFile(ofstream& outElecFile) {
		try	{
			auto curr = regList.begin();

			outElecFile.write(rcastcc(regList.size()), sizeof(int));
			for (int i = 0; i < regList.size(); i++) {
				(*curr)->repList.saveToFile(outElecFile);
				curr++;
			}
			if (outElecFile.good() == false) {
				throw ("*** error writing RegionList ***");
			}
		}
		catch (const char* ex) {
			throw ex;
		}
	}

}
