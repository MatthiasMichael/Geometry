#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include "named_type.h"

#include "GeometricTypes.h"
#include "GeometrySkills.h"


template<typename ScalarType>
struct NamedScalarTypes
{
private:
	struct DegreesTag {};
	struct RadiansTag {};
	struct LengthTag {};

public:
	using Degrees = NamedType<ScalarType, DegreesTag, ScalarTypeSkills>;
	using Radians = NamedType<ScalarType, RadiansTag, ScalarTypeSkills>;
	using Length = NamedType<ScalarType, LengthTag, ScalarTypeSkills>;
};


template<typename ScalarType, int Dim>
struct NamedVectorTypes
{
private:
	struct DistanceTag {};
	struct DirectionTag {};
	struct SizeTag {};

public:
	using Distance = NamedType<typename GeometricTypes<ScalarType, Dim>::Vector, DistanceTag, VectorTypeSkills>;
	using Direction = NamedType<typename GeometricTypes<ScalarType, Dim>::Vector, DirectionTag, VectorTypeSkills>;
	using Size = NamedType<typename GeometricTypes<ScalarType, Dim>::Vector, DirectionTag, VectorTypeSkills>;
};


template<typename ScalarType>
typename NamedScalarTypes<ScalarType>::Radians deg2rad(typename NamedScalarTypes<ScalarType>::Degrees degrees)
{
	return NamedScalarTypes<ScalarType>::Radians((static_cast<ScalarType>(M_PI) / 180.f) * degrees.get());
}


template<typename ScalarType>
typename NamedScalarTypes<ScalarType>::Degrees rad2deg(typename NamedScalarTypes<ScalarType>::Radians radians)
{
	return NamedScalarTypes<ScalarType>::Degrees((180.f / static_cast<ScalarType>(M_PI)) * radians.get());
}