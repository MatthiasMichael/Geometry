#pragma once

#include "GeometryUtils.h"

template<typename ScalarType, int Dim>
struct Line
{
	using Point = typename GeometricTypes<ScalarType, Dim>::Vector;
	using Direction = typename NamedVectorTypes<ScalarType, Dim>::Direction;

	Point p;
	Direction dir;
};