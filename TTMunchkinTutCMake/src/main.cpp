# include "opencv2/opencv.hpp"
# include "opencv2/imgcodecs.hpp"
# include "opencv2/imgproc.hpp"
# include "opencv2/highgui.hpp"
# include "opencv2/aruco.hpp"
# include "opencv2/calib3d.hpp"
#include "GameState.h"
# include "MunchkinCards.h"
#include "CardTypeActions.h"
# include "common.h"
#include "Button.h"

# include <sstream>
# include <iostream>
# include <fstream>
# include <string>
# include <stdexcept>
# include <unordered_map>
# include <functional>

using namespace cv;
using namespace std;
using namespace munch_tut;

const float calbirationSquareDimension = 0.025f; //meters
const float arucoSquareDimension = 0.02f; //meters
const Size chessboardDimensions = Size(6, 9);

vector<MunchkinCard> cards;
std::unordered_map<CardType, CardTypeFunc> functMap;

GameState gamestate;


// button callbacks
void button_exit(const Button& b);
void button_continue(const Button& b);
void button_end_turn(const Button& b);

void update()
{

}

void logic()
{
	//Rect rect(corners[1].x, corners[1].y, corners[0].y - corners[1].y, corners[0].y - corners[1].y);
	if (gamestate.mouseparams.event == MouseEvent::lclick)
	{
		gamestate.mouseparams.event = MouseEvent::none;

		// process button events
		for (const Button& b : gamestate.buttons)
		{
			if (b.poll_click(gamestate.mouseparams.clickP, gamestate.canvas_size))
			{
				// do stuff
			}
		}

		if ((gamestate.mouseparams.poly.size() > 2 && pointPolygonTest(gamestate.mouseparams.poly, gamestate.mouseparams.clickP, false) == 1/*rect.contains(clickP)*/) || (gamestate.should_continue == true))
		{
			MunchkinCard singleCard = cards.at(gamestate.mouseparams.markerId);
			std::cout << "Left button of mouse is clicked _ position (" << gamestate.mouseparams.clickP.x << "," << gamestate.mouseparams.clickP.y << ")" << endl;
			std::cout << "markerId: " << gamestate.mouseparams.markerId << endl;
			std::cout << "Name: " << cards.at(gamestate.mouseparams.markerId).cardName << endl;
			std::cout << "type: " << (int)cards.at(gamestate.mouseparams.markerId).type << endl;
			auto cardtype = cards.at(gamestate.mouseparams.markerId).type;

			if (functMap.find(cardtype) != functMap.end())
			{
				functMap[cardtype](gamestate, singleCard);
			}
		}

	}
	if (gamestate.mouseparams.event == MouseEvent::rclick)
	{
		gamestate.mouseparams.event = MouseEvent::none;

		if ((gamestate.mouseparams.poly.size() > 2 && pointPolygonTest(gamestate.mouseparams.poly, gamestate.mouseparams.clickP, false) == 1))
		{
			MunchkinCard singleCard = cards.at(gamestate.mouseparams.markerId);
			auto cardtype = cards.at(gamestate.mouseparams.markerId).type;
			gamestate.remove_card = true;

			if (functMap.find(cardtype) != functMap.end())
			{
				functMap[cardtype](gamestate, singleCard);
			}
		}
	}

	if (gamestate.end_turn == true)
	{

		gamestate.buttons.at(1).visible = true;
		gamestate.buttons.at(1).callback = button_end_turn;
	}

}

void createArucoMarkers()
{
	Mat outputMarker;

	Ptr<aruco::Dictionary> markerDictionary = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME::DICT_4X4_50);

	for (int i = 0; i < 50; i++)
	{
		aruco::drawMarker(markerDictionary, i, 500, outputMarker, 1);
		ostringstream convert;
		string imageName = "4x4Marker_";
		convert << imageName << i << ".jpg";
		imwrite(convert.str(), outputMarker);
	}
}

void createKnownBoardPositions(Size boardSize, float squareEdgeLength, vector<Point3f>& corners)
{
	for (int i = 0; i < boardSize.height; i++)
	{
		for (int j = 0; j < boardSize.width; j++)
		{
			corners.push_back(Point3f(j * squareEdgeLength, i * squareEdgeLength, 0.0f));
		}
	}
}

void getChessboardCorners( vector<Mat> images, vector<vector<Point2f>>& allFoundCorners, bool showResults = false )
{
	for (vector<Mat>::iterator iter = images.begin(); iter != images.end(); iter++)
	{
		vector<Point2f> pointBuf;
		bool found = findChessboardCorners(*iter, Size(9, 6), pointBuf, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE);

		if (found)
		{
			allFoundCorners.push_back(pointBuf);
		}

		if (showResults)
		{
			cv::drawChessboardCorners(*iter, Size(9, 6), pointBuf, found);
			imshow("Looking for Corners", *iter);
			waitKey(0);
		}
	}
}

bool saveCameraCalibration(string name, Mat cameraMatrix, Mat distanceCoefficients)
{
	ofstream outStream(name);
	if (outStream)
	{
		uint16_t rows = cameraMatrix.rows;
		uint16_t columns = cameraMatrix.cols;

		outStream << rows << endl;
		outStream << columns << endl;

		for (int r = 0; r < rows; r++)
		{
			for (int c = 0; c < columns; c++)
			{
				double value = cameraMatrix.at<double>(r, c);
				outStream << value << endl;
			}
		}

		rows = distanceCoefficients.rows;
		columns = distanceCoefficients.cols;

		outStream << rows << endl;
		outStream << columns << endl;

		for (int r = 0; r < rows; r++)
		{
			for (int c = 0; c < columns; c++)
			{
				double value = distanceCoefficients.at<double>(r, c);
				outStream << value << endl;
			}
		}

		outStream.close();
		return true;
	}
	return false;
}

bool loadCameraCalibration(string name, Mat& cameraMatrix, Mat& distanceCoefficients)
{
	ifstream inStream(name);
	if (inStream)
	{
		uint16_t rows;
		uint16_t columns;

		inStream >> rows;
		inStream >> columns;

		cameraMatrix = Mat(Size(columns, rows), CV_64F);

		for (int r = 0; r < rows; r++)
		{
			for (int c = 0; c < columns; c++)
			{
				double read = 0.0f;
				inStream >> read;
				cameraMatrix.at<double>(r, c) = read;
				std::cout << cameraMatrix.at<double>(r, c) << "\n";
			}
		}

		//DistanceCoefficients
		inStream >> rows;
		inStream >> columns;

		distanceCoefficients = Mat::zeros(rows, columns, CV_64F);

		for (int r = 0; r < rows; r++)
		{
			for (int c = 0; c < columns; c++)
			{
				double read = 0.0f;
				inStream >> read;
				distanceCoefficients.at<double>(r, c) = read;
				std::cout << distanceCoefficients.at<double>(r, c) << "\n";
			}
		}
		inStream.close();
		return true;
		
	}

	return false;
}

void cameraCalibration(vector<Mat> calibrationImages, Size boardSize, float squareEdgeLength, Mat& cameraMatrix, Mat& distanceCoefficients)
{
	vector<vector<Point2f>> checkerboardImageSpacePoints;
	getChessboardCorners(calibrationImages, checkerboardImageSpacePoints, false);

	vector<vector<Point3f>> worldSpaceCornerPoints(1);

	createKnownBoardPositions(boardSize, squareEdgeLength, worldSpaceCornerPoints[0]);
	worldSpaceCornerPoints.resize(checkerboardImageSpacePoints.size(), worldSpaceCornerPoints[0]);

	vector<Mat> rVectors, tVectors;
	distanceCoefficients = Mat::zeros(8, 1, CV_64F);

	calibrateCamera(worldSpaceCornerPoints, checkerboardImageSpacePoints, boardSize, cameraMatrix, distanceCoefficients, rVectors, tVectors);
}

void callBackFunction(int event, int x, int y, int flags, void* userdata)
{
	gamestate.mouseparams.clickP.x = x;
	gamestate.mouseparams.clickP.y = y;
	if (event == EVENT_LBUTTONDOWN)
	{
		//logic();
		gamestate.mouseparams.event = MouseEvent::lclick;
	}
	else if (event == EVENT_RBUTTONDOWN)
	{
		gamestate.mouseparams.event = MouseEvent::rclick;
	}
	else
	{
		gamestate.mouseparams.event = MouseEvent::none;
	}
}

Color chooseCardColor(int markerId)
{
	Color color;
	CardType type;
	try
	{
		type = cards.at(markerId).type;

		switch (type)
		{
		case CardType::curse:
			color[0] = 102;
			color[1] = 0;
			color[2] = 255;
			break;
		case CardType::munchClass:
			color[0] = 0;
			color[1] = 0;
			color[2] = 255;
			break;
		case CardType::joker:
			color[0] = 255;
			color[1] = 255;
			color[2] = 255;
			break;
		case CardType::lvlUp:
			color[0] = 255;
			color[1] = 153;
			color[2] = 51;
			break;
		case CardType::itemBuff:
			color[0] = 0;
			color[1] = 255;
			color[2] = 255;
			break;
		case CardType::monster:
			color[0] = 255;
			color[1] = 0;
			color[2] = 0;
			break;
		case CardType::race:
			color[0] = 255;
			color[1] = 255;
			color[2] = 0;
			break;
		case CardType::item:
			color[0] = 0;
			color[1] = 255;
			color[2] = 0;
			break;
		}

		return color;

	}
	catch (const std::out_of_range& ex)
	{
		std::cerr << "Invalid marker ID" << std::endl;
		color[0] = 255;
		color[1] = 255;
		color[2] = 255;
		return color;
		//throw ex;
	}

}

int startWebcamMonitoring(const Mat& cameraMatrix, const Mat& distanceCoeffiecients, float arucoSquareDimensions)
{
	Mat frame;
	gamestate.mouseparams.tutText = {"Um zu beginnen lege eine ", "Karte nach der anderen vor ", "die Kamera und klicke mit der ", "linken Maustaste auf ", "das farbige Quadrat!"};
	int offset = 25;

	vector<int> markerIds;
	Moments m;
	Color cardColor;
	vector<Point2f> corners;
	vector<Point> poly;
	vector<vector<Point2f>> markerCorners, rejectedCandidates;
	aruco::DetectorParameters parameters;

	Ptr<aruco::Dictionary> markerDictionary = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME::DICT_4X4_50);

	VideoCapture vid(0);

	if (!vid.isOpened())
	{
		return -1;
	}
	vid.set(CV_CAP_PROP_FPS, 60);

	namedWindow("Webcam", CV_WINDOW_NORMAL);

	vector<Vec3d> rotationVectors, translationVectors;

	while (!gamestate.should_exit)
	{
		if (!vid.read(frame))
			break;

		aruco::detectMarkers(frame, markerDictionary, markerCorners, markerIds);
		for( int i = 0; i < gamestate.mouseparams.tutText.size(); i++)
		{
			cv::putText(frame, gamestate.mouseparams.tutText.at(i), Point(50, 50 + i * offset), FONT_HERSHEY_COMPLEX, 1.0, Scalar(0, 0, 0), 4);
			cv::putText(frame, gamestate.mouseparams.tutText.at(i), Point(50, 50+i*offset), FONT_HERSHEY_COMPLEX, 1.0, Scalar(0, 204, 0), 2);
		}

		if (markerIds.size() > 0)
		{
			aruco::drawDetectedMarkers(frame, markerCorners, markerIds);
			aruco::estimatePoseSingleMarkers(markerCorners, arucoSquareDimensions, cameraMatrix, distanceCoeffiecients, rotationVectors, translationVectors);

			for (int i = 0; i < markerIds.size(); i++)
			{
				//aruco::drawAxis(frame, cameraMatrix, distanceCoeffiecients, rotationVectors[i], translationVectors[i], 0.1);
				gamestate.mouseparams.markerId = markerIds.at(i);
				for (int j = markerCorners[i].size() - 4; j < markerCorners[i].size(); j++)
				{
					corners.push_back(markerCorners[i][j]);
				}
				/*for (int j = 0; j < 4; j++)
				{
					params.markerCorner.push_back(markerCorners[i][j]);
					//corners.push_back(markerCorners[i][j]);
				}*/
				cv::approxPolyDP(corners, poly, 0, true);
				gamestate.mouseparams.poly = poly;
				
				try
				{
					cardColor = chooseCardColor(markerIds.at(i));
					cv::fillPoly(frame, poly, cv::Scalar(cardColor[2], cardColor[1], cardColor[0]));
					m = moments(poly);
					Point center(m.m10 / m.m00, m.m01 / m.m00);
					cv::putText(frame, cards.at(markerIds[i]).cardName, center, FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(255,255,255));
				}
				catch (const std::out_of_range& ex)
				{
					std::cerr << "Invalid marker ID thingy" << std::endl;
					continue;
					//throw ex;
				}
				corners.clear();
			}
		}
		cv::setMouseCallback("Webcam", callBackFunction);
		// draw buttons
		for (const Button& b : gamestate.buttons)
		{
			b.draw(frame);
		}
		// show frame
		gamestate.canvas_size = cv::Size(frame.cols, frame.rows);
		imshow("Webcam", frame);
		if (waitKey(5) == 27) break;

		// poll game logic
		logic();
	}

	return 1;
}

void cameraCalibrationProcess(Mat& cameraMatrix, Mat& distanceCoefficients)
{
	Mat frame;
	Mat drawToFrame;

	vector<Mat> savedImages;

	vector<vector<Point2f>> markerCorners, rejectedCandidates;

	VideoCapture vid(0);

	if (!vid.isOpened())
	{
		return;
	}

	int framesPerSecond = 30;

	namedWindow("Webcam", CV_WINDOW_FULLSCREEN);

	while (true)
	{
		if (!vid.read(frame))
			break;

		vector<Vec2f> foundPoints;
		bool found = false;

		found = findChessboardCorners(frame, chessboardDimensions, foundPoints, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE | CV_CALIB_CB_FAST_CHECK);
		frame.copyTo(drawToFrame);
		cv::drawChessboardCorners(drawToFrame, chessboardDimensions, foundPoints, found);
		if (found)
			imshow("Webcam", drawToFrame);
		else
			imshow("Webcam", frame);
		char character = waitKey(1000 / framesPerSecond);

		switch (character)
		{
		case ' ':
			//saving image
			if (found)
			{
				Mat temp;
				frame.copyTo(temp);
				savedImages.push_back(temp);
			}
			break;
		case 13:
			//start calibration
			if (savedImages.size() > 15)
			{
				cameraCalibration(savedImages, chessboardDimensions, calbirationSquareDimension, cameraMatrix, distanceCoefficients);
				saveCameraCalibration("CameraCalibration", cameraMatrix, distanceCoefficients);
			}
			break;
		case 27:
			//exit Program
			return;
			break;

		}
	}

}


int main(int argv, char** argc)
{
	Mat cameraMatrix = Mat::eye(3, 3, CV_64F);

	Mat distanceCoefficients;
	//try
	//{
		//createArucoMarkers();
	if (argv == 2)
	{
		if (std::string(argc[1]) == "-c")
		{
			cameraCalibrationProcess(cameraMatrix, distanceCoefficients);
			return 0;
		}
		else if (std::string(argc[1]) == "-g")
		{
			createArucoMarkers();
			return 0;
		}
		else
		{
			std::cerr << "Invalid command line option" << std::endl;
			return 1;
		}
	}
	else if(argv == 1)
	{

		cards = MunchkinCard::cardsConstr();
		gamestate.player01.sex = "male";
		gamestate.player01.lvl = 1;
		gamestate.player01.strength = 0;
		gamestate.player01.hands = 2;
		gamestate.player01.carriesLargeItems = false;
		gamestate.player01.hasArmor = false;
		gamestate.player01.hasHat = false;
		gamestate.player01.hasShoes = false;
		gamestate.player01.runStrength = 4;
		gamestate.player01.availableClasses = 1;
		gamestate.player01.availableRaces = 1;
		gamestate.should_exit = false;
		gamestate.should_continue = false;
		gamestate.run_away = false;
		gamestate.end_turn = false;
		gamestate.remove_card = false;
		gamestate.mouseparams.event = MouseEvent::none;

		// setup buttons
		// exit button
		gamestate.buttons.push_back(
			Button{
				0,
				cv::Rect(5, 5, 25, 25),
				cv::Scalar(255, 0, 0),
				true,
				ButtonOrigin::topright,
				button_exit
			}
		);

		// continue button
		gamestate.buttons.push_back(
			Button{
				1,
				cv::Rect(5, 5, 50, 50),
				cv::Scalar(0, 255, 0),
				false,
				ButtonOrigin::bottomright,
				button_continue
			}
		);

		functMap[CardType::curse] = cardtypeaction::curse;
		functMap[CardType::item] = cardtypeaction::item;
		functMap[CardType::monster] = cardtypeaction::monster;
		functMap[CardType::munchClass] = cardtypeaction::munchClass;
		functMap[CardType::race] = cardtypeaction::race;
		functMap[CardType::itemBuff] = cardtypeaction::itemBuff;
		functMap[CardType::joker] = cardtypeaction::joker;
		functMap[CardType::lvlUp] = cardtypeaction::lvlUp;

		//cameraCalibrationProcess(cameraMatrix, distanceCoefficients);
		loadCameraCalibration("CameraCalibration", cameraMatrix, distanceCoefficients);
		startWebcamMonitoring(cameraMatrix, distanceCoefficients, 0.005f);

		return 0;
	}
	else
	{
		std::cerr << "Invalid command line arguments" << std::endl;
		return 1;
	}
	//}
	//catch(std::exception& ex)
	//{
	//	std::cerr << ex.what() << std::endl;
	//	return 0;
	//}
	//catch(...)
	//{
	//	std::cerr << "unknown error" << std::endl;
	//	return 0;
	//}	
}

void button_exit(const Button & b)
{
	gamestate.should_exit = true;
	//std::cout << "Button " << b.id << std::endl;
}

void button_continue(const Button & b)
{
	gamestate.should_continue = true;
	gamestate.buttons.at(1).visible = false;

}

void button_end_turn(const Button & b)
{
	gamestate.end_turn = false;
	gamestate.buttons.at(1).visible = false;
	gamestate.mouseparams.tutText = { "Nachdem du deine Schaetze ", "durchgesehen und ggf ausgeruestet ", "hast kommt nun die milde Gabe!", "gebe so viele Karten an", "die Person mit niedrigstem level", ", oder lege sie ab wenn du", "das niedrigste level hast, ", "bis du nurnoch 5 Karten hast!" };
	gamestate.buttons.at(1).visible = false;
	gamestate.buttons.at(1).callback = button_continue;

}