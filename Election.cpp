#include "election.h"

namespace Voting {
	
	Election::Election(int _d, int _m, int _y): date{ _d,_m,_y },voteList(){
	}
	Election::~Election() {
		free();
	}	
	
	void Election::free() {		
		auto curr = regionList.begin();
		while (curr != nullptr)
		{
			delete* curr;
			curr++;
		}
		auto curr1 = partyList.begin();
		while (curr1 != nullptr)
		{
			delete* curr1;
			curr1++;
		}
		auto curr2 = resPartyList.begin();
		while (curr2 != nullptr)
		{
			delete* curr2;
			curr2++;
		}
		auto curr3 = electionPartyList.begin();
		while (curr3 != nullptr)
		{
			delete* curr3;
			curr3++;
		}		
	}

	Election::Election(ifstream& elecFile, CivilianList* civList, bool electionType) {
		try {
			elecFile.read(rcastc(&date), sizeof(date));
			loadFromFileRegionList(elecFile, civList);
			loadFromFilePartyList(elecFile, electionType, civList);
			loadFromFileLinkListVoteSys(elecFile, electionType);
			loadFromFileElectionPartyList(elecFile, electionType);
			updateRegionsInCivilians(civList);
		}
		catch (const char* ex)
		{
			free();			
			throw ex;
		}
		catch (bad_alloc & ex)
		{
			free();			
			throw ex;
		}

	}
	void Election::operator=(const Election& other) {
		this->date = other.date;
		this->regionList = other.regionList;
		this->partyList = other.partyList;
		this->resPartyList = other.resPartyList;
		this->electionPartyList = other.electionPartyList;		
		this->voteList = other.voteList;
	}

	void Election::updateParty(Civilian*civilian, int partyNum, int regionNum){
		NodePartyList* curr = *partyList.findByIndex(partyNum);
		
		if (curr == nullptr)
		{
			throw "***party doesnt exist***";
		}

		curr->party->getRegList().addDelegate(civilian, regionNum);
	}

	void Election::addRegionToPartyList(){
		auto curr = partyList.begin();		
		while (curr!=nullptr) {
			(*curr)->party->getRegList().addNode(1);  
			curr++;
		}	
	}

	int Election::getNumOfRegions() const{
		return regionList.size();		
	}

	Region * Election::findRegion(int regionNumber) const{
		NodeRegionList* curr = *regionList.findByIndex(regionNumber);
		return curr->region;
	}

	void Election::addCivilianToRegion(CivilianList& civlionList) {
		Civilian*civilian = civlionList.getCivilian();
		if (civilian != nullptr) {
			Region* region = civilian->getRegion();
			region->addCivilianToTail(civilian);			
		}
	}

	void Election::addNodeToVoteList(){
		Region* region = regionList.back()->region;
		voteList.addNode(region);
	}

	void Election::addPartyPerRegion(){
		Party* party = partyList.back()->party;
		voteList.addNodeParty(party);
	}

	void Election::updateVotes(Civilian * civilian, int partyNum){		
		updatePartyVotesLinkedList(partyNum); 
		voteList.updateVotes(civilian->getRegion()->getSerialNumber(), partyNum);
	}

	void Election::displayResults(){
		voteList.countVotes();
		updatePercentageInRegions();
		updatePercentageInParty();
		display();
	}

	void Election::updatePercentageInRegions(){
		auto curr = regionList.begin();
		double num;
		double num2;
		double result;
		while (curr != nullptr)
		{
			 num = (*curr)->region->getvoteNum();
			 num2 = (*curr)->region->getCounterPeople();
			 result = (num / num2) * 100;
			 (*curr)->region->setPrecentOfVoters(result);
			 curr++;
		}
	}

	void Election::updatePercentageInParty(){
		voteList.updatePercentageInParty();	
	}

	void Election::display(){		
		voteList.displayRegularElection();
		updatePartyDelegatorsLinkList();
		buildResultList();
		printResultPartyList();
		updateNumDelegatorsResultPartyList();
	}

	ostream& operator<<(ostream& os, const Election& election) {
		election.dispElectionType(os);
		os << "Election date: " << "Day:" << election.date.day << " " << "Month: " << election.date.month << " " << "Year: " <<election.date.year << endl;
		return os;
	}

	void Election::printResultPartyList() const {
		auto curr = resPartyList.begin();
		
		cout << "------------------------------------------------------------------------------------------------" << endl;
		cout << "The results are: " << endl;
		while (curr != nullptr) {
			cout << "***********************************"<< endl;
			cout << "Party: " << (*curr)->party->getName() << " " << endl;
			cout << "Name of candidate: " << (*curr)->party->getCandidate()->getcivilName()<<endl<< "Num of delegators : " << (*curr)->numOfDelegators<<endl << "Num of votes : " << (*curr)->numOfVotes << endl;
			curr++;
		}
		curr = resPartyList.begin();
		if (curr != nullptr) {

			cout << "------------------------------------------------------------------------------------------------" << endl;
			cout << "The winner: " << endl;
			cout << "Party: " << (*curr)->party->getName() << " " << endl;
			cout << "Name of candidate: " << (*curr)->party->getCandidate()->getcivilName() << endl << "Num of delegators : " << (*curr)->numOfDelegators << endl << endl <<endl;
		}	
	}

	void Election::buildResultList(){
		
		auto currPartyList = partyList.begin();
		
		while (currPartyList != nullptr) {
			auto curr = partyList.begin();
			auto curr_ = partyList.begin();
			int max = 0, counter = 0, maxIndex = 1;
			
			while (curr != nullptr) {
				counter++;
				if ((*curr)->numOfDelegators > max) {
					max = (*curr)->numOfDelegators;
					maxIndex = counter;
					curr_ = curr;
				}
				else if ((*curr)->numOfDelegators == max) {
					if (((*curr)->party->getSerialNumber()) < ((*curr_)->party->getSerialNumber())) {
						max = (*curr)->numOfDelegators;
						maxIndex = counter;
						curr_ = curr;
					}
				}
				curr++;
			}
			
			addNodeToListResList((*curr_));			
		}
		
	}

	void Election::updatePartyVotesLinkedList(int partyNum){
		auto curr = partyList.begin();
		while (curr != nullptr) {
			
			if ((*curr)->party->getSerialNumber() == partyNum) {
				(*curr)->numOfVotes++;
				break;
			}
			if (curr != nullptr)
				curr++;
		}
	}

	void Election::updatePartyDelegatorsLinkList() {
		int partySerialNumber;
		int numDelegates;
		Result* res;
		ElectionParty* elecParty;
		res = voteList.getResult();	
		auto curr = res->getResList().begin();
		
		while (curr != nullptr) {
			numDelegates = (*curr)->numOfDelegates;
			partySerialNumber = (*curr)->partySerialNum;
			elecParty = (*curr)->elecParty;
			addElectionPartyToTail(*elecParty);
			if (numDelegates == 0) {
				updateDividedResult(*elecParty);
			}
			else {
				updateResult(numDelegates, partySerialNumber);
			}
			curr++;
		}	
		
	}
	void Election::updateResult(int numDelegates, int partySerialNumber)
	{
		NodePartyList* curr = *partyList.findByIndex(partySerialNumber);		
		curr->numOfDelegators = curr->numOfDelegators + numDelegates;
	}

	void Election::addNodeToListResList(NodePartyList * newnode){
		NodeResPartyList* node = new NodeResPartyList;
		node->numOfDelegators = newnode->numOfDelegators;
		node->numOfVotes = newnode->numOfVotes;
		node->party = newnode->party;		
		delete newnode;
		resPartyList.push_back(node);
	}

	void  Election::addPartyToPartyList(Party & party) {		
		NodePartyList*  node = new NodePartyList;
		node->party = new Party(party);
		node->numOfDelegators = 0;
		node->numOfVotes = 0;		

		partyList.push_back(node);		
	}

	void Election::displayRegions() const {

		auto curr = regionList.begin();
		while (curr != nullptr) {
			cout << *((*curr)->region);
			curr++;
		}
	}

	void Election::displayParties() const {
		auto curr = partyList.begin();
		while (curr != nullptr) {
			cout << *((*curr)->party);
			curr++;
		}
	}


	void  Election::addRegionToList(Region* region) {
		NodeRegionList* node = new NodeRegionList;
		node->region = region;		
		regionList.push_back(node);		
	}

	void Election::updateDividedResult(ElectionParty& elecParty) {		
		auto curr = elecParty.getPartyVotes().begin();
		while (curr != nullptr) {
			(*curr)->numOfDelegator += elecParty.getPartyVotes().getHead()->data->numOfDelegator;
			curr++;
		}
	}

	void Election::dispElectionType(ostream& os) const {
		os << " Type Of Election: Regular " << endl;
	}


	void Election::saveToFile(ofstream& outElecFile, bool electionType) {
		try {
			outElecFile.write(rcastcc(&date), sizeof(date));
			copyRegionList(outElecFile);
			copyPartyList(outElecFile);
			copyElectionPartyList(outElecFile);
			voteList.saveToFile(outElecFile, electionType);
			if (outElecFile.good() == false) {
				throw ("*** error writing Election ***");
			}
		}
		catch (const char* ex) {
			throw ex;
		}
	}	
	

	void Election::copyPartyList(ofstream& outElecFile) {
		auto curr = partyList.begin();
		outElecFile.write(rcastcc(partyList.size()), sizeof(int));
		for (int i = 0; i < partyList.size(); i++) {
			(*curr)->party->saveToFile(outElecFile);			
			outElecFile.write(rcastcc(&((*curr)->numOfVotes)), sizeof(int));
			outElecFile.write(rcastcc(&((*curr)->numOfDelegators)), sizeof(int));
			curr++;
		}
	}

	void Election::copyRegionList(ofstream& outElecFile) {
		auto curr = regionList.begin();
		bool typeOfRegion;
		Region* reg;
		outElecFile.write(rcastcc(regionList.size()), sizeof(int));
		for (int i = 0; i < regionList.size(); i++) {
			reg = dynamic_cast<UnifiedRegion*>((*curr)->region);
			if (reg) {
				typeOfRegion = true;
			}
			else {
				typeOfRegion = false;
			}
			outElecFile.write(rcastcc(&typeOfRegion), sizeof(bool));
			(*curr)->region->saveToFile(outElecFile);
			curr++;
		}
	}

	void Election::addElectionPartyToTail(ElectionParty& elecParty) {		

		ElectionPartyNode* item = new ElectionPartyNode{ new ElectionParty(elecParty)};
		electionPartyList.push_back(item);		
	}

	void Election::copyElectionPartyList(ofstream& outElecFile) {
		auto curr = electionPartyList.begin();
		for (int i = 0; i < electionPartyList.size(); i++) {
			if (curr != nullptr) {
				(*curr)->elecParty->saveToFile(outElecFile);
				curr++;
			}
		}
	}

	void Election::loadFromFileRegionList(ifstream& in, CivilianList* civilList)
	{		
		NodeRegionList* curr;
		Region* unifiedRegion = nullptr;
		Region* dividedRegion = nullptr;
		bool flag;
		int sizeArr, * arr = nullptr;
		Civilian* civil;
		int sizeRegionList;
		in.read(rcastc(&sizeRegionList), sizeof(sizeRegionList));
		
		for (int i = 0; i < sizeRegionList; i++) {
			in.read(rcastc(&flag), sizeof(bool));
			if (flag) {
				unifiedRegion = new UnifiedRegion(in, civilList, &sizeArr, &arr);
				for (int j = 0; j < sizeArr; j++) {
					civilList->updateCivilian(arr[j], &civil);
					unifiedRegion->addCivilianToTail(civil);
				}
				
				curr = new NodeRegionList{ unifiedRegion };
				addNodeToTailRegionListFromFile(curr);
			}
			else {
				dividedRegion = new DividedRegion(in, civilList, &sizeArr, &arr);
				for (int j = 0; j < sizeArr; j++) {
					civilList->updateCivilian(arr[j], &civil);
					dividedRegion->addCivilianToTail(civil);
				}
				curr = new NodeRegionList{ dividedRegion};
				addNodeToTailRegionListFromFile(curr);
			}
		}
	}

	void Election::loadFromFilePartyList(ifstream& in, bool regularElection, CivilianList* civilianList)
	{
		Party* party;
		int numDelegators;
		int numVotes;
		NodeResPartyList* curr;
		int sizePartyList;
		in.read(rcastc(&sizePartyList), sizeof(sizePartyList));
		for (int i = 0; i < sizePartyList; i++) {
			party = new Party(in, civilianList);
			in.read(rcastc(&numVotes), sizeof(int));
			in.read(rcastc(&numDelegators), sizeof(int));
			curr = new NodeResPartyList{ party , numVotes,numDelegators};
			addNodeToTailPartyResListFromFile(curr);
		}
	}

	void Election::addNodeToTailPartyListloadFromFile(NodePartyList* node) {
		NodePartyList* item = new NodePartyList;
		item->numOfDelegators = node->numOfDelegators;
		item->numOfVotes = node->numOfVotes;
		item->party = node->party;		
		delete node;
		partyList.push_back(item);		
	}
	void Election::loadFromFileElectionPartyList(ifstream& elecFile, bool electionType) {
		int regionId;
		int partyId;
		Region* region;
		Party* party;
		ElectionParty* electionParty;		
						
		for (int i = 0; i < regionList.size(); i++) {
			elecFile.read(rcastc(&regionId), sizeof(int));
			elecFile.read(rcastc(&partyId), sizeof(int));
			region = findRegion(regionId);
			electionParty = new ElectionParty(elecFile);
			party = findPartyFromParty(partyId);
			*electionParty = findVoteSys(i);
			voteList.createNodeList(region, *electionParty, party);			
		}

	}

	void Election::loadFromFileLinkListVoteSys(ifstream& elecFile, bool electionType) {
		ElectionPartyNode* curr;
		ElectionParty* electionParty;
		Party* party;
		int numVotesPerParty;
		int partyId;
		int numDelegators;
		int sizeParty;
		double precentVote;
		
		sizeParty = partyList.size();
		for (int i = 0; i < regionList.size(); i++) {
			electionParty = new ElectionParty(elecFile);

			for (int j = 0; j < sizeParty; j++) {
				elecFile.read(rcastc(&partyId), sizeof(int));
				elecFile.read(rcastc(&numVotesPerParty), sizeof(int));
				elecFile.read(rcastc(&precentVote), sizeof(float));
				elecFile.read(rcastc(&numDelegators), sizeof(int));
				party = findPartyFromParty(partyId);
				electionParty->setNodeVoteContent(j, party, numVotesPerParty, precentVote, numDelegators);
			}
			curr = new ElectionPartyNode{electionParty};
			addElectionPartyToTailFromFile(curr);
		}
	}

	Party* Election::findPartyFromResParty(int partyId)
	{
		auto curr = resPartyList.begin();
		while (curr != nullptr) {
			if ((*curr)->party->getSerialNumber() == partyId)
				return (*curr)->party;
			curr++;
		}
		return nullptr;
	}

	Party* Election::findPartyFromParty(int partyId)
	{
		auto curr = partyList.begin();
		while (curr != nullptr) {
			if ((*curr)->party->getSerialNumber() == partyId)
				return (*curr)->party;
			curr++;
		}
		return nullptr;
	}

	ElectionParty& Election::findVoteSys(int index) {
		ElectionPartyNode* curr = *electionPartyList.findByIndex(index);		
		return *(curr->elecParty);
	}

	void Election::updateRegionsInCivilians(CivilianList* civilList) {
		int regionId;
		Region* region;
		while (civilList->getHead()) {
			regionId = civilList->getHead()->civilian->getRegionNum();
			region = findRegion(regionId);
			civilList->getHead()->civilian->setRegionPointer(region);
			
		}
	}

	void Election::addNodeToTailRegionListFromFile(NodeRegionList* node)
	{
		regionList.push_back(node);
	}

	void Election::addNodeToTailPartyListFromFile(NodePartyList* node)
	{
		partyList.push_back(node);
	}

	void Election::addNodeToTailPartyResListFromFile(NodeResPartyList* node)
	{
		resPartyList.push_back(node);
	}

	void Election::addElectionPartyToTailFromFile(ElectionPartyNode* node)
	{
		electionPartyList.push_back(node);
	}

	Election::NodePartyList* Election::alocateNode(NodePartyList* node) {
		NodePartyList* result;
		result = new NodePartyList{ node->party, node->numOfVotes,node->numOfDelegators };
		return result;
	}
	
	void Election::addNodeToTailPartyList(NodeResPartyList* node)
	{		
		NodePartyList* item = new NodePartyList;
		item->numOfDelegators = node->numOfDelegators;
		item->numOfVotes = node->numOfVotes;
		item->party = node->party;
		partyList.push_back(item);
	}

	void Election::updateNumDelegatorsResultPartyList()
	{
		auto curr2 = partyList.begin();
		while (curr2 != nullptr) {
			(*curr2)->numOfDelegators = 0;
			curr2++;
		}
		auto curr = resPartyList.begin();
		while (curr != nullptr) {
			(*curr)->numOfDelegators = 0;
			curr++;
		}

	}	

}