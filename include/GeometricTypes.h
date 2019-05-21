#pragma once

#include "Eigen/Core"
#include "Eigen/Geometry"


template<typename TScalar_, int Dim>
struct GeometricTypes
{
	using Vector = Eigen::Matrix<TScalar_, Dim, 1>;
	using Matrix = Eigen::Matrix<TScalar_, Dim, Dim>;

	using HomogeneousVector = Eigen::Matrix<TScalar_, Dim + 1, 1>;
	using HomogeneousMatrix = Eigen::Matrix<TScalar_, Dim + 1, Dim + 1>;
	
	struct Transform
	{
		using Translation = Eigen::Translation<TScalar_, Dim>;
		using Affine = Eigen::Transform<TScalar_, Dim, Eigen::Affine>;
		using Projective = Eigen::Transform<TScalar_, Dim, Eigen::Projective>;

		template<int TransformationType>
		using General = Eigen::Transform<TScalar_, Dim, TransformationType>;
	};
};