#pragma once
#pragma once
#include"election.h"
#include <fstream>

namespace Voting {
	class SimpleElection :public Election {
	private:
	public:
		SimpleElection(int d, int m, int y);
		SimpleElection(ifstream& elecFile, CivilianList* civilianList, bool flagElection);
		virtual ~SimpleElection();
		virtual void display() override;
		virtual void dispElectionType(ostream& os)const;
	};

}