#include "civilianList.h"

namespace Voting {

	CivilianList::CivilianList() {}

	CivilianList::~CivilianList(){
		free();		
	}

	void CivilianList::free() {
		auto curr = civList.begin();
		while (curr != nullptr)
		{				
			delete *curr;
			curr++;
		}
	}

	void CivilianList::addNodeToList(const Civilian & civilian) {		
		CivilianNode* item = new CivilianNode();
		item->vote = false;
		item->civilian = new Civilian(civilian);		
		civList.push_back(item);		
	}
	   	 
	Civilian* CivilianList::vote(int partyNum, int id) {
		auto curr = civList.begin();		
		while (curr != civList.end())
		{			
			if (((*curr)->civilian->getId()) == id){
				(*curr)->vote = true;
				return (*curr)->civilian;
			}
			curr++;
		}		
		return nullptr;
	}

	ostream& operator<<(ostream& os, const CivilianList& v) {
		auto curr = v.civList.begin();		
		
		while (curr != v.civList.end()) {
			os << ((*curr)->civilian) << endl;
			curr++;
		}
		return os;
	}

	Civilian* CivilianList::checkCivilian(int id) {
		auto curr = civList.begin();
		while (curr != civList.end())
		{
			if (((*curr)->civilian->getId()) == id) {
				return (*curr)->civilian;
			}
			curr++;
		}
		return nullptr;
	}

	void CivilianList::displayCivilians()
	{
		auto curr = civList.begin();
		while (curr != civList.end())
		{
			Civilian* civ = (*curr)->civilian;
			cout << *civ;
			curr++;
		}
	}

	bool CivilianList::updateCivilian(int id, Civilian**civilian)
	{
		auto curr = civList.begin();
		while (curr != civList.end())
		{
			if ((*curr)->civilian->getId() == id) {
				*civilian = (*curr)->civilian;
				return true;
			}
			curr++;
		}
		return false;
	}

	CivilianList::CivilianList(ifstream& in) : CivilianList() {
		int size = 0;
		in.read(rcastc(&size), sizeof(int));
		bool flag;
		Civilian* civil;
		auto curr = civList.getHead();
		
		for (int i = 0; i < size; i++) {
			civil = new Civilian(in);
			in.read(rcastc(&flag), sizeof(bool));
			curr->data = new CivilianNode{ civil,flag};
			civList.push_back(curr->data);			
		}
	}

	void CivilianList::operator=(const CivilianList& other)
	{
		this->civList = other.civList;		
	}

	void CivilianList::setHead(CivilianNode* newHead)
	{
		this->civList.push_front(newHead);		
	}	

	void CivilianList::saveToFile(ofstream& outElecFile) {	
		try {
			outElecFile.write(reinterpret_cast<const char*>(civList.size()), sizeof(int));
			auto curr = civList.begin();
			while (curr != civList.end()) {
				(*curr)->civilian->saveToFile(outElecFile);
				outElecFile.write(reinterpret_cast<const char*>(&(*curr)->vote), sizeof(bool));
				curr++;
			}
			if (outElecFile.good() == false) {
				throw ("*** error writing CivilianList ***");
			}
		}
		catch (const char* ex) {
			throw ex;
		}
	}	
}