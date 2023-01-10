#include "simpleElection.h"
namespace Voting {

	SimpleElection::SimpleElection(int d, int m, int y) :Election(d, m, y) {

	}

	SimpleElection::SimpleElection(ifstream& elecFile, CivilianList* civilianList, bool flagElection) : Election(elecFile, civilianList, flagElection) {

	}

	SimpleElection::~SimpleElection()
	{
	}

	void SimpleElection::display() {
		voteList.displaySimpleElection();
	}

	void SimpleElection::dispElectionType(ostream& os) const {
		os << "Type of Election: Simple  " << endl;
	}
}