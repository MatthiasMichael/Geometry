#pragma once

#include <iomanip>
#include <vector>

#include "named_type.h"

#include "GeometricTypes.h"
#include "GeometryUtils.h"
#include "ImageCoordinateSystem.h"
#include "Rotation.h"


template<typename WorldCoordinateSystem>
struct ViewRay
{
	static constexpr int NumDimensions = WorldCoordinateSystem::NumDimensions;

	using WorldVector = typename WorldCoordinateSystem::NamedVector;

	WorldVector origin;
	WorldVector direction;
};


template <typename WorldCoordinateSystem>
class Camera
{
public:
	static constexpr int NumDimensions = WorldCoordinateSystem::NumDimensions;

	using TScalar = typename WorldCoordinateSystem::TScalar;
	using Matrix = typename GeometricTypes<TScalar, 3>::Matrix;
	using Transform = typename GeometricTypes<TScalar, 3>::Transform::Affine;

	using WorldVector = typename WorldCoordinateSystem::NamedVector;
	using WorldTransform = typename WorldCoordinateSystem::NamedAffineTransform;

	struct CameraVectorTag{};
	using CameraVector = NamedType<typename GeometricTypes<TScalar, 3>::Vector, CameraVectorTag, VectorTypeSkills>;

	using HomogeneousImageVector = typename ImageVectorCoordinateSystem_T<TScalar>::NamedHomogeneousVector;

	using Size = NamedVectorTypes<int, 2>::Size;


	struct FocalLength
	{
		FocalLength(TScalar focalLength) : x(focalLength), y(focalLength) { }
		FocalLength(TScalar x, TScalar y) : x(x), y(y) { }

		TScalar x, y;
	};


	Camera() : m_extrinsicMatrix(), m_intrinsicMatrix(), m_imageSize(make_named<Size>(0, 0))
	{
		// Currently the transformation only works if the coordinates are given w.r.t.
		// to the standard basis. Other World Coordinate Systems require previous conversions
		assert(WorldCoordinateSystem::getDirections() == Matrix::Identity());
	}

	void setImageSize(const Size & imageSize)
	{
		m_imageSize = imageSize;
	}

	void setBaseDirections(const std::vector<WorldVector> & baseDirections)
	{
		assert(baseDirections.size() == NumDimensions);

		Matrix cameraDirections;
		for (size_t dir = 0; dir < NumDimensions; ++dir)
		{
			cameraDirections.col(dir) = baseDirections[dir].get();
		}

		setBaseDirections(cameraDirections);
	}

	void setBaseDirections(const Matrix & baseDirections)
	{
		m_extrinsicMatrix.setDirections(baseDirections);
	}

	void setOrigin(const WorldVector & origin)
	{
		m_extrinsicMatrix.setOrigin(origin);
	}

	void setFocalLength(const FocalLength & focalLength)
	{
		m_intrinsicMatrix.setFocalLength(focalLength);
	}

	void setOpticalCenter(const ImagePoint & center)
	{
		m_intrinsicMatrix.setOpticalCenter(center);
	}

	void extrinsicTransform(const WorldTransform & transform)
	{
		m_extrinsicMatrix.transformCamera(transform);
	}

	void extrinsicTransform(const Rotation<WorldCoordinateSystem> & rotation)
	{
		m_extrinsicMatrix.transformCamera(rotation.transform);
	}

	Size getImageSize() const
	{
		return m_imageSize;
	}

	WorldVector getOrigin() const
	{
		return m_extrinsicMatrix.getOrigin();
	}

	CameraVector worldToCamera(const WorldVector & p) const
	{
		return m_extrinsicMatrix.worldToCamera(p);
	}

	ImagePoint cameraToImage(const CameraVector & p) const
	{
		return m_intrinsicMatrix.cameraToImage(p);
	}

	ImagePoint worldToImage(const WorldVector & p) const
	{
		return cameraToImage(worldToCamera(p));
	}

	CameraVector imageToCamera(const ImagePoint & p) const
	{
		return m_intrinsicMatrix.imageToCamera(p);
	}

	ViewRay<WorldCoordinateSystem> cameraToWorld(const CameraVector & p) const
	{
		return m_extrinsicMatrix.cameraToWorld(p);
	}

	ViewRay<WorldCoordinateSystem> imageToWorld(const ImagePoint & p) const
	{
		return cameraToWorld(imageToCamera(p));
	}

	bool isVisible(const ImagePoint & p) const
	{
		return p(0) >= 0 && p(1) >= 0 && p(0) < m_imageSize(0) && p(1) < m_imageSize(1);
	}

	bool isVisible(const CameraVector & p) const
	{
		return p(2) >= 0 && isVisible(cameraToImage(p));
	}

	bool isVisible(const WorldVector & p) const
	{
		return isVisible(worldToCamera(p));
	}

	std::tuple<CameraVector, bool> worldToCameraWithVisibilityCheck(const WorldVector & p) const
	{
		const auto cameraVector = worldToCamera(p);
		return std::make_tuple(cameraVector, isVisible(cameraVector));
	}

	std::tuple<ImagePoint, bool> worldToImageWithVisibilityCheck(const WorldVector & p) const
	{
		// Prohibit multiple transformations
		const auto cameraVector = worldToCamera(p);
		const auto imagePoint = cameraToImage(cameraVector);

		return std::make_tuple(imagePoint, cameraVector(2) >= 0 && isVisible(imagePoint));
	}

	bool operator==(const Camera<WorldCoordinateSystem> & other) const
	{
		return m_extrinsicMatrix == other.m_extrinsicMatrix &&
			m_intrinsicMatrix == other.m_intrinsicMatrix &&
			m_imageSize == other.m_imageSize;
	}

	friend std::ostream & operator<<(std::ostream & os, const Camera & m)
	{
		os << m.m_extrinsicMatrix << std::endl << m.m_intrinsicMatrix << std::endl;
		os << m.m_imageSize(0) << " " << m.m_imageSize(1);
		return os;
	}

	friend std::istream & operator>>(std::istream & is, Camera & m)
	{
		is >> m.m_extrinsicMatrix >> m.m_intrinsicMatrix;
		is >> m.m_imageSize(0) >> m.m_imageSize(1);
		return is;
	}


private:
	class ExtrinsicMatrix
	{
	public:
		ExtrinsicMatrix() : 
			m_transform(Transform::Identity()), 
			m_inverseTransform(Transform::Identity())
		{
			// empty
		}
		
		void setDirections(const Matrix & directions)
		{
			m_inverseTransform.linear() = directions;
			m_transform.linear() = directions.transpose();
		}

		void setOrigin(const WorldVector & origin)
		{
			m_inverseTransform.translation() = origin;
			m_transform.translation() = -1 * m_transform.linear() * origin;
		}

		void transformCamera(const WorldTransform & transform)
		{
			m_inverseTransform = transform.get() * m_inverseTransform;
			m_transform = m_inverseTransform.inverse();
		}

		Matrix getDirections() const
		{
			return m_inverseTransform.linear();
		}

		WorldVector getOrigin() const
		{
			return WorldVector(m_inverseTransform.translation());
		}

		CameraVector worldToCamera(const WorldVector & v) const
		{
			return CameraVector(m_transform * v.get());
		}

		ViewRay<WorldCoordinateSystem> cameraToWorld(const CameraVector & v) const
		{
			const WorldVector direction = WorldVector(m_inverseTransform * v.get()) - getOrigin();
			return ViewRay<WorldCoordinateSystem>{
				getOrigin(), WorldVector(direction.get().normalized())
			};
		}

		bool operator==(const ExtrinsicMatrix & other) const { return m_transform.isApprox(other.m_transform); }
		
		friend std::ostream & operator<<(std::ostream & os, const ExtrinsicMatrix & m)
		{			
			const auto mat = m.m_transform.matrix();

			for(int r = 0; r < mat.rows(); ++r)
			{
				for(int c = 0; c < mat.cols(); ++c)
				{
					os << std::setprecision(std::numeric_limits<TScalar>::max_digits10) << mat(r, c);
					if(r != mat.rows() - 1 || c != mat.cols() - 1)
					{
						os << " ";
					}
				}
			}
			return os;
		}

		friend std::istream & operator>>(std::istream & is, ExtrinsicMatrix & m)
		{
			std::decay_t<decltype(m.m_transform.matrix())> mat;

			for (int r = 0; r < mat.rows(); ++r)
			{
				for (int c = 0; c < mat.cols(); ++c)
				{
					is >> mat(r, c);
				}
			}

			m.m_transform = Transform(mat);
			m.m_inverseTransform = m.m_transform.inverse();
			return is;
		}

	private:
		Transform m_transform;
		Transform m_inverseTransform;
	};

	class IntrinsicMatrix
	{
	public:
		IntrinsicMatrix() : 
			m_transform(Transform::Identity()),
			m_inverseTransform(Transform::Identity())
		{
			// empty
		}

		void setFocalLength(const FocalLength & f)
		{
			m_transform(0, 0) = f.x;
			m_transform(1, 1) = f.y;
			
			m_inverseTransform = m_transform.inverse();
		}

		void setOpticalCenter(const ImagePoint & center)
		{
			m_transform(0, 2) = static_cast<TScalar>(center(0));
			m_transform(1, 2) = static_cast<TScalar>(center(1));
			
			m_inverseTransform = m_transform.inverse();
		}

		FocalLength getFocalLength() const
		{
			return FocalLength(m_transform(0, 0), m_transform(1, 1));
		}

		ImagePoint getOpticalCenter() const
		{
			return ImagePoint(m_transform(0, 2), m_transform(1, 2));
		}

		ImagePoint cameraToImage(const CameraVector & v) const
		{
			const HomogeneousImageVector pImg(m_transform * v.get());
			return make_named<ImagePoint>(round(pImg(0) / pImg(2)), round(pImg(1) / pImg(2)));
		}

		CameraVector imageToCamera(const ImagePoint & v) const
		{
			HomogeneousImageVector pImg3;
			pImg3.get() << v.get().cast<TScalar>(), 1;
			return CameraVector(m_inverseTransform * pImg3.get());
		}

		bool operator==(const IntrinsicMatrix & other) const { return m_transform.isApprox(other.m_transform); }

		friend std::ostream & operator<<(std::ostream & os, const IntrinsicMatrix & m)
		{
			const auto mat = m.m_transform.matrix();

			for (int r = 0; r < mat.rows(); ++r)
			{
				for (int c = 0; c < mat.cols(); ++c)
				{
					os << std::setprecision(std::numeric_limits<TScalar>::max_digits10) << mat(r, c);
					if (r != mat.rows() - 1 || c != mat.cols() - 1)
					{
						os << " ";
					}
				}
			}
			return os;
		}

		friend std::istream & operator>>(std::istream & is, IntrinsicMatrix & m)
		{
			std::decay_t<decltype(m.m_transform.matrix())> mat;

			for (int r = 0; r < mat.rows(); ++r)
			{
				for (int c = 0; c < mat.cols(); ++c)
				{
					is >> mat(r, c);
				}
			}

			m.m_transform = Transform(mat);
			m.m_inverseTransform = m.m_transform.inverse();
			return is;
		}

	private:
		Transform m_transform;
		Transform m_inverseTransform;
	};

private:
	ExtrinsicMatrix m_extrinsicMatrix;
	IntrinsicMatrix m_intrinsicMatrix;

	Size m_imageSize;
};


template<typename WorldCoordinateSystem>
struct CameraHash
{
	size_t operator()(const Camera<WorldCoordinateSystem> & k) const
	{
		const auto v = k.getOrigin().get();
		size_t seed = 3;
		for (int i = 0; i < 3; ++i) {
			seed ^= static_cast<size_t>(v(i) * 64 * 64 + 64) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}

		return seed;
	}
};