#include "dividedRegion.h"
namespace Voting {
	DividedRegion::DividedRegion(const string& regionName, int size) :Region(regionName, size) {
	}



	DividedRegion::DividedRegion(ifstream& in, CivilianList* civList, int* sizeArr, int** arr) : Region(in, civList, sizeArr, &arr)
	{
	}

	DividedRegion::~DividedRegion()
	{
	}
	
	void DividedRegion::dispType(ostream& os) const {
		os << " Region Type: Divided Region" << endl;
	}
}