#pragma once

#include "GeometricTypes.h"
#include "GeometryUtils.h"

#include "ImageCoordinateSystem.h"

class ScanLine
{
public:
	ScanLine();
	ScanLine(const ImagePoint & start, const ImagePoint & end);

	int add(const ImagePoint & p);
	int finalize();

	const ImagePoint & getStart() const { return start; }
	const ImagePoint & getEnd() const { return end; }

	const int getY() const { return start(1); }
	const int getStartX() const { return start(0); }
	const int getEndX() const { return end(0); }

private:
	static const int s_invalidValue;
	static const ImagePoint s_invalidPoint;

	ImagePoint start;
	ImagePoint end;
};