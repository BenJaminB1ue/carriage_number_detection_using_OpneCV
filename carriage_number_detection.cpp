//Author:BenJamin Blue
//2019.7 in NJUST
//Environment:Win10+VS2017+OpenCV3.4.6

#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include<cv.h>

using namespace cv;
using namespace std;

vector<Mat> result4;
Mat binary2_clone;
int row_min, col_min, row_max, col_max; //points of the rectangle

//separate characters with each cut of pieces using projection
void project_and_cut(Mat binary, Mat binary2)
{
	int startindex = 0;
	int endindex = 0;
	bool inblock = false; //go through the area of characters?
	bool meet_peak = false; //go through the area of the highest peak?
	int pixcel_distance = 5; // can't be closer
	int* pointcount = new int[binary.rows]; //count the number of feature points
	int max_pointcount = 0;
	vector<Mat> result;
	vector<Mat> result2;
	vector<Mat> result3;
	Mat imgGray;
	Mat imgT;
	Mat imgOP;
	Mat imgCL;
	Mat imgOP2;
	Mat imgCL2;
	Mat roi3;
	Mat painty(binary.size(), CV_8UC1, Scalar(255)); //initialization as white

	binary2_clone = binary2.clone();
	memset(pointcount, 0, binary.rows * 4);

	//count the amount of black pixels projected on y axis
	for (int i = 0; i < binary.rows; i++)
	{
		for (int j = 0; j < binary.cols; j++)
		{
			if (binary.at<uchar>(i, j) == 0)
			{
				pointcount[i]++;
				if (max_pointcount < pointcount[i])
					max_pointcount = pointcount[i];
			}
		}
	}

	//to make the histogram
	for (int i = 0; i < binary.rows; i++)
	{
		for (int j = 0; j < pointcount[i]; j++)
		{
			painty.at<uchar>(i, j) = 0;
		}
	}

	//imshow("painty", painty);

	//cut to horizontal bars
	for (int i = 0; i < painty.rows; i++) //i is rows of the image
	{

		if (pointcount[i] == max_pointcount)//judge if it meets the highest peak through all the rows
			meet_peak = true;

		if (!inblock&&pointcount[i] != 0) //go through the area with characters
		{
			inblock = true;
			startindex = i;
		}

		if (inblock&&pointcount[i] == 0) //go through the area without characters
		{
			endindex = i;
			inblock = false;

			if (meet_peak == true)//meet the highest peek
			{
				Mat roi = binary.rowRange(startindex, endindex); //cut the binary bar
				result.push_back(roi);
				meet_peak = false;
				//imshow("slice1", result[0]);

				row_min = startindex - pixcel_distance;
				row_max = endindex + pixcel_distance;

				Mat roi2 = binary2.rowRange(startindex, endindex); //cut the original bar
				result2.push_back(roi2);
				//imshow("slice2", result2[0]);
			}
		}
	}

	//imshow("slice" , result[0]);
	delete pointcount;

	Mat paintx(result[0].size(), CV_8UC1, Scalar(255)); //initialization as white
	int* blackcout = new int[result[0].cols];//feature points
	memset(blackcout, 0, result[0].cols * 4);
	//projection of x axis
	for (int i = 0; i < result[0].rows; i++)
	{
		for (int j = 0; j < result[0].cols; j++)
		{
			if (result[0].at<uchar>(i, j) == 0)
			{
				blackcout[j]++; //count the black pixel
			}
		}
	}
	for (int i = 0; i < result[0].cols; i++)
	{
		for (int j = 0; j < blackcout[i]; j++)
		{
			paintx.at<uchar>(result[0].rows - 1 - j, i) = 0; //show
		}
	}

	int startindex2 = 0;
	int endindex2 = 0;

	//cout << "cols is" << result[0].cols << "\n";

	bool begain_start = false;
	bool begain_end = false;

	//starting and ending y coordinates
	for (int i = 0; i < result[0].cols - 1; i++)
	{
		if ((begain_start == false) && (blackcout[i + 1] - blackcout[i] != 0))
		{
			startindex2 = i;
			begain_start = true;
		}
		//cout <<i<< "blackout is "<<" "<<blackcout[i]<<"\n";

		if ((begain_end == false) && (blackcout[result[0].cols - i - 3] - blackcout[result[0].cols - i - 2] != 0))
		{
			endindex2 = result[0].cols - i;
			begain_end = true;
		}
	}

	//imshow("paintx", paintx);

	roi3 = result2[0].colRange(startindex2, endindex2); //cut from the original bar
	result3.push_back(roi3);//original rectangle
	//imshow("slice3", result3[0]);
	col_min = startindex2 - pixcel_distance;
	col_max = endindex2 + pixcel_distance;
	rectangle(binary2_clone, Point(col_min - pixcel_distance, row_min - pixcel_distance), Point(col_max + pixcel_distance, row_max + pixcel_distance), Scalar(0, 255, 0), 3);//show the green rectangle in original image
	//imshow("carriage number detection", binary2_clone);

	delete blackcout;

	//another binary&morphological processing for the original rectangle just once
	cvtColor(result3[0], imgGray, CV_BGR2GRAY);
	threshold(imgGray, imgT, 170, 255, CV_THRESH_BINARY);

	//closing operation
	Mat elementCL = getStructuringElement(MORPH_RECT, Size(4, 3));
	morphologyEx(imgT, imgCL, MORPH_CLOSE, elementCL);
	//opening operation
	Mat elementOP = getStructuringElement(MORPH_RECT, Size(4, 4));
	morphologyEx(imgCL, imgOP, MORPH_OPEN, elementOP);

	Mat painty2(imgOP.size(), CV_8UC1, Scalar(255));

	//horizontal projection for each characters
	int* pointcount2 = new int[imgOP.cols]; //feture points for cloumns
	memset(pointcount2, 0, imgOP.cols * 4);

	//count the amount of black pixcels
	for (int i = 0; i < imgOP.rows; i++)
	{
		for (int j = 0; j < imgOP.cols; j++)
		{
			if (imgOP.at<uchar>(i, j) == 0)
			{
				pointcount2[j]++;
			}
		}
	}

	//make the projection image
	for (int i = 0; i < imgOP.cols; i++)
	{
		for (int j = 0; j < pointcount2[i]; j++)
		{
			painty2.at<uchar>(imgOP.rows - 1 - j, i) = 0;
		}
	}

	//imshow("painty2", painty2);

	int startindex3 = 0;
	int endindex3 = 0;
	bool inblock2 = false; //go through the characters area?


	//cut each characters
	for (int i = 0; i < painty2.cols; i++)
	{
		if (!inblock2&&pointcount2[i] != 0) //go through the area with characters
		{
			inblock2 = true;
			startindex3 = i;
		}
		if (inblock2&&pointcount2[i] == 0) //go through the area without characters
		{
			endindex3 = i;
			inblock2 = false;
			Mat roi = imgOP.colRange(startindex3, endindex3 + 1);
			result4.push_back(roi);
		}
	}

	//show each characters cut
	for (int i = 0; i < result4.size(); i++)
	{
		Mat tmp = result4[i];
		if (i <= 1)
			resize(tmp, tmp, Size(20, 25));
		else
			resize(tmp, tmp, Size(16, 25));
		imshow("character " + to_string(i), tmp);
	}
}


//match each of the piece with templates and find the optimal one, then print it on the original image
void match_and_print()
{
	Mat temp[10];
	string str;
	string th[10] = { "st","nd","rd","th","th","th","th","th","th","th" };

	//read 4 word templates from the file "character_template" ,put that file to the program path ,please
	Mat temp_R = imread("character_template/words/R.png");
	cvtColor(temp_R, temp_R, CV_BGR2GRAY);
	threshold(temp_R, temp_R, 170, 255, CV_THRESH_BINARY);

	Mat temp_W = imread("character_template/words/W.png");
	cvtColor(temp_W, temp_W, CV_BGR2GRAY);
	threshold(temp_W, temp_W, 170, 255, CV_THRESH_BINARY);

	Mat temp_Y = imread("character_template/words/Y.png");
	cvtColor(temp_Y, temp_Y, CV_BGR2GRAY);
	threshold(temp_Y, temp_Y, 170, 255, CV_THRESH_BINARY);

	Mat temp_Z = imread("character_template/words/Z.png");
	cvtColor(temp_Z, temp_Z, CV_BGR2GRAY);
	threshold(temp_Z, temp_Z, 170, 255, CV_THRESH_BINARY);
	//imshow("temp_z", temp_Z);

	//read 10 number templates
	for (int i = 0; i < 10; i++)
	{
		temp[i] = imread("character_template/numbers/" + to_string(i) + ".png");
		cvtColor(temp[i], temp[i], CV_BGR2GRAY);
		threshold(temp[i], temp[i], 170, 255, CV_THRESH_BINARY);
		//imshow("temp_" + to_string(i), temp[i]);
	}

	char match_result[8];
	printf("---------- TEMPLATE MATCHING PROCESS ----------\n");
	printf("Idea: we regard the template with the least distance as the optimal template\n");
	printf("\n");

	for (int i = 0; i < result4.size(); i++) //for each cut
	{
		cout << "----------The " << i + 1 << th[i] << " template matching----------\n";

		if (i <= 1)//the first two characters must be letters
		{
			resize(result4[i], result4[i], Size(20, 25));
			double odis[4];
			double odis_min = 10000;
			int odis_min_index_1 = 0;

			odis[0] = norm(result4[i], temp_R, CV_L2);
			cout << "the distance between the " << i + 1<< th[i] << " charater and template R is " << odis[0]<<"\n";

			odis[1] = norm(result4[i], temp_W, CV_L2);
			cout << "the distance between the " << i + 1 << th[i] << " charater and template W is " << odis[1] << "\n";

			odis[2] = norm(result4[i], temp_Y, CV_L2);
			cout << "the distance between the " << i + 1 << th[i] << " charater and template Y is " << odis[2] << "\n";

			odis[3] = norm(result4[i], temp_Z, CV_L2);
			cout << "the distance between the " << i + 1 << th[i] << " charater and template Z is " << odis[3] << "\n";

			for (int j = 0; j < 4; j++) //judge the min index
			{
				//	printf("the o distance is %f\n", odis[j]);
				if (odis_min > odis[j])
				{
					odis_min = odis[j];
					odis_min_index_1 = j;
				}
			}
			odis_min = 10000;

			switch (odis_min_index_1)
			{
			case 0:
				match_result[i] = 'R';
				break;
			case 1:
				match_result[i] = 'W';
				break;
			case 2:
				match_result[i] = 'Y';
				break;
			case 3:
				match_result[i] = 'Z';
				break;
			default:
				;
			}
			cout << "The " << i + 1 << th[i] << " character is " << match_result[i] << "\n";
		}
		else //the rest are numbers
		{
			double odis[10];
			double odis_min = 10000;
			int odis_min_index_2 = 0;

			resize(result4[i], result4[i], Size(16, 25));

			for (int j = 0; j < 10; j++) //match of templates
			{
				odis[j] = norm(result4[i], temp[j], CV_L2);
				//printf("the o distance is %f\n", odis[j]);
			}
			for (int j = 0; j < 10; j++) //judge the max index
			{
				cout << "the distance between the " << i + 1 << th[i] << " charater and the template "<<j<<" is " << odis[j] << "\n";
				//	printf("the o distance is %f\n", odis[j]);
				if (odis_min > odis[j])
				{
					odis_min = odis[j];
					odis_min_index_2 = j;
				}
			}
			odis_min = 10000;
			//cout << "odis_min_index is " <<odis_min_index_2<<"\n";

			switch (odis_min_index_2)
			{
			case 0:
				match_result[i] = '0';
				break;
			case 1:
				match_result[i] = '1';
				break;
			case 2:
				match_result[i] = '2';
				break;
			case 3:
				match_result[i] = '3';
				break;
			case 4:
				match_result[i] = '4';
				break;
			case 5:
				match_result[i] = '5';
				break;
			case 6:
				match_result[i] = '6';
				break;
			case 7:
				match_result[i] = '7';
				break;
			case 8:
				match_result[i] = '8';
				break;
			case 9:
				match_result[i] = '9';
				break;

			default:
				;
			}
			cout <<"The "<<i + 1<< th[i] << " character is " << match_result[i] << "\n";

			//imshow("test" + to_string(i), result4[i]);
		}
	}

	for (int i = 0; i < 8; i++)
	{
		if (i == 2)
		{
			str += ' ';
			str += match_result[i];//str += ch;
		}
		else
			str += match_result[i];
	}

	cout << "-------------------------------\n";
	cout << "Matching result is " << str << "\n";
	putText(binary2_clone, str, Point(col_min, row_min - 10), CV_FONT_HERSHEY_COMPLEX, 0.7, Scalar(0, 255, 0));
	imshow("carriage number recognization", binary2_clone);

	////show the value of each binary pixel
	//Picture_info(result4[5]);
	//Picture_info(temp_R);
    //delete pointcount2;
}


int main()
{
	Mat imgGray;
	Mat imgT;
	Mat imgOP;
	Mat imgCL;
	Mat imgOP2;
	Mat imgCL2;
	Mat elementCL;
	Mat elementOP;
	Mat elementCL2;
	Mat elementOP2;
	Mat reverse_imgOP2;
	Mat dstImage;
	Mat edge;
	Mat img = imread("1 Carriage number/carriage3.jpg");//carriage3.jpg is greatly suggested !

	cvtColor(img, imgGray, CV_BGR2GRAY);//grayscale processing
	//imshow("Gray image", imgGray);

	threshold(imgGray, imgT, 170, 255, CV_THRESH_BINARY);//threshold processing
	//imshow("Binary image", imgT);//

	//the 1st morphological processing
	//Closing operation
	elementCL = getStructuringElement(MORPH_RECT, Size(4, 3));//kernel
	morphologyEx(imgT, imgCL, MORPH_CLOSE, elementCL);
	//imshow("Closing operation", imgCL);

	//Opening operation
	elementOP = getStructuringElement(MORPH_RECT, Size(4, 4));//kernel
	morphologyEx(imgCL, imgOP, MORPH_OPEN, elementOP);
	//imshow("Opening operation", imgOP);

	//canny operator
	dstImage.create(imgOP.size(), imgOP.type());
	Canny(imgOP, imgOP, 3, 9, 3);
	dstImage = Scalar::all(0);
	imgOP.copyTo(dstImage, edge);
	//imshow("Canny operation", dstImage);

	//the 2rd morphological processing
	//Closing operation
	elementCL2 = getStructuringElement(MORPH_RECT, Size(5, 5));//kernel
	morphologyEx(dstImage, imgCL2, MORPH_CLOSE, elementCL2);
	//imshow("Closing operation 2", imgCL2);

	//Opening operation
	elementOP2 = getStructuringElement(MORPH_RECT, Size(4, 4));//kernel
	morphologyEx(imgCL2, imgOP2, MORPH_OPEN, elementOP2);
	//imshow("Opening operation 2", imgOP2);

    reverse_imgOP2 = 255 - imgOP2; // reverse black and white
	//imshow(" reverse_imgOP2", reverse_imgOP2);

	project_and_cut(reverse_imgOP2, img);//separate characters with each cut of pieces using projection
	match_and_print();//match each of the piece with templates and find the optimal one, then print it on the original image

	system("color 1F");//do you like blue? :)

	//等待用户按键
	waitKey(0);
	return 0;
}

//resize(imgT, imgT, Size(510, 680));//change the scale of images