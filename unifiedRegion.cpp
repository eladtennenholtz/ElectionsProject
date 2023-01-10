#include "unifiedRegion.h"
namespace Voting {
	UnifiedRegion::UnifiedRegion(const string& regionName, int size) : Region(regionName, size) {

	}

	UnifiedRegion::UnifiedRegion(ifstream& in, CivilianList* civList, int* sizeArr, int** arr) : Region(in, civList, sizeArr, &arr)
	{
	}	
	
	void UnifiedRegion::dispType(ostream& os) const {
		os << " Region Type: Unfied Region" << endl;
	}

}