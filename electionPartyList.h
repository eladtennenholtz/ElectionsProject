#pragma once
#include "list.h"
#include"electionParty.h"
#include"result.h"

namespace Voting {
	class Region;
	class ElectionPartyList {
	private:
		struct VoteList {
			Region  *region;
			ElectionParty vote;
			Party* winParty;			
		};
		Result* result;
		List<VoteList*> voteList;
	public:
		ElectionPartyList() { result = nullptr; };
		~ElectionPartyList() {
			free();
		};
		void addNode(Region* region);
		void addNodeParty(Party*p);
		void updateVotes(int region, int party);
		void countVotes();
		void updatePercentageInParty();
		void displayRegularElection();
		void displaySimpleElection();
	 	void buildResult(int numDelegates, int partyNum, ElectionParty* electionParty);
		Result* getResult()const { return result; }
		void saveToFile(ofstream& outElecFile, bool electionType);		
		void createNodeList(Region* region, ElectionParty& elecParty, Party* party);
		void addNodeFromFile(VoteList* node);
		void operator=(const ElectionPartyList& other);	
		
		List<VoteList*> getVoteList() const { return voteList; }
		void free() {
			auto curr = voteList.begin();
			while (curr != nullptr)
			{
				delete* curr;
				curr++;
			}
		}
	};

}
