#pragma once

#include "CoordinateSystem.h"

#include "named_type.h"

struct WorldTag {};

//_T since you usually want to do
// using WorldCoordinateSystem = WorldCorrdinateSystem_T<float>;

template<typename TScalar>
using WorldCoordinateSystem_T = CoordinateSystem<3, TScalar, WorldTag>;

template<typename TScalar>
using WorldVector_T = typename WorldCoordinateSystem_T<TScalar>::NamedVector;

template<typename TScalar>
using WorldTransform_T = typename WorldCoordinateSystem_T<TScalar>::NamedAffineTransform;