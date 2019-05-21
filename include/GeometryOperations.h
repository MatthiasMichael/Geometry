#pragma once

#include "GeometryUtils.h"
#include "ImageCoordinateSystem.h"

#include "Line.h"
#include "Plane.h"


template <typename ScalarType>
ScalarType cross(const ImageVector_T<ScalarType> & O, const ImageVector_T<ScalarType> & A,
                 const ImageVector_T<ScalarType> & B)
{
	return (A(0) - O(0)) * (B(1) - O(1)) - (A(1) - O(1)) * (B(0) - O(0));
}


template <typename ScalarType, int Dim>
ScalarType getAngleCos(const typename GeometricTypes<ScalarType, Dim>::Vector & a,
                       const typename GeometricTypes<ScalarType, Dim>::Vector & b)
{
	return a.dot(b) / (a.norm() * b.norm());
}


template <typename ScalarType, int Dim>
ScalarType getAngle(const typename GeometricTypes<ScalarType, Dim>::Vector & a,
                    const typename GeometricTypes<ScalarType, Dim>::Vector & b)
{
	return acos(getAngleCos(a, b));
}


template <typename ScalarType, int Dim>
typename GeometricTypes<ScalarType, Dim>::Vector
projectPointToLine(const typename GeometricTypes<ScalarType, Dim>::Vector & point, const Line<ScalarType, Dim> & line)
{
	return line.p + (point - line.p).dot(line.dir.get()) * line.dir.get();
}


template <typename ScalarType, int Dim>
typename GeometricTypes<ScalarType, Dim>::Vector projectPointToPlane(
	const typename GeometricTypes<ScalarType, Dim>::Vector & point, const Plane<ScalarType, Dim> & plane)
{
	const auto norm = plane.getNormal().get();

	return point - (point.dot(norm) - plane.getDistanceFromOrigin().get()) * norm;
}


template <typename ScalarType, int Dim>
typename GeometricTypes<ScalarType, Dim>::Vector projectVectorToVector(
	const typename GeometricTypes<ScalarType, Dim>::Vector & vectorToProject,
	const typename GeometricTypes<ScalarType, Dim>::Vector & vectorToProjectOnto
)
{
	return vectorToProject.dot(vectorToProjectOnto) / vectorToProjectOnto.norm() * vectorToProjectOnto;
}


template <typename ScalarType, int Dim>
ScalarType distancePointToLine(const typename GeometricTypes<ScalarType, Dim>::Vector & point,
                               const Line<ScalarType, Dim> & line)
{
	return (projectPointToLine(point, line) - point).norm;
}


template <typename ScalarType, int Dim>
ScalarType distancePointToPlane(const typename GeometricTypes<ScalarType, Dim>::Vector & point,
                                const Plane<ScalarType, Dim> & plane)
{
	return abs(point.dot(plane.getNormal().get()) - plane.getDistanceFromOrigin().get());
}


template <typename ScalarType, int Dim>
typename Line<ScalarType, Dim>::Point intersectLineAndLine(const Line<ScalarType, Dim> & l1,
                                                           const Line<ScalarType, Dim> & l2)
{
	using Vector = typename Line<ScalarType, Dim>::Direction::UnderlyingType;

	const Vector v1 = l1.dir.get() / l1.dir.get().norm();
	const Vector v2 = l2.dir.get() / l2.dir.get().norm();

	const Vector d = l2.p - l1.p;

	const ScalarType a = v1.dot(v2);
	const ScalarType b = d.dot(v1);
	const ScalarType c = (d - b * v1).dot(v2);

	const ScalarType x2 = 1 / (a * a - 1) * c; // Possible Out 1

	const ScalarType x1 = (d + x2 * v2).dot(v1); // Possibe Out 2

	return Line<ScalarType, Dim>::Point(0.5 * (l1.p + x1 * v1 + l2.p + x2 * v2));
}


template <typename ScalarType, int Dim>
std::tuple<typename GeometricTypes<ScalarType, Dim>::Vector, ScalarType>
intersectLineAndPlane(const Line<ScalarType, Dim> & line, const Plane<ScalarType, Dim> & plane)
{
	const auto norm = plane.getNormal().get();
	const ScalarType scale = plane.getDistanceFromOrigin().get() - norm.dot(line.p) / norm.dot(line.dir.get());

	return std::make_tuple(line.p + scale * line.dir.get(), scale);
}
