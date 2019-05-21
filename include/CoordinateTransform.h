#pragma once

#include <Eigen/Core>
#include <Eigen/Geometry>

#include "named_type.h"

#include "GeometricTypes.h"


template <typename SourceCoordinateSystem,
          typename TargetCoordinateSystem>
class CoordinateTransform
{
	//static_assert(SourceCoordinateSystem::NumDimenions == TargetCoordinateSystem::NumDimensions);
	//static_assert(std::is_same<typename SourceCoordinateSystem::TScalar, typename TargetCoordinateSystem::TScalar>::value);

public:
	using TScalar = typename SourceCoordinateSystem::TScalar;
	static constexpr int NumDimensions = SourceCoordinateSystem::NumDimensions;

	// Matrix33 and Vector3 Types of Source and TargetTypes should be identical
	using Matrix = typename SourceCoordinateSystem::Matrix;
	using HomogeneousMatrix = typename SourceCoordinateSystem::HomogeneousMatrix;

	using Vector = typename SourceCoordinateSystem::Vector;
	using HomogeneousVector = typename SourceCoordinateSystem::HomogeneousVector;

	using AffineTransform = typename SourceCoordinateSystem::AffineTransform;

	using StrongSourceVector = typename SourceCoordinateSystem::NamedVector;
	using StrongTargetVector = typename TargetCoordinateSystem::NamedVector;

	using HomogeneousStrongSourceVector = typename SourceCoordinateSystem::NamedHomogeneousVector;
	using HomogeneousStrongTargetVector = typename TargetCoordinateSystem::NamedHomogeneousVector;

	using StrongSourceTransform = typename SourceCoordinateSystem::NamedAffineTransform;
	using StrongTargetTransform = typename TargetCoordinateSystem::NamedAffineTransform;

	CoordinateTransform() = delete;

	static StrongTargetVector sourceToTarget(const StrongSourceVector & v)
	{
		assureInitialization();
		return StrongTargetVector(m_transform * v.get());
	}


	static StrongSourceVector targetToSource(const StrongTargetVector & v)
	{
		assureInitialization();
		return StrongSourceVector(m_inverseTransform * v.get());
	}


	static StrongTargetTransform sourceToTarget(const StrongSourceTransform & t)
	{
		assureInitialization();
		return StrongTargetTransform(AffineTransform(m_inverseTransform * t.get() * m_transform));
	}


	static StrongSourceTransform targetToSource(const StrongTargetTransform & t)
	{
		assureInitialization();
		return StrongSourceTransform(AffineTransform(m_transform * t.get() * m_inverseTransform));
	}

	
private:
	static void * assureInitialization()
	{
		// This is necessary since the standard allows (requires?) to omit the initialization of
		// static members of a template class if they are not explicitly used.
		// See: https://stackoverflow.com/questions/1819131/
		return static_cast<void*>(&constructor);
	}

	static HomogeneousMatrix makeHomogeneous(const Matrix & matrix, const Vector & vector)
	{
		HomogeneousMatrix h;
		h << matrix, vector, Eigen::Matrix<TScalar, 1, NumDimensions>::Zero(), 1;
		return h;
	}

	static void refresh()
	{
		const Matrix sourceDirections = SourceCoordinateSystem::getDirections();
		const Matrix targetDirections = TargetCoordinateSystem::getDirections();

		const Vector sourceOrigin = SourceCoordinateSystem::getOrigin();
		const Vector targetOrigin = TargetCoordinateSystem::getOrigin();

		const HomogeneousMatrix source = makeHomogeneous(sourceDirections, sourceOrigin);
		const HomogeneousMatrix target = makeHomogeneous(targetDirections, targetOrigin);

		m_inverseTransform = AffineTransform(source.inverse() * target);
		m_transform = m_inverseTransform.inverse();
	}

private:
	static AffineTransform m_transform;
	static AffineTransform m_inverseTransform;

	friend class Constructor;

	static struct Constructor
	{
		using Parent = CoordinateTransform<SourceCoordinateSystem, TargetCoordinateSystem>;

		Constructor()
		{
			Parent::refresh();
			std::function<void()> callback = &Parent::refresh;
			SourceCoordinateSystem::addCallback(callback);
			TargetCoordinateSystem::addCallback(callback);
		}
	} constructor;

};

template <typename SourceCoordinateSystem, typename TargetCoordinateSystem>
typename CoordinateTransform<SourceCoordinateSystem, TargetCoordinateSystem>::AffineTransform
CoordinateTransform<SourceCoordinateSystem, TargetCoordinateSystem>::m_transform = AffineTransform::Identity();

template <typename SourceCoordinateSystem, typename TargetCoordinateSystem>
typename CoordinateTransform<SourceCoordinateSystem, TargetCoordinateSystem>::AffineTransform
CoordinateTransform<SourceCoordinateSystem, TargetCoordinateSystem>::m_inverseTransform = AffineTransform::Identity();

template <typename SourceCoordinateSystem, typename TargetCoordinateSystem>
typename CoordinateTransform<SourceCoordinateSystem, TargetCoordinateSystem>::Constructor
CoordinateTransform<SourceCoordinateSystem, TargetCoordinateSystem>::constructor;

template<typename TargetCoordinateSystem, typename StrongSourceVector>
typename TargetCoordinateSystem::NamedVector convertTo(const StrongSourceVector & v)
{
	return CoordinateTransform<typename StrongSourceVector::ParameterType, TargetCoordinateSystem>::sourceToTarget(v);
}