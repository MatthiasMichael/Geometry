#include <iostream>

#include "GeometryOperations.h"
#include "commonTypes.h"

void test3D()
{
	Line3D line1{ Vector3(2, 0, 0), make_named<Direction3D>(-1, 0, 0) };
	Line3D line2{ Vector3(0, 2, 0), make_named<Direction3D>(0, -1, 0) };

	std::cout << "Intersection point line1 - line2:\n" << intersectLineAndLine(line1, line2) << std::endl;

	Line3D line3{ Vector3(0, 0, 10), make_named<Direction3D>(0, 0, 1) };
	Plane3D plane{ Vector3(0, 0, 0), make_named<Direction3D>(1, 0, 0), make_named<Direction3D>(0, 1, 0) };

	const auto &[point, distance] = intersectLineAndPlane(line3, plane);
	std::cout << "Intersection point line3 - plane:\n" << point << "\nDistance: " << distance << std::endl;
}


int main()
{
	Line2D line{ Vector2{1, 0}, make_named<Direction2D>(0, 1) };
	Plane3D plane{ Vector3{1, 1, 0}, make_named<Direction3D>(1, 0, 0), make_named<Direction3D>(0, 0, 1) };

	std::cout << "Distance Plane to Origin: " << plane.getDistanceFromOrigin().get() << std::endl;
	std::cout << "Plane Normal: " << plane.getNormal().get() << std::endl;

	test3D();

	return EXIT_SUCCESS;
}
