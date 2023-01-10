#pragma once
#include "region.h"

namespace Voting {
	class DividedRegion :public Region {
	private:	


	public:
		DividedRegion(const string&, int);
		DividedRegion();
		DividedRegion(ifstream& in, CivilianList* civList, int* sizeArr, int** arr);
		virtual ~DividedRegion();		
		virtual void dispType(ostream& os)const override;
		
	};
}

