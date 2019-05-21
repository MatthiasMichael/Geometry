#include <iostream>

//#include "FunctionTimer.h"
//#include "rtcvImage.h"

#include "fitProjectiveTransform.h"
#include "ImageTransform.h"

#include "commonTypes.h"

void printTiming(const std::string & name, double time, int numTries)
{
	std::cout << name << ": " << time * 1000 << " ms total; " << (time / static_cast<double>(numTries)) * 1000. << " ms mean;" << std::endl;
}

int main()
{
	const int imgSize = 1920;
	//rtcvImage8U img(imgSize, imgSize);
	//img.fill(255);
	for (int x = 0; x < imgSize; ++x)
	{
		for (int y = 0; y < imgSize; ++y)
		{
			//img.setValue(x, y, static_cast<unsigned char>((double(x) / double(imgSize)) * 255.));
			/*if (x % 10 == 0 || y % 10 == 0)
			{
				img.setValue(x, y, 0);
			}*/
		}
	}

	//img.writeToFile("C:/TEMP/trans_A.pgm");

	std::vector<ImagePoint> srcPoints =
	{
		make_named<ImagePoint>(0, 0),
		make_named<ImagePoint>(imgSize, 0),
		make_named<ImagePoint>(imgSize, imgSize),
		make_named<ImagePoint>(0, imgSize)
	};

	std::vector<ImagePoint> dstPoints =
	{
		make_named<ImagePoint>(100, 100),
		make_named<ImagePoint>(static_cast<int>(100 + imgSize * 0.5), static_cast<int>(100 - imgSize * 0.2)),
		make_named<ImagePoint>(static_cast<int>(100 + imgSize), static_cast<int>(100 + imgSize * 1.5)),
		make_named<ImagePoint>(static_cast<int>(100 + imgSize * 0.2), static_cast<int>(100 + imgSize))
	};

	double t_fit1, t_fit2, t_trans_1_S, t_trans_1_F, t_trans_2_S, t_trans_2_F;
	int numTries = 100;

	std::cout << "Time Fit 1...";

	{
		//BlockTimer timer(t_fit1);
		for (int i = 0; i < numTries; ++i)
		{
			fitProjectiveTransform<ScalarType>(srcPoints, dstPoints);
		}
	}

	std::cout << " done!\nTime Fit 1...";

	{
		//BlockTimer timer(t_fit2);
		for (int i = 0; i < numTries; ++i)
		{
			fitProjectiveTransform2<ScalarType>(srcPoints, dstPoints);
		}
	}

	std::cout << " done!" << std::endl;

	//ImageTransform<ScalarType, GeometricTypes<ScalarType, 2>::Transform::Projective> transform1(fitProjectiveTransform<ScalarType>(srcPoints, dstPoints));
	//ImageTransform<ScalarType, GeometricTypes<ScalarType, 2>::Transform::Projective> transform2(fitProjectiveTransform2<ScalarType>(srcPoints, dstPoints));
	//rtcvImage8U imgTransformed;

	std::cout << "Time Trans 1 S...";
	{
		//BlockTimer timer(t_trans_1_S);
		for (int i = 0; i < numTries; ++i)
		{
			//imgTransformed = transform1.transform(img);
		}
	}
	std::cout << " done!" << std::endl;
	//imgTransformed.writeToFile("C:/TEMP/trans_1_S.pgm");
	std::cout << "Time Trans 2 S...";
	{
		//BlockTimer timer(t_trans_2_S);
		for (int i = 0; i < numTries; ++i)
		{
			//imgTransformed = transform2.transform(img);
		}
	}
	std::cout << " done!" << std::endl;
	//imgTransformed.writeToFile("C:/TEMP/trans_2_S.pgm");
	std::cout << "Time Trans 1 F...";
	{
		//BlockTimer timer(t_trans_1_F);
		for (int i = 0; i < numTries; ++i)
		{
			//imgTransformed = transform1.transformFast(img);
		}
	}
	std::cout << " done!" << std::endl;
	//imgTransformed.writeToFile("C:/TEMP/trans_1_F.pgm");
	std::cout << "Time Trans 2 F...";
	{
		//BlockTimer timer(t_trans_2_F);
		for (int i = 0; i < numTries; ++i)
		{
			//imgTransformed = transform2.transformFast(img);
		}
	}
	std::cout << " done!" << std::endl << std::endl;
	//imgTransformed.writeToFile("C:/TEMP/trans_2_F.pgm");

	printTiming("Fit 1", t_fit1, numTries);
	printTiming("Fit 2", t_fit2, numTries);
	printTiming("Trans 1 S", t_trans_1_S, numTries);
	printTiming("Trans 2 S", t_trans_2_S, numTries);
	printTiming("Trans 1 F", t_trans_1_F, numTries);
	printTiming("Trans 2 F", t_trans_2_F, numTries);

	

	return EXIT_SUCCESS;
}