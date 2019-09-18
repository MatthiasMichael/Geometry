CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

PROJECT( Geometry_Tests )

ADD_CUSTOM_TARGET( Geometry_Test_Common SOURCES
	common/commonTypes.h
)

SET_PROPERTY( TARGET Geometry_Test_Common PROPERTY FOLDER "Tests")

IF( BUILD_EXAMPLE_COORDINATE_TRANSFORM )
	ADD_SUBDIRECTORY( test_coordinate_transform/cmake ${CMAKE_CURRENT_BINARY_DIR}/test_coordinate_transform)
ENDIF( )

IF( BUILD_EXAMPLE_PROJECTIVE_TRANSFORM )
	ADD_SUBDIRECTORY( test_projective_transform/cmake ${CMAKE_CURRENT_BINARY_DIR}/test_projective_transform)
ENDIF( )

IF( BUILD_EXAMPLE_CONVEX_HULL )
	ADD_SUBDIRECTORY( test_convex_hull/cmake ${CMAKE_CURRENT_BINARY_DIR}/test_convex_hull)
ENDIF( )

IF( BUILD_EXAMPLE_OPERATIONS )
	ADD_SUBDIRECTORY( test_operations/cmake ${CMAKE_CURRENT_BINARY_DIR}/test_operations)
ENDIF( )