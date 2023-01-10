#include <fstream>
#include "Utils.h"
#include "simpleElection.h"
#include "civilianList.h"
#include "dividedRegion.h"
#include "unifiedRegion.h"
using namespace std;
using namespace Voting;
static const int maxLen = 30;

void handleMenu(Election& election, CivilianList &civilianList, bool electionType, bool afterLoad = false) {	
	string fileName = new char(maxLen);
	string civilianName = new char(maxLen);
	string regionName = new char(maxLen);
	string partyName = new char(maxLen);
	int requestNum;
	int id;
	int birthYear;
	int regionNum;
	int party;
	int length;
	bool regionType;
	Region* region;
	Region* constRegion = nullptr;
	Election* currElection = nullptr;
	bool isAfterLoad = afterLoad;

	if (electionType == 0) {//simple
		constRegion = new DividedRegion("const", 10);
		election.addRegionToList(constRegion);
		election.addRegionToPartyList();
		election.addNodeToVoteList();		
	}

	while (1) {
		if (currElection != nullptr) {
			election = *currElection;
		}
		cout << "1.Add region(name,number of delegators, region type-0/1:divided/unified)" << endl;
		cout << "2.Add civilian(name,id,year of birth,region number)" << endl;
		cout << "3.Add party(name,candidate id)" << endl;
		cout << "4.Add delegator(id,region,party)" << endl;
		cout << "5.Display regions" << endl;
		cout << "6.Display civilians" << endl;
		cout << "7.Display parties" << endl;
		cout << "8.Civilian Vote (party num,civilian id)" << endl;
		cout << "9.Display Election results" << endl;
		cout << "10.Exit " << endl;
		cout << "11.saveToFile File (file name) " << endl;
		cout << "12.loadFromFile File (file name)  " << endl;

	
		cin >> requestNum;		
		if (requestNum <1 || requestNum>12)
		{
			cout << "Your request number is not the list" << endl;
			continue;
		}
		else {
			switch (requestNum) {
				case 1: {
					if (electionType == 1) //regular
					{
						cin >> regionName >> length >> regionType;						
						if (regionType == 1) {
							region = new UnifiedRegion(regionName, length);
							election.addRegionToList(region);
							election.addRegionToPartyList();
							election.addNodeToVoteList();
						}
						else {//simple
							region = new DividedRegion(regionName, length);
							election.addRegionToList(region);
							election.addRegionToPartyList();
							election.addNodeToVoteList();
						}				

					}
					else {
						cout << "For Simple Election there is no region to add" << endl;						
					}					
				}
						break;
				case 2: {
					cin >> civilianName;
					cin >> id; 
					while (id < 100000000 || id>999999999)
					{						
						cout << "valid id is 9 digits! " << endl;
						cin >> id;
					}
					cin >> birthYear;
					while (election.getDate().year - birthYear < 18)
					{						
						cout << "sorry you can not vote - you are unter 18 years old! ";
						cin >> birthYear;
					}
					cin >> regionNum;
					if (electionType == 0) {
						Civilian civilian(civilianName, id, birthYear, constRegion);
						civilianList.addNodeToList(civilian);
						election.addCivilianToRegion(civilianList);
						if (isAfterLoad)
							election.getRegionList().getHead()->data->region->addCivilianToTail(&civilian);
					}
					else {
						Region* region = election.findRegion(regionNum);
						Civilian civilian(civilianName, id, birthYear, region);
						civilianList.addNodeToList(civilian);
						election.addCivilianToRegion(civilianList);
					}					
				}
						break;
				case 3: {
					cin >> partyName >> id;
					Civilian* civilian = civilianList.checkCivilian(id);
					int numofRegions;
					numofRegions = election.getNumOfRegions();
					if (civilian != nullptr) {
						Party party(partyName, id, civilian, numofRegions);
						election.addPartyToPartyList(party);
						election.addPartyPerRegion();
					}
					else {
						cout << "Party id is not in the list" << endl;
					}
				}
						break;
				case 4: {
					cin >> id >> regionNum >> party;
					Civilian* civilian = nullptr;
					try {
						if (electionType == 0) {

							if (civilianList.updateCivilian(id, &civilian)) {
								election.updateParty(civilian, party);
							}
							else {
								throw "civilian is not in the list !";
							}
						}
						else {
							if (civilianList.updateCivilian(id, &civilian)) {
								election.updateParty(civilian, party, regionNum);
							}
							else {
								throw "civilian is not in the list !";
							}

						}
					}
					catch (const char* ex)
					{
						cout << ex << endl;
					}
					
				}
						break;
				case 5: {
					if (electionType == 0) {
						cout << "For Simple Election there is no region to display" << endl;
					}
					else {
						election.displayRegions();
					}
				}
						break;
				case 6: {
					civilianList.displayCivilians();
				}
						break;
				case 7: {
					election.displayParties();
				}
						break;
				case 8: {
					cin >> party >> id;
					try {
						Civilian* civilian = civilianList.vote(party, id);
						if (civilian == nullptr) {
							throw "civilian is not in the list !";
						}
						else
							if (civilian->getIsVote() == true) {
								throw "civilian already voted !" ;
							}
							else {
								civilian->setIsVote(true);
								election.updateVotes(civilian, party);
							}
					}
					catch (const char* ex) {
						cout << ex << endl;
					}
				}
						break;
				case 9: {					
					cout << election;
					election.displayResults();					
				}
						break;
				case 10: {
				}
						 break;
				case 11: {
					cin >> fileName;
					
					try {
						ofstream electionfile(fileName, ios::binary | ios::trunc);
						if (!electionfile) { throw"can not create a file!"; }
						if (electionfile.good() == false) {
							throw "*************Could not write the election file ******************";
							//exit(-1);
						}

						electionfile.write(rcastcc(&electionType), sizeof(bool));

						if (electionType == 0) { //simple	
							ElectionPartyList& elecPartyList = election.getVoteList();
							if (elecPartyList.getVoteList().empty() == false) {
								election.addElectionPartyToTail(elecPartyList.getVoteList().getHead()->data->vote);
							}
						}
						civilianList.saveToFile(electionfile);
						election.saveToFile(electionfile, electionType);
						electionfile.close();
						continue;
					}
					catch (const char* ex) {
						cout << ex << endl;
					}
					catch (...) {
						cout << "error writing to file" << endl;
					}
				}
					   break;
				case 12: {
					cin >> fileName;
					
					try {
						ifstream electionfile(fileName, ios::binary);
						if (!electionfile) { throw"can not read a file!"; }
						if (electionfile.good() == true) {

							electionfile.read(rcastc(&electionType), sizeof(bool));
							CivilianList* civilianList = new CivilianList(electionfile);

							if (electionType == 1) { //regular election
								currElection = new Election(electionfile, civilianList, electionType);
							}
							else {
								currElection = new SimpleElection(electionfile, civilianList, electionType);
								
							}
							isAfterLoad = true;
							electionfile.close();
						}
						else {
							throw "*************Could not read the election file ******************";
							//exit(-1);
						}
					}
					catch (const char* ex) {
						cout << ex << endl;
					}
				}
					   break;
			}
			if (requestNum == 10) {
				break;
			}
			else
			{
				continue;
			}
		}
		
	}
	
}
bool checkDate(int day, int month, int year)
{
	if (year <= 0)
		return false;
	if (month < 1 || month >12)
		return false;
	if (month == 2)
	{
		return (day >= 1 && day <= 28);
	}
	else if (month == 4 || month == 6 ||  month == 9 || month == 11)
	{
		return (day >= 1 && day <= 30);
	}
	else
	{
		return (day >= 1 && day <= 31);
	}
}

void mainMenu() {
	CivilianList civilianList;
	Election* election;
	int day, month, year;
	int requestNum;
	bool electionType;
	
	string fileName;
	bool isAfterLoad = false;

	while (1) {
		cout << "1. enter Election Date: Day,Month,Year and type of election: for simple type 0 /for regular type 1 " << endl;		
		cout << "2. enter the name for election file: " << endl;
		cout << "3.exit" << endl;

		cin >> requestNum;
		switch (requestNum) {
			case 1: {
				cin >> day >> month >> year;
				while (checkDate(day, month, year) == false) {
					cout << "date is not legal ! " << endl;
					cin >> day >> month >> year;
				}
				cin >> electionType;
				if (electionType == 1)
					election = new Election(day, month, year);
				else
					election = new SimpleElection(day, month, year);

				handleMenu(*election, civilianList, electionType);
			}
				  break;
			case 2: {
				cin >> fileName;
				
				ifstream elecFile(fileName, ios::binary);
				if (!elecFile.good()) {
					cout << "can not read from file" << endl;
					exit(-1);
				}
				elecFile.read(rcastc(&electionType), sizeof(bool));
				CivilianList* civilianList = new CivilianList(elecFile);						

				if (electionType == 1) {
					election = new Election(elecFile, civilianList, electionType);
				}
				else {
					election = new SimpleElection(elecFile, civilianList, electionType);
				}
				
				isAfterLoad = true;


				handleMenu(*election, *civilianList, electionType, isAfterLoad);
				elecFile.close();

			}
				  break;
			case 3: {
				cout << "******THE END OF ELECTION *******" << endl;
			}
				  break;
			default:
				break;
		}
		if (requestNum == 3) {
			break;
		}
		else
		{
			continue;
		}		
	}

}


int main(){
		
	mainMenu();
}







