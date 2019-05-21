#pragma once

#include <vector>

#include "ImageCoordinateSystem.h"

class BoundingBox
{
public:
	BoundingBox();
	explicit BoundingBox(const std::vector<ImagePoint> & points);
	
	template<typename ScalarType>
	static BoundingBox fromImageVectors(const std::vector<ImageVector_T<ScalarType>> & points);

	int numRows() const;
	int numCols() const;

	bool isEmpty() const;
	bool isInside(const ImagePoint & p) const;

	template<typename ScalarType>
	bool isInside(const ImageVector_T<ScalarType> & p) const;

	template<typename ScalarType>
	ImageVector_T<ScalarType> clip(const ImageVector_T<ScalarType> & p) const;

	const ImagePoint & topLeft() const { return m_topLeft; }
	const ImagePoint & bottomRight() const { return m_bottomRight; }

	int top() const { return m_topLeft(1); }
	int left() const { return m_topLeft(0); }
	int bottom() const { return m_bottomRight(1); }
	int right() const { return m_bottomRight(0); }

	bool operator==(const BoundingBox & other) const;

private:
	static const BoundingBox s_emptyBox;

	ImagePoint m_topLeft;
	ImagePoint m_bottomRight;
};


template <typename ScalarType>
BoundingBox BoundingBox::fromImageVectors(const std::vector<ImageVector_T<ScalarType>> & points)
{
	using ImageVector = ImageVector_T<ScalarType>;

	ImageVector topLeft = make_named<ImageVector>(std::numeric_limits<ScalarType>::max(), std::numeric_limits<ScalarType>::max());
	ImageVector bottomRight = make_named<ImageVector>(std::numeric_limits<ScalarType>::lowest(), std::numeric_limits<ScalarType>::lowest());

	for (const auto & p : points)
	{
		topLeft(0) = std::min(topLeft(0), p(0));
		topLeft(1) = std::min(topLeft(1), p(1));

		bottomRight(0) = std::max(bottomRight(0), p(0));
		bottomRight(1) = std::max(bottomRight(1), p(1));
	}
	BoundingBox bounds;
	bounds.m_topLeft = make_named<ImagePoint>(
		static_cast<int>(std::ceil(topLeft(0))),
		static_cast<int>(std::ceil(topLeft(1)))
		);

	bounds.m_bottomRight = make_named<ImagePoint>(
		static_cast<int>(std::floor(bottomRight(0))),
		static_cast<int>(std::floor(bottomRight(1)))
		);

	return bounds;
}


template <typename ScalarType>
bool BoundingBox::isInside(const ImageVector_T<ScalarType> & p) const
{
	return 
		p(0) >= static_cast<ScalarType>(m_topLeft(0)) && 
		p(0) <= static_cast<ScalarType>(m_bottomRight(0)) && 
		p(1) >= static_cast<ScalarType>(m_topLeft(1)) && 
		p(1) <= static_cast<ScalarType>(m_bottomRight(1));
}


template <typename ScalarType>
ImageVector_T<ScalarType> BoundingBox::clip(const ImageVector_T<ScalarType> & p) const
{
	return make_named<ImageVector_T<ScalarType>>(
		std::max(std::min(static_cast<ScalarType>(right()), p(0)), static_cast<ScalarType>(left())),
		std::max(std::min(static_cast<ScalarType>(bottom()), p(1)), static_cast<ScalarType>(top()))
	);
}
