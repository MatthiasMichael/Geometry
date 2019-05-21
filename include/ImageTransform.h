//#pragma once
//
//#include "rtcvImage.h"
//
//#include "ConvexHull.h"
//#include "GeometryUtils.h"
//
//
//template<typename ScalarType, typename Transformation>
//class ImageTransform
//{
//public:
//	explicit ImageTransform(const Transformation & transform);
//
//	template<typename T>
//	rtcvImage<T> transform(const rtcvImage<T> & img);
//
//	template<typename T>
//	rtcvImage<T> transformFast(const rtcvImage<T> & img);
//
//private:
//	Transformation m_transform;
//	Transformation m_inverseTransform;
//};
//
//
//template <typename ScalarType, typename Transformation>
//ImageTransform<ScalarType, Transformation>::ImageTransform(const Transformation & transform) :
//	m_transform(transform),
//	m_inverseTransform(transform.inverse())
//{
//	// empty
//}
//
//
//template <typename ScalarType, typename Transformation>
//template <typename T>
//rtcvImage<T> ImageTransform<ScalarType, Transformation>::transform(const rtcvImage<T> & img)
//{
//	using Vector3 = typename GeometricTypes<ScalarType, 3>::Vector;
//	using Vector2 = typename GeometricTypes<ScalarType, 2>::Vector;
//	using Vector2i = GeometricTypes<int, 2>::Vector;
//
//	auto topLeft = m_transform * Vector3(0.f, 0.f, 1.f);
//	auto topRight = m_transform * Vector3(img.getSx(), 0, 1.f);
//	auto bottomRight = m_transform * Vector3(img.getSx(), img.getSy(), 1.f);
//	auto bottomLeft = m_transform * Vector3(0, img.getSy(), 1.f);
//
//	topLeft /= topLeft(2);
//	topRight /= topRight(2);
//	bottomRight /= bottomRight(2);
//	bottomLeft /= bottomLeft(2);
//
//	const Vector2i newTopLeft(
//		static_cast<int>(std::ceil(std::min(std::min(topLeft(0), topRight(0)), std::min(bottomLeft(0), bottomRight(0))))),
//		static_cast<int>(std::ceil(std::min(std::min(topLeft(1), topRight(1)), std::min(bottomLeft(1), bottomRight(1)))))
//		);
//
//	const Vector2i newBottomRight(
//		static_cast<int>(std::floor(std::max(std::max(topLeft(0), topRight(0)), std::max(bottomLeft(0), bottomRight(0))))),
//		static_cast<int>(std::floor(std::max(std::max(topLeft(1), topRight(1)), std::max(bottomLeft(1), bottomRight(1)))))
//	);
//
//	const Vector2i newImageSize = newBottomRight - newTopLeft;
//
//	rtcvImage<T> newImage(newImageSize(0), newImageSize(1));
//	newImage.fill(T());
//
//	for(int y = 0; y < newImageSize(1); ++y)
//	{
//		for(int x = 0; x < newImageSize(0); ++x)
//		{
//			auto src = m_inverseTransform * Vector3(x + newTopLeft(0), y + newTopLeft(1), 1);
//			src /= src(2);
//			if (src(0) < 0 || src(1) < 0 || src(0) >= img.getSx() - 1 || src(1) >= img.getSy() - 1)
//			{
//				//std::cout << "Outside" << std::endl;
//				continue;
//			}
//			newImage.setValue(x, y, img.getValue(src(0), src(1)));
//		}
//	}
//
//	return newImage;
//}
//
//
//template <typename ScalarType, typename Transformation>
//template <typename T>
//rtcvImage<T> ImageTransform<ScalarType, Transformation>::transformFast(const rtcvImage<T> & img)
//{
//	using ImageVector = ImageVector_T<ScalarType>;
//
//	using Vector3 = typename GeometricTypes<ScalarType, 3>::Vector;
//	using Vector2 = typename GeometricTypes<ScalarType, 2>::Vector;
//
//	auto topLeft = m_transform * Vector3(0.f, 0.f, 1.f);
//	auto topRight = m_transform * Vector3(img.getSx(), 0, 1.f);
//	auto bottomRight = m_transform * Vector3(img.getSx(), img.getSy(), 1.f);
//	auto bottomLeft = m_transform * Vector3(0, img.getSy(), 1.f);
//
//	topLeft /= topLeft(2);
//	topRight /= topRight(2);
//	bottomRight /= bottomRight(2);
//	bottomLeft /= bottomLeft(2);
//
//	ConvexHull<ScalarType> hull(
//		{
//			make_named<ImageVector>(topLeft(0), topLeft(1)),
//			make_named<ImageVector>(topRight(0), topRight(1)),
//			make_named<ImageVector>(bottomRight(0), bottomRight(1)),
//			make_named<ImageVector>(bottomLeft(0), bottomLeft(1))
//		}
//	);
//
//	auto lines = hull.calcScanLines();
//
//
//	rtcvImage<T> newImage(hull.getBounds().numCols(), hull.getBounds().numRows());
//	newImage.fill(T());
//
//	for (const auto & l : lines)
//	{
//		for (int x = l.getStartX(); x < l.getEndX(); ++x)
//		{
//			auto src = m_inverseTransform * Vector3(x, l.getY(), 1);
//			src /= src(2);
//			if (src(0) < 0 || src(1) < 0 || src(0) >= img.getSx() - 1 || src(1) >= img.getSy() - 1)
//			{
//				//std::cout << "Outside" << std::endl;
//				continue;
//			}
//			newImage.setValue(x - hull.getBounds().left(), l.getY() - hull.getBounds().top(), img.getValue(src(0), src(1)));
//		}
//	}
//
//	return newImage;
//
//}
