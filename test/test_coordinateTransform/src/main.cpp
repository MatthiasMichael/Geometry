#include <QApplication>

#include <boost/qvm/all.hpp>
#include "qvm_eigen.h"
#include "qvm_osg.h"

//#include "rtcvImage.h"

#include "Roi3DF_osg.h"

#include "QtOsgWidget.h"
#include "OsgGeometryManager.h"
#include "osg_utils.h"

#include "DisplayableAreaLights.h"
#include "DisplayableCameras.h"
#include "DisplayableCoordinateSystem.h"
#include "DisplayableGrid.h"

#include "CoordinateTransform.h"
#include "CoordinateSystem.h"

#include "Camera.h"
#include "Rotation.h"

#include "commonTypes.h"

struct UnrealTag {};

using UnrealCoordinateSystem = CoordinateSystem<3, ScalarType, UnrealTag>;

using UnrealVector = NamedType<UnrealCoordinateSystem::Vector, UnrealCoordinateSystem, VectorTypeSkills>;
using UnrealTransform = NamedType<UnrealCoordinateSystem::AffineTransform, UnrealCoordinateSystem>;

using UnrealToWorld = CoordinateTransform<UnrealCoordinateSystem, WorldCoordinateSystem>;

using UnrealRotation = Rotation<UnrealCoordinateSystem>;


UnrealTransform getCameraTransformation_ue(Degrees yaw, Degrees pitch, Degrees roll)
{
	const Radians angle_rotZ = deg2rad<ScalarType>(yaw);
	const Radians angle_rotY = deg2rad<ScalarType>(pitch);
	const Radians angle_rotX = deg2rad<ScalarType>(roll);

	const float cZ = cos(angle_rotZ.get());
	const float cY = cos(angle_rotY.get());
	const float cX = cos(angle_rotX.get());

	const float sZ = sin(angle_rotZ.get());
	const float sY = sin(angle_rotY.get());
	const float sX = sin(angle_rotX.get());

	Matrix33 mat_rotZ;
	Matrix33 mat_rotX;
	Matrix33 mat_rotY;

	mat_rotZ <<
		cZ , -sZ , 0 ,
		sZ , cZ , 0 ,
		0 , 0 , 1;

	mat_rotY <<
		cY , 0 , -sY ,
		0 , 1 , 0 ,
		sY , 0 , cY;

	mat_rotX <<
		1 , 0 , 0 ,
		0 , cX , sX ,
		0 , -sX , cX;

	Matrix33 mat_rot = mat_rotZ * mat_rotY * mat_rotX;

	return UnrealTransform(Transform(mat_rot));
}


CameraGeometry::Info makeInfo(const Camera<WorldCoordinateSystem> & cam)
{
	using namespace boost::qvm;

	using Size = Camera<WorldCoordinateSystem>::Size;

	const osg::Vec3 origin = convert_to<osg::Vec3>(cam.getOrigin().get());

	const Size imageSize = cam.getImageSize();

	const ImagePoint topLeft = make_named<ImagePoint>(0, 0);
	const ImagePoint topRight = make_named<ImagePoint>(imageSize(0), 0);
	const ImagePoint bottomLeft = make_named<ImagePoint>(0, imageSize(1));
	const ImagePoint bottomRight = make_named<ImagePoint>(imageSize(0), imageSize(1));

	const ViewRay<WorldCoordinateSystem> dir_topLeft = cam.imageToWorld(topLeft);
	const ViewRay<WorldCoordinateSystem> dir_topRight = cam.imageToWorld(topRight);
	const ViewRay<WorldCoordinateSystem> dir_bottomLeft = cam.imageToWorld(bottomLeft);
	const ViewRay<WorldCoordinateSystem> dir_bottomRight = cam.imageToWorld(bottomRight);

	return CameraGeometry::Info
	{
		origin,
		convert_to<osg::Vec3>(dir_topLeft.direction.get()),
		convert_to<osg::Vec3>(dir_topRight.direction.get()),
		convert_to<osg::Vec3>(dir_bottomLeft.direction.get()),
		convert_to<osg::Vec3>(dir_bottomRight.direction.get())
	};
}


class DisplayablePoints : public OsgDisplayable
{
public:
	DisplayablePoints(const std::vector<osg::Vec3> & points, float size) :
		m_points(points),
		m_size(size)
	{
		// empty
	}

	osg::ref_ptr<osg::Group> getGeometry() const override
	{
		osg::ref_ptr<osg::Group> camGroup = new osg::Group();
		for (const auto & p : m_points)
		{
			osg::ref_ptr<osg::ShapeDrawable> sd = new osg::ShapeDrawable(osg::ref_ptr<osg::Sphere>(new osg::Sphere(p, m_size)));
			sd->setColor(osg::Vec4(1, 0, 0, 1));
			camGroup->addChild(sd);
		}

		return camGroup;
	}

private:
	std::vector<osg::Vec3> m_points;
	float m_size;
};


int main(int argc, char ** argv)
{
	UnrealCoordinateSystem::setDirections({ Vector3(1, 0, 0), Vector3(0, -1, 0), Vector3(0, 0, 1) });
	WorldCoordinateSystem::setDirections({ Vector3(1, 0, 0), Vector3(0, 1, 0), Vector3(0, 0, 1) });

	Camera<WorldCoordinateSystem> camera;
	
	using Vector = WorldCoordinateSystem::Vector;
	using FocalLength = decltype(camera)::FocalLength;
	using Size = decltype(camera)::Size;

	camera.setBaseDirections(
		{
			make_named<WorldVector>(0.f, -1.f, 0.f),
			make_named<WorldVector>(0.f, 0.f, -1.f),
			make_named<WorldVector>(1.f, 0.f, 0.f)
		}
	);

	const Degrees yaw(223.12f);
	const Degrees pitch(316.78f);
	const Degrees roll(0.f);

	const UnrealVector ue_cameraPos(Vector(31.61f, 28.6f, 23.54f));
	const UnrealRotation ue_cameraRotation = UnrealRotation(yaw, pitch, roll);

	std::cout << ue_cameraPos(0) << ", " << ue_cameraPos(1) << ", " << ue_cameraPos(2) << std::endl;

	WorldVector w_cameraPos = UnrealToWorld::sourceToTarget(ue_cameraPos);
	WorldVector w_cameraPos2 = convertTo<WorldCoordinateSystem>(ue_cameraPos);
	WorldRotation w_cameraRotation(ue_cameraRotation);

	camera.extrinsicTransform(w_cameraRotation);
	camera.extrinsicTransform(make_named<WorldTransform>(Translation(w_cameraPos.get())));

	camera.setFocalLength(FocalLength(713.2f));
	camera.setOpticalCenter(make_named<ImagePoint>(400, 300));
	camera.setImageSize(make_named<Size>(800, 600));

	std::ofstream("C:/TEMP/cam.txt") << camera;
	decltype(camera) loaded;
	std::ifstream("C:/TEMP/cam.txt") >> loaded;

	std::cout << std::boolalpha << (camera == camera) << std::endl;
	std::cout << std::boolalpha << (camera == Camera<WorldCoordinateSystem>()) << std::endl;
	std::cout << std::boolalpha << (camera == loaded) << std::endl;

	std::vector<UnrealVector> ue_points =
	{
		make_named<UnrealVector>(0.f, 0.f, 0.f),
		make_named<UnrealVector>(0.f, -15.f, 0.f),
		make_named<UnrealVector>(-10.f, -10.f, 0.f),
		make_named<UnrealVector>(19.f, 13.f, 0.f),
		make_named<UnrealVector>(-8.f, 16.f, 0.f)
	};

	std::vector<WorldVector> w_points;
	std::transform(ue_points.begin(), ue_points.end(), std::back_inserter(w_points),
	               [](const UnrealVector & ue_v)
	               {
		               return UnrealToWorld::sourceToTarget(ue_v);
	               }
	);

	for (const auto & w_p : w_points)
	{
		std::cout << w_p.get() << std::endl << std::endl;
	}

	std::cout << std::endl << std::endl;

	std::vector<decltype(camera)::CameraVector> cam_points;
	std::transform(w_points.begin(), w_points.end(), std::back_inserter(cam_points),
	               [&camera](const WorldVector & w_v)
	               {
		               return camera.worldToCamera(w_v);
	               });

	for (const auto & cam_p : cam_points)
	{
		std::cout << cam_p.get() << std::endl << std::endl;
	}

	std::cout << std::endl << std::endl;

	std::vector<ImagePoint> img_points;
	std::transform(w_points.begin(), w_points.end(), std::back_inserter(img_points),
	               [&camera](const WorldVector & w_v)
	               {
		               return camera.worldToImage(w_v);
	               });

	for (const auto & img_p : img_points)
	{
		std::cout << img_p.get() << std::endl << std::endl;
	}

	std::cout << std::endl << std::endl;

	std::vector<decltype(camera)::CameraVector> cam_points_reconstructed;
	std::transform(img_points.begin(), img_points.end(), std::back_inserter(cam_points_reconstructed),
		[&camera](const ImagePoint & p)
	{
		return camera.imageToCamera(p);
	});

	for (const auto & cam_p : cam_points_reconstructed)
	{
		std::cout << cam_p.get() << std::endl << std::endl;
	}

	std::cout << std::endl << std::endl;

	std::vector<ViewRay<WorldCoordinateSystem>> viewRays;
	std::transform(img_points.begin(), img_points.end(), std::back_inserter(viewRays),
		[&camera](const ImagePoint & p)
	{
		return camera.imageToWorld(p);
	});


	for (int i = 0; i < viewRays.size(); ++i)
	{
		const auto & v = viewRays[i];
		const float dist = (w_points[i].get() - v.origin.get()).norm();
		std::cout << v.origin.get() + dist * v.direction.get() << std::endl << std::endl;
	}

	/*rtcvImageRgba img;
	img.resize(800, 600);
	img.fill(rtcvRgbaValue(20, 20, 20));

	for(const auto & p : img_points)
	{
		img.drawRoi(rtcvRoi(rtcvPoint(p.get()(0), p.get()(1)), 10, 10), rtcvRed);
	}

	img.writeToFile("C:/TEMP/testImg.ppm");*/


	QApplication app(argc, argv);

	Roi3DF area(osg::Vec3(-30, -30, 0), osg::Vec3(30, 30, 20));

	QtOsgWidget widget(nullptr);

	osg::StateSet * rootStateSet = widget.getRoot()->getOrCreateStateSet();
	rootStateSet->setMode(GL_LIGHTING, osg::StateAttribute::ON);
	rootStateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);

	//widget.addNodeToScene(createLightsFromUsableArea(area));

	GeometryManager manager(&widget);

	manager.switchGeometry(std::make_unique<DisplayableAreaLight>(area));
	manager.switchGeometry(std::make_unique<DisplayableGrid>(area, osg::Vec3(1, 1, 1)));
	manager.switchGeometry(std::make_unique<DisplayableCoordinateSystem>(osg::Vec3(0, 0, 0), 0.5f, 5.f));

	std::vector<CameraGeometry::Info> camerasToDisplay = { makeInfo(camera) };
	manager.switchGeometry(std::make_unique<DisplayableCameras>(camerasToDisplay, 1.f, 5.f));

	std::vector<osg::Vec3> pointsToDisplay;
	std::transform(w_points.begin(), w_points.end(), std::back_inserter(pointsToDisplay),
		[](const WorldVector & v)
	{
		return boost::qvm::convert_to<osg::Vec3>(v.get());
	}
	);

	manager.switchGeometry(std::make_unique<DisplayablePoints>(pointsToDisplay, 1.f));

	widget.show();


	return app.exec();
}
