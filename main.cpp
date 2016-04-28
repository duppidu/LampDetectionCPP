/* 
 * File:   main.cpp
 * Author: thomas.duppenthaler
 *
 * Created on 21. Januar 2016, 01:58
 */
#include <fstream>  
#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>



using namespace std;
using namespace cv;

/*
 * 
 */
class OpenCVTest
{
    //public variables
    Mat originalColour, originalLight, gray, extractedLight, extractingRed, extractingGreen, extractingOrange, lampsearch, hsv;
    const static int ARRAYSIZE = 15;
    std::ofstream ofs;


    //public functions of the class

public:

    struct lamp
    {
        Rect red, orange, green;
        bool redOn, redBlink, orangeOn, orangeBlink, greenOn, greenBlink;

    };
    std::map<int, lamp> lamps;
    void toGrayscale(Mat);
    void extractingLight(Mat);
    void searchRed(Mat);
    void searchGreen(Mat);
    void loop();
    void getMiddlePixelColor(Mat);
    Rect* searchColors(String, Mat);
    void searchLamps();
    void saveResultsToFile();

};

/**
 * prints the color of the pixel in the middle of the screen
 * @param colored
 */


void OpenCVTest::getMiddlePixelColor(Mat colored)
{
    rectangle(colored, Point(318, 238), Point(322, 242), Scalar(255, 0, 190), 1, 8, 0);
    Vec3b pixel = colored.at<Vec3b>(240, 320);
    int b = pixel[0];
    int g = pixel[1];
    int r = pixel[2];

    cout << "B:" << b << "   G:" << g << "   R:" << r << endl;

}

/**
 * compares the area of the contours for the sorting algorithm
 * @param contour1
 * @param contour2
 * @return 
 */


bool compareContourAreas(std::vector<cv::Point> contour1, std::vector<cv::Point> contour2)
{
    double i = fabs(contourArea(cv::Mat(contour1)));
    double j = fabs(contourArea(cv::Mat(contour2)));
    return ( i < j);
}

/**
 * Search for colour function with some image processing work
 * @param color
 * @param original image
 * @return 
 */
Rect* OpenCVTest::searchColors(String color, Mat original)
{

    //Different colors object
    Scalar offMinBGR, offMaxBGR, onMinBGR, onMaxBGR, contColor;
    //matrixes
    Mat colorOn = original;
    Mat colorOff = original;

    Mat canny_output, output, extractedColor;
    //Rectangle array to store every founded bound rectangle of colors
    Rect *boundrect = new Rect[OpenCVTest::ARRAYSIZE];
    //count variable
    int y = 0;

    //defining the min and max values of colors and the color of the contours of the founded color area
    if (color == "red")
    {
        offMinBGR = Scalar(10, 0, 19);
        offMaxBGR = Scalar(60, 1, 180);
        onMinBGR = Scalar(0, 0, 230);
        onMaxBGR = Scalar(20, 8, 255);
        contColor = Scalar(0, 0, 255);

    }
    else if (color == "orange")
    {
        offMinBGR = Scalar(0, 20, 130);
        offMaxBGR = Scalar(50, 125, 210);
        onMinBGR = Scalar(0, 88, 239);
        onMaxBGR = Scalar(40, 220, 255);
        contColor = Scalar(255, 0, 0);
    }
    else if (color == "green")
    {
        offMinBGR = Scalar(130, 109, 0);
        offMaxBGR = Scalar(163, 140, 10);
        onMinBGR = Scalar(180, 200, 80);
        onMaxBGR = Scalar(223, 230, 95);
        contColor = Scalar(0, 255, 0);
    }
    else
    {
        cout << "Wrong Input" << endl;
    }



    //searching for the color with the min max values
    inRange(colorOff, offMinBGR, offMaxBGR, colorOff);
    inRange(colorOn, onMinBGR, onMaxBGR, colorOn);
    //combines the two searches
    bitwise_or(colorOff, colorOn, extractedColor);

    //Values for the erode function
    Mat erodeKernel = getStructuringElement(MORPH_RECT,
            Size(25, 8),
            Point(1, 1));
    //Values for the closing function
    Mat closingKernel = getStructuringElement(MORPH_RECT,
            Size(8, 8),
            Point(4, 4));
    //Values for the opening function
    Mat openingKernel = getStructuringElement(MORPH_RECT,
            Size(3, 3),
            Point(1, 1));

   

//cloesing
    morphologyEx(extractedColor, extractedColor, MORPH_CLOSE, closingKernel);
//opening
    morphologyEx(extractedColor, extractedColor, MORPH_OPEN, openingKernel);
//dialation
    dilate(extractedColor, extractedColor, erodeKernel);
//combining the result with the original
    bitwise_and(original, original, output, extractedColor);



    


    //Contours object
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    //Operation create a image witch is used to find the contours
    Canny(extractedColor, canny_output, 100, 200, 3);

    //Finding Countours opperation
    findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    // sort contours
    std::sort(contours.begin(), contours.end(), compareContourAreas);



    // loop to draw contours
    for (int i = 0; i < contours.size(); i++)
    {


        approxPolyDP(contours[i], contours[i], 10, true);
        Rect x = boundingRect(contours.at(i));

        // just draw contours where the area is bigger than 2500 pixels
        if (x.area() > 10)
        {
            //draw rectangle around the contours
            rectangle(output, Point(x.x, x.y), Point(x.x + x.width, x.y + x.height), contColor, 3, 8, 0);
            boundrect[y] = x;
            y++;

        }
    }
    if (color == "red")
    {
        OpenCVTest::extractingRed = output;

    }
    else if (color == "orange")
    {
        OpenCVTest::extractingOrange = output;
    }
    else if (color == "green")
    {
        OpenCVTest::extractingGreen = output;
    }
    else
    {
        cout << "Wrong Input" << endl;
    }



    return boundrect;
}

/**
 * Makes different opperations with the grayscale image to extract the Light
 * and turns the image into a binary black and whigt image 
 * @param grayscale : input picture in grayscale
 */


void OpenCVTest::extractingLight(Mat original)
{
    int lampCount = 0;


    cvtColor(original, OpenCVTest::extractedLight, CV_BGR2GRAY);



    //turn the brightes whight spots to wight and the rest to black
    threshold(OpenCVTest::extractedLight, OpenCVTest::extractedLight, 249, 255, THRESH_BINARY);

    //defining the eroding model with the shape, size and the point to erode

    Mat cloesingKernel = getStructuringElement(MORPH_RECT,
            Size(4, 4),
            Point(2, 2));

    Mat dilateKernel = getStructuringElement(MORPH_RECT,
            Size(2, 2),
            Point(1, 1));

//closing
    morphologyEx(OpenCVTest::extractedLight, OpenCVTest::extractedLight, MORPH_CLOSE, cloesingKernel);



    //eroding opperation
    dilate(OpenCVTest::extractedLight, OpenCVTest::extractedLight, dilateKernel);


    while (lamps[lampCount].orange.area() > 0)
    {
        int redDetectedCount = 0;
        int orangeDetectedCount = 0;
        int greenDetectedCount = 0;

        for (int orangeYCount = lamps[lampCount].orange.y; orangeYCount < lamps[lampCount].orange.y + lamps[lampCount].orange.height; orangeYCount++)
        {
            for (int orangeXCount = lamps[lampCount].orange.x; orangeXCount < lamps[lampCount].orange.x + lamps[lampCount].orange.width; orangeXCount++)
            {
                Vec3b pixel = OpenCVTest::extractedLight.at<Vec3b>(orangeYCount, orangeXCount);

                if (pixel[0] == 255)
                {
                    orangeDetectedCount++;
                }

            }
        }

        for (int redYCount = lamps[lampCount].red.y; redYCount < lamps[lampCount].red.y + lamps[lampCount].red.height; redYCount++)
        {
            for (int redXCount = lamps[lampCount].red.x; redXCount < lamps[lampCount].red.x + lamps[lampCount].red.width; redXCount++)
            {
                Vec3b pixel = OpenCVTest::extractedLight.at<Vec3b>(redYCount, redXCount);

                if (pixel[0] == 255)
                {
                    redDetectedCount++;
                }

            }
        }

        for (int greenYCount = lamps[lampCount].green.y; greenYCount < lamps[lampCount].green.y + lamps[lampCount].green.height; greenYCount++)
        {
            for (int greenXCount = lamps[lampCount].green.x; greenXCount < lamps[lampCount].green.x + lamps[lampCount].green.width; greenXCount++)
            {
                Vec3b pixel = OpenCVTest::extractedLight.at<Vec3b>(greenYCount, greenXCount);

                if (pixel[0] == 255)
                {
                    greenDetectedCount++;
                }

            }
        }

        if (orangeDetectedCount >= 10)
        {
            lamps[lampCount].orangeOn = true;
        }
        else
        {
            lamps[lampCount].orangeOn = false;
        }

        if (redDetectedCount >= 10)
        {
            lamps[lampCount].redOn = true;
        }
        else
        {
            lamps[lampCount].redOn = false;
        }


        if (greenDetectedCount >= 10)
        {
            lamps[lampCount].greenOn = true;
        }
        else
        {
            lamps[lampCount].greenOn = false;
        }

        cout << redDetectedCount << "     " << orangeDetectedCount << "     " << greenDetectedCount << endl;
        lampCount++;
    }
    OpenCVTest::saveResultsToFile();
    imshow("extracted", OpenCVTest::extractedLight);

}

void OpenCVTest::searchLamps()
{
    Rect* orange = new Rect[OpenCVTest::ARRAYSIZE];
    Rect* red = new Rect[OpenCVTest::ARRAYSIZE];
    Rect* green = new Rect[OpenCVTest::ARRAYSIZE];
    int orangeCount = 0, redCount = 0, greenCount = 0, lampCount = 0;

    //delete all elements in the map
    OpenCVTest::lamps.clear();

    orange = OpenCVTest::searchColors("orange", OpenCVTest::originalColour);

    if (orange[orangeCount].area() > 0)
    {

        red = OpenCVTest::searchColors("red", OpenCVTest::originalColour);
        green = OpenCVTest::searchColors("green", OpenCVTest::originalColour);

        while (orange[orangeCount].area() > 0 || orangeCount >= 20)
        {

            while (red[redCount].area() > 0 || redCount >= 20)
            {
                if (red[redCount].x >= orange[orangeCount].x - 3 * orange[orangeCount].width
                        && red[redCount].x <= orange[orangeCount].x + 3 * orange[orangeCount].width
                        && red[redCount].y >= orange[orangeCount].y - 3 * orange[orangeCount].height
                        && red[redCount].y <= orange[orangeCount].y - 0.2 * orange[orangeCount].height)
                {
                    while (green[greenCount].area() > 0)
                    {
                        if (green[greenCount].x >= orange[orangeCount].x - 3 * orange[orangeCount].width
                                && green[greenCount].x <= orange[orangeCount].x + 3 * orange[orangeCount].width
                                && green[greenCount].y >= orange[orangeCount].y + 0.1 * orange[orangeCount].height
                                && green[greenCount].y <= orange[orangeCount].y + 3 * orange[orangeCount].height)
                        {

                            lamps[lampCount].red = red[redCount];
                            lamps[lampCount].orange = orange[orangeCount];
                            lamps[lampCount].green = green[greenCount];
                            rectangle(OpenCVTest::originalColour, Point(red[redCount].x, red[redCount].y), Point(green[greenCount].x + green[greenCount].width, green[greenCount].y + green[greenCount].height), Scalar(255, 0, 190), 3, 8, 0);
                            lampCount++;

                        }
                        greenCount++;
                    }
                }
                redCount++;
            }
            orangeCount++;
        }
    }
}

/**
 * This Functin is searches for red color in the colored image
 * @param colored
 */


void OpenCVTest::saveResultsToFile()
{
    int lampcount = 0;


    if (lamps[0].orange.area() > 0)
    {
        OpenCVTest::ofs.open("output.txt", std::ofstream::out | std::ofstream::trunc);
        while (lamps[lampcount].orange.area() > 0)
        {
            ofs << lamps[lampcount].redOn << lamps[lampcount].orangeOn << lamps[lampcount].greenOn << "\n";
            lampcount++;
        }
    }
    ofs.close();
}

/**
 * The runing loop witch shows the images
 */
void OpenCVTest::loop()
{
    //open the video stream
    VideoCapture stream(1);


    if (!stream.isOpened())
    {
        cout << "can not open";

    }
    bool is_close_call = false;

    // loop
    while (is_close_call != true)
    {

        //read the image
        stream.read(OpenCVTest::originalColour);
        stream.read(OpenCVTest::originalLight);

        OpenCVTest::getMiddlePixelColor(OpenCVTest::originalColour);

        OpenCVTest::searchLamps();     

        OpenCVTest::extractingLight(OpenCVTest::originalLight);
        imshow("original", OpenCVTest::originalColour);


        //Waiting after the loop is done
        if (waitKey(250) >= 0)
        {
            is_close_call = true;
        }

    }
}

/**
 * Main class to run the programm
 * @param argc
 * @param argv
 * @return 
 */
int main(int argc, char** argv)
{
    OpenCVTest test;
    test.loop();
    return 0;
}

