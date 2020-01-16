CMAKE_MINIMUM_REQUIRED( VERSION 3.10 )

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

SET( BUILD_EXAMPLE_COORDINATE_TRANSFORM OFF CACHE BOOL "-" )
SET( BUILD_EXAMPLE_PROJECTIVE_TRANSFORM OFF CACHE BOOL "-" )
SET( BUILD_EXAMPLE_CONVEX_HULL OFF CACHE BOOL "-" )
SET( BUILD_EXAMPLE_OPERATIONS OFF CACHE BOOL "-" )

PROJECT( Geometry )

IF(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo_multi.cmake)
    INCLUDE(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo_multi.cmake)
ELSE()
    INCLUDE(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
ENDIF()

CONAN_BASIC_SETUP(TARGETS)

INCLUDE_DIRECTORIES(
	../include
)

SET( INCLUDE_FILES_QVM
	../include/qvm_cuda.h
	../include/qvm_eigen.h
	../include/qvm_osg.h
)

SET( INCLUDE_FILES
	${INCLUDE_FILES_QVM}
	../include/BoundingBox.h
	../include/Camera.h
	../include/ConvexHull.h
	../include/CoordinateTransform.h
	../include/CoordinateSystem.h
	../include/GeometricTypes.h
	../include/GeometrySkills.h
	../include/GeometryUtils.h
	../include/GeometryOperations.h
	../include/ImageTransform.h
	../include/Rotation.h
	../include/ScanLine.h
	../include/WorldCoordinateSystem.h
	../include/ImageCoordinateSystem.h
	../include/Line.h
	../include/Plane.h
	../include/fitProjectiveTransform.h
)

SET( SOURCE_FILES
	../src/BoundingBox.cpp
	../src/ScanLine.cpp
)

ADD_LIBRARY( ${PROJECT_NAME} STATIC
	${INCLUDE_FILES}
	${SOURCE_FILES}
)

TARGET_LINK_LIBRARIES( ${PROJECT_NAME}
	CONAN_PKG::NamedType
	CONAN_PKG::Eigen3
	CONAN_PKG::boost
)

SOURCE_GROUP( "Header Files\\QVM" FILES ${INCLUDE_FILES_QVM} )

SET_PROPERTY( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Algorithms")

IF( BUILD_EXAMPLE_COORDINATE_TRANSFORM OR BUILD_EXAMPLE_PROJECTIVE_TRANSFORM OR BUILD_EXAMPLE_CONVEX_HULL OR BUILD_EXAMPLE_OPERATIONS )
	ADD_SUBDIRECTORY( ../test ${CMAKE_CURRENT_BINARY_DIR}/test )
ENDIF()

