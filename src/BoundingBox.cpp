#include "BoundingBox.h"

const BoundingBox BoundingBox::s_emptyBox = BoundingBox();

BoundingBox::BoundingBox() :
	m_topLeft(make_named<ImagePoint>(std::numeric_limits<int>::max(), std::numeric_limits<int>::max())),
	m_bottomRight(make_named<ImagePoint>(std::numeric_limits<int>::lowest(), std::numeric_limits<int>::lowest()))
{
	// empty
}

BoundingBox::BoundingBox(const std::vector<ImagePoint> & points) : BoundingBox()
{
	for(const auto & p : points)
	{
		m_topLeft(0) = std::min(m_topLeft(0), p(0));
		m_topLeft(1) = std::min(m_topLeft(1), p(1));

		m_bottomRight(0) = std::max(m_bottomRight(0), p(0));
		m_bottomRight(1) = std::max(m_bottomRight(1), p(1));
	}
}

int BoundingBox::numRows() const
{
	return (isEmpty() || m_topLeft(1) == m_bottomRight(1)) ? 0 : m_bottomRight(1) - m_topLeft(1) + 1;
}

int BoundingBox::numCols() const
{
	return (isEmpty() || m_topLeft(0) == m_bottomRight(0)) ? 0 : m_bottomRight(0) - m_topLeft(0) + 1;
}

bool BoundingBox::isInside(const ImagePoint & p) const
{
	return 
		p(0) >= m_topLeft(0) && 
		p(0) <= m_bottomRight(0) && 
		p(1) >= m_topLeft(1) && 
		p(1) <= m_bottomRight(1);
}

bool BoundingBox::isEmpty() const
{
	return *this == s_emptyBox;
}


bool BoundingBox::operator==(const BoundingBox & other) const 
{
	return m_topLeft == other.m_topLeft && m_bottomRight == other.m_bottomRight;
}
