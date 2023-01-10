#include "delegateList.h"

namespace Voting {

	DelegateList::DelegateList(){}		
	DelegateList::~DelegateList(){	
		free();
	}

	void DelegateList::free() {
		auto curr = this->delegList.begin();
		while (curr != nullptr)
		{
			delete* curr;
			curr++;
		}
	}

	void DelegateList::operator=(const DelegateList & other) {
		this->delegList = other.delegList;		
	}

	void DelegateList::addNodeToList(NodeRegionDelegateList * node){
		delegList.push_back(node);		
	}	

	void DelegateList::addDelegateToList(Civilian * civilian){
		addNodeToList(new NodeRegionDelegateList{ civilian});	
	}
	void DelegateList::display(int numDelegates){
		auto curr = delegList.begin();		
		int counter = 1;
		int i = 0;
		while (curr != delegList.end() && i < numDelegates) {			
			cout << "Delegate " << ++counter << ":" << endl << ((*curr)->civilian) << endl;
			i++;
			curr++;
		}
	}
	ostream & operator<<(ostream &os, const DelegateList & r){		
		auto curr = r.delegList.begin();
		int i = 1;
		while (curr!= r.delegList.end()){
			os << "delegate number: "<< ++i << " delegate name: " <<(*curr)->civilian->getcivilName()<<endl;
			curr++;
		}
		return os;
	}

	DelegateList::DelegateList(ifstream& in, CivilianList* civilianList) {
		auto curr = delegList.getHead();
		Civilian* civilian;
		int civilianId;
		int size;
		in.read(rcastc(&size), sizeof(int));
		for (int i = 0; i < size; i++)
		{
			in.read(rcastc(&civilianId), sizeof(int));
			civilianList->updateCivilian(civilianId, &civilian);
			curr->data = new NodeRegionDelegateList{ civilian};
			addDelegatorToTailFromFile(curr->data);
		}
	}

	void DelegateList::saveToFile(ofstream& outElecFile)
	{
		try {
			auto curr = delegList.begin();
			int id;
			outElecFile.write(rcastcc(delegList.size()), sizeof(int));
			for (int i = 0; i < delegList.size(); i++)
			{
				id = (*curr)->civilian->getId();
				outElecFile.write(rcastcc(&id), sizeof(int));
				curr++;
			}
			if (outElecFile.good() == false) {
				throw ("*** error writing DelegateList ***");
			}
		}
		catch (const char* ex) {
			throw ex;
		}
	}
	
	void DelegateList::addDelegatorToTailFromFile(NodeRegionDelegateList* node)
	{
		delegList.push_back(node);		
	}
}
