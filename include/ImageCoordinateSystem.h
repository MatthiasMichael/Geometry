#pragma once

#include "CoordinateSystem.h"

struct ImageCoordinateSystemTag { };

// Image Points are integer coordinate values corresponding to actual pixels
using ImagePointCoordinateSystem = CoordinateSystem<2, int, ImageCoordinateSystemTag>;

using ImagePoint = ImagePointCoordinateSystem::NamedVector;

// Image Vectors are real valued coordinates which can be useful for intermediate calculations
template<typename ScalarType>
using ImageVectorCoordinateSystem_T = CoordinateSystem<2, ScalarType, ImageCoordinateSystemTag>;

template<typename ScalarType>
using ImageVector_T = typename ImageVectorCoordinateSystem_T<ScalarType>::NamedVector;

template<typename ScalarType>
ImagePoint round(const ImageVector_T<ScalarType> & p)
{
	return make_named<ImagePoint>(
		static_cast<int>(std::round(p(0))),
		static_cast<int>(std::round(p(1)))
		);
}

template<typename ScalarType>
ImagePoint ceil(const ImageVector_T<ScalarType> & p)
{
	return make_named<ImagePoint>(
		static_cast<int>(std::ceil(p(0))),
		static_cast<int>(std::ceil(p(1)))
		);
}

template<typename ScalarType>
ImagePoint floor(const ImageVector_T<ScalarType> & p)
{
	return make_named<ImagePoint>(
		static_cast<int>(std::floor(p(0))),
		static_cast<int>(std::floor(p(1)))
		);
}

template<typename ScalarType>
ImagePoint cast(const ImageVector_T<ScalarType> & p)
{
	return make_named<ImagePoint>(
		static_cast<int>(p(0)),
		static_cast<int>(p(1))
		);
}