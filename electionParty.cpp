#include "electionParty.h"
namespace Voting {

	int ElectionParty::counterElecParty = 0;
	bool ElectionParty::isLoaded = false;

	ElectionParty::ElectionParty() {
		regionVotes = 0;		
		this->serialNumber = ++counterElecParty;
	}

	ElectionParty::~ElectionParty()
	{	
		free();
	}
	void ElectionParty::free() {
		auto curr = partyVotes.begin();
		while (curr != nullptr)
		{
			delete* curr;
			curr++;
		}
	}

	void ElectionParty::addPartyNodeToTail(Party * party) {		
		PartyVote* node = new PartyVote();
		node->party = party;
		node->counter = 0;
		partyVotes.push_back(node);
	}

	void ElectionParty::updatePartyVote(int partyNum) {		
		auto curr = partyVotes.begin();
		while (curr != nullptr) {
			if ((*curr)->party->getSerialNumber() == partyNum) {
				(*curr)->counter = (*curr)->counter + 1;
				break;
			}
			curr++;
		}
	}

	int ElectionParty::countVotes() {
		int voteNum = 0;
		auto curr = partyVotes.begin();
		while (curr != nullptr)
		{
			voteNum += (*curr)->counter;
			curr++;
		}
		return voteNum;
	}

	bool ElectionParty::setRegionVotes(const int regionVotes) {
		if (regionVotes >= 0) {
			this->regionVotes = regionVotes;
			return true;
		}
		return false;
	}
	void ElectionParty::updatePercentage() {
		auto curr = partyVotes.begin();
		double num1, num2;
		double p;
		while (curr != nullptr) {
			num1 = (*curr)->counter;
			num2 = regionVotes;
			p = (num1 / num2);
			(*curr)->votePercentage = p * 100;
			curr++;
		}
	}
	void ElectionParty::displayParty(Region* region) {
		auto curr = partyVotes.begin();
		double votesPerParty;
		int counter = 0, size;
		double * arr,*resArr;
		while (curr != nullptr) {
			counter++;
			curr++;
		}
		curr = partyVotes.begin();
		size = counter ;
		arr = new double[size];
		counter = 0;
		while (curr != nullptr) {
			votesPerParty = (*curr)->counter;
			arr[counter]= votesPerParty;
			counter++;
			curr++;
		}
		resArr=updateDelegatorsPerParty(region->getNumOfDelegators(),region->getvoteNum(),arr,size);
		counter = 0;
		curr = partyVotes.begin();
		while (curr != nullptr) {
			cout << "--------------------------------------------" << endl;
			cout << "The current Party " << (*curr)->party->getName()<< " has " << (*curr)->counter << " votes and "<< (*curr)->votePercentage << " percentage" << endl;
			cout << "num of delegators in the party is: " << resArr[counter]<<endl;
			cout << endl;
			counter++;
			curr++;
		}
		delete resArr;
	}
	Party* ElectionParty::displayCandidate() {
		Party *party = nullptr;
		auto curr = partyVotes.begin();
		int max = 0, voteNum = 0;
		while (curr != nullptr) {
			voteNum = (*curr)->counter;
			if (voteNum > max) {
				max = voteNum;
				party = (*curr)->party;
			}
			curr++;
		}

		return party;
	}

	void ElectionParty::displayDelegators(Party * party, int numDelegates,int regionNum) {
		party->getRegList().displayDelegates(numDelegates, regionNum);
	}

	double * ElectionParty::updateDelegatorsPerParty(int numOfDelegators, int voteNum, double * arr, int size){
		double * res = new double[size];
		for (int c = 0; c < size; c++){
			res[c] = 0;
		}
		double elector = voteNum / numOfDelegators;
		int maxIndex = 0;
		double max=0;
		for (int i = 0; i < numOfDelegators; i++){
			for (int j = 0; j < size; j++){
				if (arr[j] > max) {
					max = arr[j];
					maxIndex = j;	
				}
			}
			arr[maxIndex] = arr[maxIndex] - elector;
			res[maxIndex]++;
			max = 0;
			maxIndex = 0;
		}
		delete[]arr;
		return res;
	}

	Party* ElectionParty::getWinParty() {
		Party* party = nullptr;
		auto curr = partyVotes.begin();
		int max = 0, voteNum = 0;
		while (curr != nullptr) {
			voteNum = (*curr)->counter;
			if (voteNum > max) {
				max = voteNum;
				party = (*curr)->party;
			}
			curr++;
		}

		return party;
	}

	void ElectionParty::displayRegular(Region& region) {
		auto curr = partyVotes.begin();
		PartyVote** arr;
		Party* party;
		Region* tempRegion;
		int size = 0, i = 0;
		while (curr != nullptr) {
			size++;
			curr++;
		}
		arr = new PartyVote * [size];
		updateElectionPartyNodes(region);
		curr = partyVotes.begin();
		while (curr != nullptr) {
			arr[i] = *curr;
			i++;
			curr++;
		}
		qsort(arr, size, sizeof(PartyVote*), CompareDelegators);
		tempRegion = dynamic_cast<UnifiedRegion*>(&region);
		if (tempRegion) {
			party = arr[0]->party;
			cout << "Name of candidate:  " << party->getCandidate()->getcivilName() << endl;
			displayPartyContent(arr, size, region);
		}
		else {
			cout << "------------------------------------------------------------------------------------------------" << endl;
			for (i = 0; i < size; i++) {
				cout << "***********************************************************************" << endl;
				party = arr[i]->party;
				if (arr[i]->numOfDelegator >= 1) {
					cout <<"Name of candidate:  " << party->getCandidate()->getcivilName() << endl;					
					cout << "The current Party " << arr[i]->party->getName() << " has " << arr[i]->counter << " votes and " << arr[i]->votePercentage << " percentage" << endl;
					cout << "Num of delegators: " << arr[i]->numOfDelegator << endl;
					cout << "***********************************************************************" << endl;
					printDelegators(arr[i]->party, arr[i]->numOfDelegator, i + 1);
					cout << endl;
				}
				else {
					cout << "The current Party " << arr[i]->party->getName() << " has " << arr[i]->counter << " votes and " << arr[i]->votePercentage << " percentage" << endl;
					cout << "Num of delegators: " << arr[i]->numOfDelegator << endl;
					cout << "***********************************************************************" << endl;
					cout << endl;
				}
			}
		}
	}
	void ElectionParty::displaySimple(Region& region) {
		auto curr = partyVotes.begin();
		PartyVote** arr;
		Party* party;
		int size = 0, i = 0;
		while (curr != nullptr) {
			size++;
			curr++;
		}
		arr = new PartyVote * [size];
		updateElectionPartyNodes(region);
		curr = partyVotes.begin();
		while (curr != nullptr) {
			arr[i] = *curr;
			i++;
			curr++;
		}
		qsort(arr, size, sizeof(PartyVote*), ComparePartyVotes);
		if (curr != nullptr) {
			for (i = 0; i < size; i++) {
				cout << "***********************************************************************" << endl;
				party = arr[i]->party;
				cout << "Name of candidate:  " << party->getCandidate()->getcivilName() << endl;
				cout << "The current Party " << (*curr)->party->getName() << " has " << (*curr)->counter << " votes and " << (*curr)->votePercentage << " percentage" << endl;
				cout << "Num of delegators: " << arr[i]->numOfDelegator << endl;
				cout << "***********************************************************************" << endl;
				printDelegators(arr[i]->party, arr[i]->numOfDelegator, 1);
				cout << endl;
			}
		}
	}
	void ElectionParty::printDelegators(Party* party, int numDelegators, int regionNum) {
		party->getRegList().displayDelegates(numDelegators, regionNum);
	}

	void ElectionParty::displayPartyContent(PartyVote** arr, int size, Region& region) {
		for (int i = 0; i < size; i++) {
			cout << "The current Party " << arr[i]->party->getName() << " has " << arr[i]->counter << " votes and " << arr[i]->votePercentage << " percentage" << endl;
			cout << "Num of delegators: " << arr[i]->numOfDelegator << endl;			
			cout << endl;
			cout << "***********************************************************************" << endl;
			printDelegators(arr[i]->party, region.getNumOfDelegators(), region.getSerialNumber());
		}
	}

	int ElectionParty::CompareDelegators(const void* deleg1, const void* deleg2) {
		PartyVote* tmpdeleg1 = *(PartyVote**)deleg1;
		PartyVote* tmpdeleg2 = *(PartyVote**)deleg2;
		if ((tmpdeleg1->numOfDelegator) > (tmpdeleg2->numOfDelegator)) {
			return -1;

		}
		else if ((tmpdeleg1->numOfDelegator) < (tmpdeleg2->numOfDelegator)) {
			return 1;

		}
		else if ((tmpdeleg1->party->getSerialNumber()) < (tmpdeleg2->party->getSerialNumber())) {

			return 0;
		}
		return 0;
	}
	void ElectionParty::updateElectionPartyNodes(Region& region) {
		auto curr = partyVotes.begin();
		double votesPerParty;
		int counter = 0, size;
		double* voteArr;
		double* numDelegatorsArr;
		while (curr != nullptr) {
			counter++;
			curr++;
		}
		curr = partyVotes.begin();
		size = counter;
		voteArr = new double[size];
		counter = 0;
		while (curr != nullptr) {
			votesPerParty = (*curr)->counter;
			voteArr[counter] = votesPerParty;
			counter++;
			curr++;
		}
		numDelegatorsArr = updateDelegatorsPerParty(region.getNumOfDelegators(), region.getvoteNum(), voteArr, size);
		updateNumOfDelegators(numDelegatorsArr, size);

		delete numDelegatorsArr;
	}

	void ElectionParty::updateNumOfDelegators(double* arr, int size) {
		auto curr = partyVotes.begin();
		int i = 0;
		while (curr != nullptr) {
			(*curr)->numOfDelegator = (int)arr[i];
			curr++;
			i++;
		}
	}

	int ElectionParty::ComparePartyVotes(const void* vote1, const void* vote2)
	{
		PartyVote* tempVote1 = *(PartyVote**)vote1;
		PartyVote* tempVote2 = *(PartyVote**)vote2;
		if ((tempVote1->counter) > (tempVote2->counter)) {
			return -1;

		}
		else if ((tempVote1->counter) < (tempVote2->counter)) {
			return 1;

		}
		else if ((tempVote1->party->getSerialNumber()) < (tempVote2->party->getSerialNumber())) {

			return 0;
		}
		return 0;
	}

	void ElectionParty::saveToFile(ofstream& outElecFile)
	{
		try {
			auto curr = partyVotes.begin();
			int partyId;
			outElecFile.write(rcastcc(&regionVotes), sizeof(double));
			outElecFile.write(rcastcc(partyVotes.size()), sizeof(int));
			outElecFile.write(rcastcc(&serialNumber), sizeof(int));

			for (int i = 0; i < partyVotes.size(); i++)
			{
				partyId = (*curr)->party->getSerialNumber();
				outElecFile.write(rcastcc(&(partyId)), sizeof(int));
				outElecFile.write(rcastcc(&((*curr)->counter)), sizeof(int));
				outElecFile.write(rcastcc(&((*curr)->votePercentage)), sizeof(float));
				outElecFile.write(rcastcc(&((*curr)->numOfDelegator)), sizeof(int));
				curr++;
			}
			if (outElecFile.good() == false) {
				throw ("*** error writing ElectionParty ***");
			}
		}
		catch (const char* ex) {
			throw ex;
		}
	}

	ElectionParty::ElectionParty(ElectionParty& other) {
		this->regionVotes = other.regionVotes;		
		this->serialNumber = other.serialNumber;
		this->partyVotes = other.partyVotes;		
	}

	ElectionParty::ElectionParty(ElectionParty& other, bool isFromFile) {
		this->regionVotes = other.regionVotes;
		this->serialNumber = other.serialNumber;
		this->partyVotes = other.partyVotes;
	}

	ElectionParty::ElectionParty(ifstream& in)
	{
		in.read(rcastc(&regionVotes), sizeof(float));
		in.read(rcastc(&serialNumber), sizeof(int));
	}

	void ElectionParty::addPartyVoteToTail(PartyVote* node) {
		partyVotes.push_back(node);
		if (isLoaded)
			isLoaded = false;		
	}

	void ElectionParty::setNodeVoteContent(int j, Party* party, int numVotesPerParty, double  percent, int  numofDelegators) {
		PartyVote* curr = *partyVotes.findByIndex(j);
		curr = new PartyVote{ party,numVotesPerParty ,percent ,numofDelegators};
		addPartyVoteToTail(curr);
	}
	
}