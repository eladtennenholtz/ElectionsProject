#include "ElectionPartyList.h"

namespace Voting {
		
	void ElectionPartyList::operator=(const ElectionPartyList& other) {
		result = other.result;
		this->voteList = other.voteList;		
	}


	void ElectionPartyList::addNode(Region* region) {
		VoteList* node = new VoteList();
		node->region = region;
		voteList.push_back(node);
	}

	void ElectionPartyList::addNodeParty(Party * party) {
		auto curr = voteList.begin();
		while (curr != nullptr) {
			(*curr)->vote.addPartyNodeToTail(party);
			curr++;
		}
	}
	void ElectionPartyList::updateVotes(int regionNum, int partyNum){		
		VoteList* curr = *voteList.findByIndex(regionNum);		
		curr->vote.updatePartyVote(partyNum);
	}

	void ElectionPartyList::countVotes(){
		auto curr = voteList.begin();
		int numOfVotes;
		while (curr != nullptr) {
			numOfVotes = (*curr)->vote.countVotes();
			(*curr)->region->setVoteNum(numOfVotes);
			curr++;
		}
	}

	void ElectionPartyList::updatePercentageInParty(){
		auto curr = voteList.begin();
		int voteNum;
		while (curr != nullptr) {
			voteNum= (*curr)->region->getvoteNum();
			(*curr)->vote.setRegionVotes(voteNum);
			(*curr)->vote.updatePercentage();
			curr++;
		}
	}
	 void ElectionPartyList::buildResult(int numDelegates, int partyNum, ElectionParty* electionParty){
		 this->result->addToTail(numDelegates,partyNum, electionParty);
	 }

	 void ElectionPartyList::displayRegularElection() {
		 auto curr = voteList.begin();
		 Region* currRegion = (*curr)->region;
		 Party* party;

		 result = new Result;
		 int numDeleg;
		 int partySerialNum;
		 while (curr != nullptr) {

			 cout << *((*curr)->region) << endl;

			 currRegion = dynamic_cast<UnifiedRegion*>(currRegion);
			 party = (*curr)->vote.getWinParty();
			 (*curr)->winParty = party;
			 if (party) {
				 (*curr)->vote.displayRegular(*((*curr)->region));

				 if (currRegion) {
					 numDeleg = (*curr)->region->getNumOfDelegators();
					 partySerialNum = (*curr)->winParty->getSerialNumber();
					 buildResult(numDeleg, partySerialNum, &((*curr)->vote));
				 }
				 else {

					 buildResult(0, 0, &((*curr)->vote));

				 }
			 }
			 else {
				 cout << "*******************************" << endl;
				 cout << "Empty votes for this region" << endl;
				 cout << "*******************************" << endl;
			 }
			 cout << "Region: " << (*curr)->region->getName() << " Percentage vote are: " << (*curr)->region->getPrecentOfVoters() << " percent" << endl << endl;
			 cout << "-----------------------" << endl;


			 curr++;
			 if (curr != nullptr) {
				 currRegion = (*curr)->region;
			 }
		 }
	 }

	 void ElectionPartyList::displaySimpleElection() {
		 auto curr = voteList.begin();
		 DividedRegion* currRegin = (DividedRegion*)(*curr)->region;
		 Party* party;

		 cout << "Region: " << (*curr)->region->getName() << " Percentage vote are: " << (*curr)->region->getPrecentOfVoters() << " percent" << endl << endl;
		 party = (*curr)->vote.getWinParty();
		 (*curr)->winParty = party;
		 if (party) {
			 (*curr)->vote.displaySimple(*((*curr)->region));
		 }
		 else {
			 cout << "*******************************" << endl;
			 cout << "Empty votes for this region" << endl;
			 cout << "*******************************" << endl;
		 }
	 }

	 void ElectionPartyList::saveToFile(ofstream& outElecFile, bool electionType) {
		 try {
			 auto curr = voteList.begin();
			 int regionId, partyId;

			 for (int i = 0; i < voteList.size(); i++) {
				 regionId = (*curr)->region->getSerialNumber();
				 outElecFile.write(rcastcc(&regionId), sizeof(int));				 
				 (*curr)->vote.saveToFile(outElecFile);
				 if ((*curr)->winParty != nullptr) {
					 partyId = (*curr)->winParty->getSerialNumber();
				 }
				 outElecFile.write(rcastcc(&partyId), sizeof(int));
				 curr++;
			 }
			 if (outElecFile.good() == false) {
				 throw ("*** error writing ElectionPartyList ***");
			 }
		 }
		 catch (const char* ex) {
			 throw ex;
		 }
	 }	 
	 
	 void ElectionPartyList::createNodeList(Region* region, ElectionParty& elecParty, Party* party)
	 {		 
		 elecParty.isLoaded = true;
		 VoteList* curr = new VoteList{ region,elecParty,party };
		 addNodeFromFile(curr);
	 }

	 void ElectionPartyList::addNodeFromFile(VoteList* node) {
		 voteList.push_back(node);
	 }

	 

	
}