#pragma once

#include <vector>

#include "Eigen/Core"

#include "named_type.h"

#include "GeometricTypes.h"
#include "GeometrySkills.h"


template<int NumDimensions_, typename TScalar_, typename Tag_>
class CoordinateSystem
{
public:
	using TScalar = TScalar_;
	using Tag = Tag_;
	static constexpr int NumDimensions = NumDimensions_;

	using Vector = typename GeometricTypes<TScalar, NumDimensions>::Vector;
	using Matrix = typename GeometricTypes<TScalar, NumDimensions>::Matrix;

	using HomogeneousVector = typename GeometricTypes<TScalar, NumDimensions>::HomogeneousVector;
	using HomogeneousMatrix = typename GeometricTypes<TScalar, NumDimensions>::HomogeneousMatrix;

	using AffineTransform = typename GeometricTypes<TScalar, NumDimensions>::Transform::Affine;

	using NamedVector = NamedType<Vector, CoordinateSystem<NumDimensions_, TScalar_, Tag_>, VectorTypeSkills>;
	using NamedHomogeneousVector = NamedType<HomogeneousVector, CoordinateSystem<NumDimensions_, TScalar_, Tag_>, VectorTypeSkills>;

	using NamedAffineTransform = NamedType<AffineTransform, CoordinateSystem<NumDimensions_, TScalar_, Tag_>>;
	
	using ModifyCallback = std::function<void()>;

	CoordinateSystem() = delete;

	static void setDirections(std::initializer_list<Vector> directions)
	{
		//static_assert(NumDimensions > 0);
		assert(directions.size() == NumDimensions);

		int col = 0;
		for (const auto v : directions)
		{
			s_directions.col(col++) = v;
		}

		notifyListeners();
	}

	static void setDirections(const Matrix & directions)
	{
		s_directions = directions;
		notifyListeners();
	}

	static void setOrigin(const Vector & origin)
	{
		s_origin = origin;
		notifyListeners();
	}

	static void transform(const AffineTransform & transform)
	{
		s_directions = transform.linear() * s_directions;
		s_origin = transform * s_origin;
		notifyListeners();
	}

	static Vector getDirection(int i) 
	{
		assert(i > 0 && i < NumDimensions);
		return s_directions.col(i);
	}

	static Matrix getDirections()
	{
		return s_directions;
	}

	static Vector getOrigin()
	{
		return s_origin;
	}

	static void addCallback(ModifyCallback f)
	{
		callbackOnModify().push_back(f);
	}

private:
	static void notifyListeners()
	{
		for (auto f : callbackOnModify())
		{
			f();
		}
	}

	static std::vector<ModifyCallback> & callbackOnModify()
	{
		static std::vector<ModifyCallback> s_callbackOnModify;
		return s_callbackOnModify;
	}

	static Matrix s_directions;
	static Vector s_origin;
};

template<int NumDimensions_, typename TScalar_, typename Tag_>
typename 
CoordinateSystem<NumDimensions_, TScalar_, Tag_>::Matrix
CoordinateSystem<NumDimensions_, TScalar_, Tag_>::s_directions = Matrix::Identity();

template<int NumDimensions_, typename TScalar_, typename Tag_>
typename
CoordinateSystem<NumDimensions_, TScalar_, Tag_>::Vector
CoordinateSystem<NumDimensions_, TScalar_, Tag_>::s_origin = Vector::Zero();