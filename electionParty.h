#pragma once
#include "list.h"
#include "party.h"
#include"dividedRegion.h"
#include"unifiedRegion.h"

namespace Voting {
	class Region;
	class ElectionParty {

	private:
		struct PartyVote {
			Party* party;
			int counter;
			double votePercentage;
			int numOfDelegator;			
		};
		double regionVotes;		
		int serialNumber;
		List<PartyVote*> partyVotes;
	public:
		static bool isLoaded;
		ElectionParty();
		~ElectionParty();
		ElectionParty(ElectionParty& other);
		ElectionParty(ifstream& in);
		static int counterElecParty;
		List<PartyVote*> getPartyVotes() { return partyVotes;}
		void addPartyNodeToTail(Party* party);
		void updatePartyVote(int partyNum);
		int countVotes();
		bool setRegionVotes(const int regionVotes);
		void updatePercentage();
		void displayParty(Region* region);
		Party* displayCandidate();
		void displayDelegators(Party*p,int numDelegates,int regionNum);
		double* updateDelegatorsPerParty(int numOfDelegators,int voteNum,double* arr,int size);		
		Party* getWinParty();
		void displayRegular(Region& region);
		void displaySimple(Region& region);
		void updateElectionPartyNodes(Region& region);
		void updateNumOfDelegators(double* arr, int size);
		static int CompareDelegators(const void* deleg1, const void* deleg2);
		void displayPartyContent(PartyVote** arr, int size, Region& region);
		void printDelegators(Party* party, int numDelegators, int regionNum);
		static int ComparePartyVotes(const void* vote1, const void* vote2);
		void saveToFile(ofstream& outElecFile);
		void addPartyVoteToTail(PartyVote* node);
		void setNodeVoteContent(int j, Party* party, int numVotesPerParty, double  percent, int  numofDelegators);
		ElectionParty(ElectionParty& other, bool isFromFile);	
		void free();
	};
}
