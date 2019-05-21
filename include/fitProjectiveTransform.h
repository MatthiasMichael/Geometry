#pragma once

#include <vector>

#include "GeometricTypes.h"
#include "GeometryUtils.h"
#include "ImageCoordinateSystem.h"

template<typename ScalarType>
typename GeometricTypes<ScalarType, 2>::Transform::Projective
fitProjectiveTransform(const std::vector<ImagePoint> & srcPoints, const std::vector<ImagePoint> & targetPoints)
{
	using ProjectiveTransform = typename GeometricTypes<ScalarType, 2>::Transform::Projective;

	using Matrix = typename GeometricTypes<ScalarType, 3>::Matrix;
	using Vector = typename GeometricTypes<ScalarType, 3>::Vector;

	assert(srcPoints.size() == 4);
	assert(targetPoints.size() == 4);

	Matrix lhs_src;
	lhs_src << 
		static_cast<ScalarType>(srcPoints[0](0)), static_cast<ScalarType>(srcPoints[1](0)), static_cast<ScalarType>(srcPoints[2](0)),
		static_cast<ScalarType>(srcPoints[0](1)), static_cast<ScalarType>(srcPoints[1](1)), static_cast<ScalarType>(srcPoints[2](1)),
		1, 1, 1;

	Vector rhs_src;
	rhs_src << static_cast<ScalarType>(srcPoints[3](0)), static_cast<ScalarType>(srcPoints[3](1)), 1;
			   
	const Vector solution_src = lhs_src.colPivHouseholderQr().solve(rhs_src);

	lhs_src.col(0) *= solution_src(0);
	lhs_src.col(1) *= solution_src(1);
	lhs_src.col(2) *= solution_src(2);

	Matrix lhs_dst;
	lhs_dst <<
		static_cast<ScalarType>(targetPoints[0](0)), static_cast<ScalarType>(targetPoints[1](0)), static_cast<ScalarType>(targetPoints[2](0)),
		static_cast<ScalarType>(targetPoints[0](1)), static_cast<ScalarType>(targetPoints[1](1)), static_cast<ScalarType>(targetPoints[2](1)),
		1, 1, 1;

	Vector rhs_dst;
	rhs_dst << static_cast<ScalarType>(targetPoints[3](0)), static_cast<ScalarType>(targetPoints[3](1)), 1;

	const Vector solution_dst = lhs_dst.colPivHouseholderQr().solve(rhs_dst);

	lhs_dst.col(0) *= solution_dst(0);
	lhs_dst.col(1) *= solution_dst(1);
	lhs_dst.col(2) *= solution_dst(2);

	return ProjectiveTransform(lhs_dst * lhs_src.inverse());
}


template<typename ScalarType>
typename GeometricTypes<ScalarType, 2>::Transform::Projective
fitProjectiveTransform2(const std::vector<ImagePoint> & s, const std::vector<ImagePoint> & t)
{
	using ProjectiveTransform = typename GeometricTypes<ScalarType,2>::Transform::Projective;
	
	using HomogeneousMatrix = typename GeometricTypes<ScalarType, 2>::HomogeneousMatrix;

	assert(s.size() == 4);
	assert(t.size() == 4);

	Eigen::Matrix<ScalarType, 8, 8> A;
	A <<
		s[0](0), s[0](1), 1, 0, 0, 0, -s[0](0) * t[0](0), -s[0](1) * t[0](0),
		s[1](0), s[1](1), 1, 0, 0, 0, -s[1](0) * t[1](0), -s[1](1) * t[1](0),
		s[2](0), s[2](1), 1, 0, 0, 0, -s[2](0) * t[2](0), -s[2](1) * t[2](0),
		s[3](0), s[3](1), 1, 0, 0, 0, -s[3](0) * t[3](0), -s[3](1) * t[3](0),
		0, 0, 0, s[0](0), s[0](1), 1, -s[0](0) * t[0](1), -s[0](1) * t[0](1),
		0, 0, 0, s[1](0), s[1](1), 1, -s[1](0) * t[1](1), -s[1](1) * t[1](1),
		0, 0, 0, s[2](0), s[2](1), 1, -s[2](0) * t[2](1), -s[2](1) * t[2](1),
		0, 0, 0, s[3](0), s[3](1), 1, -s[3](0) * t[3](1), -s[3](1) * t[3](1);

	Eigen::Matrix<ScalarType, 8, 1> b;
	b << t[0](0), t[1](0), t[2](0), t[3](0), t[0](1), t[1](1), t[2](1), t[3](1);

	auto X = A.colPivHouseholderQr().solve(b);

	HomogeneousMatrix S;
	S << X(0), X(1), X(2), X(3), X(4), X(5), X(6), X(7), 1;

	return ProjectiveTransform(S);
}