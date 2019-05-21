#pragma once

#include <vector>

#include "BoundingBox.h"
#include "GeometricTypes.h"
#include "GeometryUtils.h"
#include "GeometryOperations.h"
#include "ScanLine.h"

#include "ImageCoordinateSystem.h"



template<typename ScalarType>
class ConvexHull
{
public:
	using ImageVector = ImageVector_T<ScalarType>;

	ConvexHull(std::vector<ImageVector> corners);

	/*bool isInside(const ImagePoint & p);
	bool isInside(const ImageVector & p);*/

	std::vector<ScanLine> calcScanLines();

	const std::vector<ImageVector> & getHull() const { return m_hull; }
	const BoundingBox & getBounds() const { return m_bounds; }

private:
	static std::vector<ImageVector> clipHull(const BoundingBox & bounds, const std::vector<ImageVector> & hull);

private:
	std::vector<ImageVector> m_hull;
	BoundingBox m_bounds;
};


template<typename ScalarType>
ConvexHull<ScalarType>::ConvexHull(std::vector<ImageVector> points) :
	m_hull(2 * points.size(), make_named<ImageVector>(0, 0))
{
	std::sort(points.begin(), points.end(), [](const ImageVector & lhs, const ImageVector & rhs)
	{
		return lhs(0) < rhs(0) || (lhs(0) == rhs(0) && lhs(1) < rhs(1));
	});

	int k = 0;

	// Build lower hull
	for (int i = 0; i < points.size(); ++i)
	{
		while (k >= 2 && cross<ScalarType>(m_hull[k - 2], m_hull[k - 1], points[i]) <= 0)
		{
			k--;
		}
		m_hull[k++] = points[i];
	}

	// Build upper hull
	for (int i = static_cast<int>(points.size()) - 2, t = k + 1; i >= 0; i--)
	{
		while (k >= t && cross<ScalarType>(m_hull[k - 2], m_hull[k - 1], points[i]) <= 0)
		{
			k--;
		}
		m_hull[k++] = points[i];
	}

	m_hull.resize(k - 1);
	m_bounds = BoundingBox::fromImageVectors<ScalarType>(m_hull);
}


//template<typename ScalarType>
//bool ConvexHull<ScalarType>::isInside(const ImagePoint & p) const
//{
//	return true;
//}


template<typename ScalarType>
std::vector<ScanLine> ConvexHull<ScalarType>::calcScanLines()
{
	using Line = Line<ScalarType, 2>;
	using Direction = typename Line::Direction;

	const std::vector<ImageVector> & clippedHull = clipHull(m_bounds, m_hull);

	int numPointsInHull = 0;

	std::vector<ScanLine> lines(m_bounds.numRows(), ScanLine());

	for (size_t i = 0, j = clippedHull.size() - 1; i < clippedHull.size(); j = i++)
	{
		const ImagePoint corner = cast<ScalarType>(clippedHull[i]);

		int linesIdx = corner(1) - m_bounds.top();
		
		numPointsInHull += lines[linesIdx].add(corner);

		const ImageVector p1 = clippedHull[j](1) <= clippedHull[i](1) ? clippedHull[j] : clippedHull[i];
		const ImageVector p2 = clippedHull[j](1) >  clippedHull[i](1) ? clippedHull[j] : clippedHull[i];

		const Direction dir1 = make_named<Direction>(p2.get() - p1.get());
		const Direction dir2 = make_named<Direction>(1, 0);

		const Line polygonLine{ p1.get(), dir1 };

		for (ScalarType y = p1(1) + 1; y < p2(1); y += 1)
		{
			const ImageVector pointOnLine = make_named<ImageVector>(p1(0), y);
			const Line horizontalLine{ pointOnLine.get(), dir2 };

			auto intersectionPoint = make_named<ImageVector>(intersectLineAndLine(polygonLine, horizontalLine));
			assert(std::abs(intersectionPoint(1) - y) < 0.5f);

			const int currentRow = static_cast<int>(y);
			const ImagePoint pointOnEdge = make_named<ImagePoint>(static_cast<int>(std::round(intersectionPoint(0))), currentRow);

			linesIdx = pointOnEdge(1) - m_bounds.top();
			numPointsInHull += lines[linesIdx].add(pointOnEdge);
		}
	}

	numPointsInHull += lines.front().finalize();
	numPointsInHull += lines.back().finalize();

	return lines;
}


template <typename ScalarType>
std::vector<typename ConvexHull<ScalarType>::ImageVector> ConvexHull<ScalarType>::clipHull(const BoundingBox & bounds, const std::vector<ImageVector> & hull)
{
	std::vector<ImageVector> clippedHull;
	clippedHull.reserve(hull.size());

	std::transform(hull.begin(), hull.end(), std::back_inserter(clippedHull), [&bounds](const ImageVector & p)
	{
		auto c = bounds.clip<ScalarType>(p);
		c(1) = std::round(c(1));

		return c;
	});

	for(const auto & p : clippedHull)
	{
		assert(bounds.isInside<ScalarType>(p));
	}

	return clippedHull;
}
