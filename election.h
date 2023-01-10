#pragma once
#include <fstream>
#include "civilianList.h"
#include "unifiedRegion.h"
#include "dividedRegion.h"
#include "party.h"
#include "ElectionPartyList.h"

namespace Voting {
	class Election
	{
		protected:
		struct Date { int day, month, year; };
		struct NodeRegionList { Region* region;};
		struct NodePartyList { Party* party; int numOfVotes; int numOfDelegators;};
		struct NodeResPartyList { Party* party; int numOfVotes; int numOfDelegators;};
		struct ElectionPartyNode { ElectionParty* elecParty;}; 
		Date date;
		List<NodeRegionList*> regionList;
		List<NodePartyList*> partyList;
		List<NodeResPartyList*> resPartyList;
		List<ElectionPartyNode*> electionPartyList;
		ElectionPartyList voteList;
	public:

		Election(int _d, int _m, int _y);
		virtual ~Election();
		Date getDate() { return date; }
		void addPartyToPartyList(Party & party);
		void displayRegions() const;
		void displayParties() const;
		void addRegionToList(Region* region);		
		void updatePercentageInRegions();
		void updatePercentageInParty();
		virtual void display();
		void printResultPartyList() const;  
		void buildResultList();
		void updatePartyVotesLinkedList(int partyNum);
		void updatePartyDelegatorsLinkList();
		void updateResult(int numDelegates,int partySerialNumber);
		void addNodeToListResList(NodePartyList* node);
		void updateParty(Civilian* civil, int partyNum, int regionNum = 1);
		void addRegionToPartyList();
		int getNumOfRegions()const;
		Region* findRegion(int regionNumber) const;
		void addCivilianToRegion(CivilianList& civilianslist);
		void addNodeToVoteList();
		void addPartyPerRegion();
		void updateVotes(Civilian* civilian, int partyNum);
		void displayResults();
		Election(ifstream& elecFile, CivilianList* v, bool flag);
		void operator=(const Election& other);
		void updateDividedResult(ElectionParty& elecParty);
		friend ostream& operator<<(ostream&, const Election& election);
		virtual void dispElectionType(ostream& os)const;
		void saveToFile(ofstream& outElecFile, bool electionType);		
		void copyPartyList(ofstream& outElecFile);
		void copyRegionList(ofstream& outElecFile);
		void addElectionPartyToTail(ElectionParty& v);
		void copyElectionPartyList(ofstream& outElecFile);
		void loadFromFileRegionList(ifstream& in, CivilianList* civList);
		void loadFromFilePartyList(ifstream& in, bool regularElection, CivilianList* civList);
		void addNodeToTailPartyListloadFromFile(NodePartyList* node);
		void loadFromFileElectionPartyList(ifstream& elecFile, bool electionType);		
		void loadFromFileLinkListVoteSys(ifstream& elecFile, bool electionType);
		Party* findPartyFromResParty(int partyId);
		Party* findPartyFromParty(int partyId);
		ElectionParty& findVoteSys(int index);
		void addNodeToTailPartyList(NodeResPartyList* node);
		NodePartyList* alocateNode(NodePartyList* node);
		void updateNumDelegatorsResultPartyList();
		ElectionPartyList& getVoteList() { return voteList; }		
		void updateRegionsInCivilians(CivilianList* civList);
		void addNodeToTailRegionListFromFile(NodeRegionList* node);
		void addNodeToTailPartyListFromFile(NodePartyList* node);
		void addNodeToTailPartyResListFromFile(NodeResPartyList* node);
		void addElectionPartyToTailFromFile(ElectionPartyNode* node);	
		List<NodeRegionList*> getRegionList() const { return regionList; }
		void free();
	};



}