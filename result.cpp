#include "result.h"

namespace Voting {

	Result::Result() {
	}

	Result::~Result() {
		free();
	}

	void Result::free() {
		auto curr = resListList.begin();
		while (curr != nullptr)
		{
			delete* curr;
			curr++;
		}		
	}

	void Result::addToTail(int numDelegates, int partyNum, ElectionParty* elecparty) {
		resList* node = new resList;
		node->partySerialNum = partyNum;
		node->numOfDelegates = numDelegates;
		node->elecParty = elecparty;		
	}	

	void Result::operator=(const Result& other) {
		resListList = other.resListList;
	}
}



