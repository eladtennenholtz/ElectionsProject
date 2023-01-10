#pragma once
#include "region.h"
namespace Voting {
	class UnifiedRegion :public Region {
	private:

	public:
		UnifiedRegion(const string& regionName, int size);
		UnifiedRegion(ifstream& in, CivilianList* civList, int* sizeArr, int** arr);		
		virtual void dispType(ostream& os)const override;
	};

}