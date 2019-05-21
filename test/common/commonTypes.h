#pragma once

#include "GeometricTypes.h"
#include "GeometryUtils.h"

#include "WorldCoordinateSystem.h"
#include "ImageCoordinateSystem.h"

#include "Rotation.h"

#include "Line.h"
#include "Plane.h"

using ScalarType = double;

using WorldCoordinateSystem = WorldCoordinateSystem_T<ScalarType>;

using WorldVector = WorldVector_T<ScalarType>;
using WorldTransform = WorldTransform_T<ScalarType>;

using Vector2 = GeometricTypes<ScalarType, 2>::Vector;
using Vector3 = GeometricTypes<ScalarType, 3>::Vector;

using Matrix33 = GeometricTypes<ScalarType, 3>::Matrix;

using Transform = GeometricTypes<ScalarType, 3>::Transform::Affine;
using Translation = GeometricTypes<ScalarType, 3>::Transform::Translation;

using Degrees = NamedScalarTypes<ScalarType>::Degrees;
using Radians = NamedScalarTypes<ScalarType>::Radians;

using WorldRotation = Rotation<WorldCoordinateSystem>;

using ImageVector = ImageVector_T<ScalarType>;

using Direction2D = NamedVectorTypes<ScalarType, 2>::Direction;
using Direction3D = NamedVectorTypes<ScalarType, 3>::Direction;

using Line2D = Line<ScalarType, 2>;
using Line3D = Line<ScalarType, 3>;

using Plane3D = Plane<ScalarType, 3>;