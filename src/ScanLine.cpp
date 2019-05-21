#include "ScanLine.h"


const int ScanLine::s_invalidValue = std::numeric_limits<int>::max();
const ImagePoint ScanLine::s_invalidPoint = make_named<ImagePoint>(s_invalidValue, s_invalidValue);


ScanLine::ScanLine() : ScanLine(s_invalidPoint, s_invalidPoint)
{
	// empty
}


ScanLine::ScanLine(const ImagePoint & start, const ImagePoint & end) :
	start(start),
	end(end)
{
	// empty
}


int ScanLine::add(const ImagePoint & p)
{
	if (start == s_invalidPoint)
	{
		start = p;
		return 0;
	}

	assert(end == s_invalidPoint);

	end = p;

	if (start(0) > end(0))
	{
		std::swap(start, end);
	}

	return end(0) - start(0) + 1;
}


int ScanLine::finalize()
{
	assert(!(start == s_invalidPoint));

	if(end == s_invalidPoint)
	{
		end = start;
		return 1;
	}

	return 0;
}
