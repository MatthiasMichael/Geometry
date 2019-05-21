#pragma once

#include "GeometryUtils.h"

#include "CoordinateSystem.h"
#include "CoordinateTransform.h"


template <typename CoordinateSystem>
class Rotation
{
public:
	using ScalarType = typename CoordinateSystem::TScalar;
	using NamedVector = typename CoordinateSystem::NamedVector;
	using NamedTransform = typename CoordinateSystem::NamedAffineTransform;

	using Degrees = typename NamedScalarTypes<ScalarType>::Degrees;
	using Radians = typename NamedScalarTypes<ScalarType>::Radians;

	Rotation();
	Rotation(Degrees yaw, Degrees pitch, Degrees roll);

	template <typename SourceCoordinateSystem>
	explicit Rotation(const Rotation<SourceCoordinateSystem> & sourceRotation);

	NamedVector apply(const NamedVector & v);

	NamedTransform transform;

	Degrees yaw;
	Degrees pitch;
	Degrees roll;
};


template <typename CoordinateSystem>
Rotation<CoordinateSystem>::Rotation() : Rotation(Degrees(0), Degrees(0), Degrees(0))
{
	// empty
}


template <typename CoordinateSystem>
Rotation<CoordinateSystem>::Rotation(Degrees yaw, Degrees pitch, Degrees roll) :
	transform(),
	yaw(yaw),
	pitch(pitch),
	roll(roll)
{
	using Matrix = typename GeometricTypes<ScalarType, 3>::Matrix;

	const Radians angle_rotZ = deg2rad<ScalarType>(yaw);
	const Radians angle_rotY = deg2rad<ScalarType>(pitch);
	const Radians angle_rotX = deg2rad<ScalarType>(roll);

	const float cZ = cos(angle_rotZ.get());
	const float cY = cos(angle_rotY.get());
	const float cX = cos(angle_rotX.get());

	const float sZ = sin(angle_rotZ.get());
	const float sY = sin(angle_rotY.get());
	const float sX = sin(angle_rotX.get());

	Matrix mat_rotZ;
	Matrix mat_rotX;
	Matrix mat_rotY;

	mat_rotZ <<
		cZ , -sZ , 0 ,
		sZ , cZ , 0 ,
		0 , 0 , 1;

	mat_rotY <<
		cY , 0 , -sY ,
		0 , 1 , 0 ,
		sY , 0 , cY;

	mat_rotX <<
		1 , 0 , 0 ,
		0 , cX , sX ,
		0 , -sX , cX;

	Matrix mat_rot = mat_rotZ * mat_rotY * mat_rotX;

	transform = make_named<NamedTransform>(mat_rot);
}


template <typename CoordinateSystem>
template <typename SourceCoordinateSystem>
Rotation<CoordinateSystem>::Rotation(const Rotation<SourceCoordinateSystem> & sourceRotation) :
	transform(CoordinateTransform<SourceCoordinateSystem, CoordinateSystem>::sourceToTarget(sourceRotation.transform)),
	yaw(sourceRotation.yaw),
	pitch(sourceRotation.pitch),
	roll(sourceRotation.roll)
{
	// empty
}


template <typename CoordinateSystem>
typename Rotation<CoordinateSystem>::NamedVector Rotation<CoordinateSystem>::apply(const NamedVector & v)
{
	return NamedVector(transform.get() * v.get());
}
