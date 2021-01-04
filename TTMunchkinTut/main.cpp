# include "opencv2/opencv.hpp"
# include "opencv2/imgcodecs.hpp"
# include "opencv2/imgproc.hpp"
# include "opencv2/highgui.hpp"
# include "opencv2/aruco.hpp"
# include "opencv2/calib3d.hpp"
# include "MunchkinCards.h"
# include "common.h"

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
vector<String> actions;
std::unordered_map<std::string, std::function<bool()>> functMap;

struct MouseParams
{
	vector<Point> poly;
	int markerId;
	vector<Point2f> markerCorner;
	vector<string> tutText;
};

struct PlayerStats
{
	string sex;
	vector<string> bonis;
	vector<string> curses;
	vector<string> itemEffects;
	vector<string> munchClasses;
	vector<string> munchRaces;
	int lvl;
	int strength;
	int hands;
	int runStrength;
	bool carriesLargeItems;
	bool hasArmor;
	bool hasHat;
	bool hasShoes;
};

PlayerStats player01;

bool removeArmor()
{
	std::cout << "you loose your armor" << endl;
}

bool runAway()
{
	int diceRoll = rand() % 6 + 1;
	if (diceRoll > player01.runStrength)
		return true;
	else
		return false;
}

void update()
{

}

void logic()
{
	//Rect rect(corners[1].x, corners[1].y, corners[0].y - corners[1].y, corners[0].y - corners[1].y);
	if (pointPolygonTest(mp->poly, clickP, false) == 1/*rect.contains(clickP)*/)
	{
		singleCard = cards.at(mp->markerId);
		std::cout << "Left button of mouse is clicked _ position (" << x << "," << y << ")" << endl;
		std::cout << "markerId: " << mp->markerId << endl;
		std::cout << "Name: " << cards.at(mp->markerId).cardName << endl;
		std::cout << "type: " << (int)cards.at(mp->markerId).type << endl;

		switch (singleCard.parentCardType)
		{
		case(ParentCardType::door):
			switch (singleCard.type)
			{
			case(CardType::curse):
				player01.curses.push_back(singleCard.effect);
				std::cout << "TODO: implement function call for effects: " << singleCard.effect << endl;
				break;
			case(CardType::joker):
				std::cout << "TODO: implement function call for effects: " << singleCard.effect << endl;
				break;
			case(CardType::monster):
				monsStrength = singleCard.monsStrength;
				playerStrength = player01.lvl + player01.strength;
				// TODO: monsStrength +- monster effects functionCall/code in string?
				if (monsStrength > playerStrength /* && monsStrength > playerStrength + bonis*/)
				{
					mp->tutText = { "You are too weak!", "Press 'space' to try to run away!" };
					//do
					//{
					//
					//} while (waitKey(30) != 32);
					if (runAway())
						mp->tutText = { "You managed to run away!", "Continue your Turn!" };
					else
						mp->tutText = { "Failed to run away!", "Now face the evil things", singleCard.badThings };
					std::cout << "Failed to run away! Now face the evil things " << singleCard.cardName << " will do to you!!!" << singleCard.badThings << endl;
				}
				else
				{
					std::cout << "You killed " << singleCard.cardName << "! Congratulation!" << endl;
					std::cout << "Your level was raised by : " << singleCard.lvlUp << endl;
					player01.lvl += singleCard.lvlUp;
					std::cout << "Your new level is: " << player01.lvl << endl;
					std::cout << "You can collect " << singleCard.treasures << " treasures! Good luck in your next turn!" << endl;

				}
				break;
			case(CardType::munchClass):
				if (player01.munchClasses.empty() or find(player01.bonis.begin(), player01.bonis.end(), "you can use multiple Classes") != player01.bonis.end())
				{
					player01.munchClasses.push_back(singleCard.cardName);
					for (int i = 0; i < singleCard.bonis.size(); i++)
						player01.bonis.push_back(singleCard.bonis.at(i));
				}
				else
					std::cout << "You have too many Classes take Card to Hand" << endl;
				break;
			case(CardType::race):
				if (player01.munchRaces.empty() or find(player01.bonis.begin(), player01.bonis.end(), "you can use multiple Races") != player01.bonis.end())
				{
					player01.munchRaces.push_back(singleCard.cardName);
					for (int i = 0; i < singleCard.bonis.size(); i++)
						player01.bonis.push_back(singleCard.bonis.at(i));
				}
				break;
			default:
				break;
			}
			break;
		case(ParentCardType::treasure):
			switch (singleCard.type)
			{
			case(CardType::item):
				switch (singleCard.itemType)
				{
					//Schummeln?
				case(ItemType::joker):
					player01.itemEffects.push_back(singleCard.itemEffect);
					break;
				case(ItemType::boni):
					for (int i = 0; i < singleCard.bonis.size(); i++)
						player01.bonis.push_back(singleCard.bonis[i]);
					break;
				case(ItemType::hat):
					// TODO: Check for itemNeeds on singleCard.itemNeeds
					if (player01.hasHat)
					{
						if (!singleCard.bonis.empty())
						{
							for (int i = 0; i < singleCard.bonis.size(); i++)
								player01.bonis.push_back(singleCard.bonis[i]);
						}
						player01.strength += singleCard.strengthBoni;
						player01.hasHat = true;
					}
					else
						cout << "You can only carry one hat! Except a cards grands you more hats!" << endl;
					break;
				case(ItemType::armor):
					// TODO: Check for itemNeeds on singleCard.itemNeeds
					if (player01.hasArmor)
					{
						if (!singleCard.bonis.empty())
						{
							for (int i = 0; i < singleCard.bonis.size(); i++)
								player01.bonis.push_back(singleCard.bonis[i]);
						}
						player01.strength += singleCard.strengthBoni;
						player01.hasArmor = true;
					}
					else
						cout << "You can only carry one armor! Except a cards grands you more armors!" << endl;
					break;
				case(ItemType::shoes):
					// TODO: Check for itemNeeds on singleCard.itemNeeds
					if (player01.hasShoes)
					{
						if (!singleCard.bonis.empty())
						{
							for (int i = 0; i < singleCard.bonis.size(); i++)
								player01.bonis.push_back(singleCard.bonis[i]);
						}
						player01.strength += singleCard.strengthBoni;
						player01.hasShoes = true;
					}
					else
						cout << "You can only carry one shoes! Except a cards grands you more shoes!" << endl;
					break;
				case(ItemType::clothing):
					// TODO: Check for itemNeeds on singleCard.itemNeeds
					if (!singleCard.bonis.empty())
					{
						for (int i = 0; i < singleCard.bonis.size(); i++)
							player01.bonis.push_back(singleCard.bonis[i]);
					}
					player01.strength += singleCard.strengthBoni;
					break;
				case(ItemType::weapon):
					//TODO: Check for itemNeeds on singleCard.itemNeeds
					if (player01.hands >= singleCard.handsNeeded)
					{
						if (player01.carriesLargeItems == false || singleCard.itemLarge == false)
						{
							if (!singleCard.bonis.empty())
							{
								for (int i = 0; i < singleCard.bonis.size(); i++)
									player01.bonis.push_back(singleCard.bonis[i]);
							}
							player01.strength += singleCard.strengthBoni;
							player01.hands -= singleCard.handsNeeded;
							player01.carriesLargeItems = singleCard.itemLarge ? true : false;

						}
						else
							cout << "You can only carry one large item! Except a cards grands you more large items!" << endl;
					}
					else
						cout << "You need more hands to carry this item!" << endl;
					break;
				default:
					break;
				}
				break;
			case(CardType::itemBuff):
				//TODO: verweis auf ausrüstung?
				if (!singleCard.bonis.empty())
				{
					for (int i = 0; i < singleCard.bonis.size(); i++)
						player01.bonis.push_back(singleCard.bonis[i]);
				}
				player01.strength += singleCard.strengthBoni;
				break;
			case(CardType::lvlUp):
				if (!singleCard.itemNeeds.empty())
					std::cout << "You Need to do this: " << singleCard.itemNeeds << endl;

				if (player01.lvl < 9)
					player01.lvl += singleCard.lvlUp;
				else
					std::cout << "You are not able to use this with lvl 9" << endl;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}

		std::cout << "PLAYERSTATS: " << endl;
		std::cout << "Player level: " << player01.lvl << endl;
		std::cout << "Player strength: " << player01.strength << endl;
		std::cout << "Player available hands: " << player01.hands << endl;
		std::cout << "Player carries large object: " << player01.carriesLargeItems << endl;
		for (int i = 0; i < player01.bonis.size(); i++)
			std::cout << "Player boni: " << i << " " << player01.bonis.at(i) << endl;

		for (int i = 0; i < player01.curses.size(); i++)
			std::cout << "Player curse: " << i << " " << player01.curses.at(i) << endl;

		for (int i = 0; i < player01.itemEffects.size(); i++)
			std::cout << "Player itemEffects: " << i << " " << player01.itemEffects.at(i) << endl;

		if (player01.lvl >= 10)
			std::cout << "You reached level 10! Congratulations you Won your first Round of Munchkin!!!" << endl;
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
	MouseParams* mp = (MouseParams*)userdata;
	Point clickP(x,y);
	MunchkinCard singleCard;
	int monsStrength, playerStrength = 0;
	if (event == EVENT_LBUTTONDOWN)
	{
		actions.push_back("leftClick");
	}
	else if (event == EVENT_RBUTTONDOWN)
	{
		std::cout << "Right button of mouse is clicked _ position (" << x << "," << y << ")" << endl;
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
	MouseParams params;
	params.tutText = {"Um zu beginnen lege eine Karte", "nach der anderen vor die Kamera", "und klicke mit der linken Maustaste", "auf das farbige Quadrat!"};
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

	while (true)
	{
		if (!vid.read(frame))
			break;

		aruco::detectMarkers(frame, markerDictionary, markerCorners, markerIds);
		for( int i = 0; i < params.tutText.size(); i++)
		{
			cv::putText(frame, params.tutText.at(i), Point(50, 50+i*offset), FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(0, 204, 0));
		}

		if (markerIds.size() > 0)
		{
			aruco::drawDetectedMarkers(frame, markerCorners, markerIds);
			aruco::estimatePoseSingleMarkers(markerCorners, arucoSquareDimensions, cameraMatrix, distanceCoeffiecients, rotationVectors, translationVectors);

			for (int i = 0; i < markerIds.size(); i++)
			{
				//aruco::drawAxis(frame, cameraMatrix, distanceCoeffiecients, rotationVectors[i], translationVectors[i], 0.1);
				params.markerId = markerIds.at(i);
				for (int j = markerCorners[i].size() - 4; j < markerCorners[i].size(); j++)
				{
					corners.push_back(markerCorners[i][j]);
				}
				/*for (int j = 0; j < 4; j++)
				{
					params.markerCorner.push_back(markerCorners[i][j]);
					//corners.push_back(markerCorners[i][j]);
				}*/
				approxPolyDP(corners, poly, 0, true);
				params.poly = poly;
				
				cardColor = chooseCardColor(markerIds.at(i));
				try
				{
					cv::fillPoly(frame, poly, cv::Scalar(cardColor[2], cardColor[1], cardColor[0]));
					m = moments(poly);
					Point center(m.m10 / m.m00, m.m01 / m.m00);
					cv::putText(frame, cards.at(markerIds[i]).cardName, center, FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(255,255,255));
				}
				catch (const std::out_of_range& ex)
				{
					std::cerr << "Invalid marker ID thingy" << std::endl;
					return 1;
					//throw ex;
				}
				corners.clear();
				cv::setMouseCallback("Webcam", callBackFunction, (void*) &params);
			}
		}
		imshow("Webcam", frame);
		if (waitKey(30) == 27) break;
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

	int framesPerSecond = 20;

	namedWindow("Webcam", CV_WINDOW_FULLSCREEN);

	while (true)
	{
		if (!vid.read(frame))
			break;

		vector<Vec2f> foundPoints;
		bool found = false;

		found = findChessboardCorners(frame, chessboardDimensions, foundPoints, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE /*| CV_CALIB_CB_FAST_CHECK*/);
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
	//try
	//{
		//createArucoMarkers();
	
		Mat cameraMatrix = Mat::eye(3, 3, CV_64F);

		Mat distanceCoefficients;


		functMap["removeArmor"] = removeArmor;

		cards = MunchkinCard::cardsConstr();
		player01.sex = "male";
		player01.lvl = 1;
		player01.strength = 0;
		player01.hands = 2;
		player01.carriesLargeItems = false;
		player01.hasArmor = false;
		player01.hasHat = false;
		player01.hasShoes = false;
		player01.runStrength = 4;
	
		//cameraCalibrationProcess(cameraMatrix, distanceCoefficients);
		loadCameraCalibration("CameraCalibration", cameraMatrix, distanceCoefficients);
		startWebcamMonitoring(cameraMatrix, distanceCoefficients, 0.005f);

		return 0;
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