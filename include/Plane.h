#pragma once

#include "GeometryUtils.h"

template<typename ScalarType, int Dim>
struct Plane
{
	using Point = typename GeometricTypes<ScalarType, Dim>::Vector;
	using Direction = typename NamedVectorTypes<ScalarType, Dim>::Direction;

	using Length = typename NamedScalarTypes<ScalarType>::Length;

	Length getDistanceFromOrigin() const;
	Direction getNormal() const;

	Point p;
	Direction dir1;
	Direction dir2;
};


template <typename ScalarType, int Dim>
typename Plane<ScalarType, Dim>::Length Plane<ScalarType, Dim>::getDistanceFromOrigin() const
{
	return make_named<Length>(abs(p.transpose() * getNormal().get()));
}


template <typename ScalarType, int Dim>
typename Plane<ScalarType, Dim>::Direction Plane<ScalarType, Dim>::getNormal() const
{
	return make_named<Direction>(dir1.get().cross(dir2.get()).normalized());
}
