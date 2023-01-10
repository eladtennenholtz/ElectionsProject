#pragma once
#include "list.h"
#include <fstream>
namespace Voting {
	class ElectionParty;
	class Result {
	private:

		struct resList {
			int  partySerialNum = 0;
			int numOfDelegates = 0;
			ElectionParty* elecParty;			
		};
		List<resList*> resListList;
		
	public:
		Result();
		~Result();
		void addToTail(int numDelegates, int partyNum, ElectionParty* elecparty);			
		void operator=(const Result&);
		List<resList*> getResList() { return resListList; };
		void free();
	};

}

