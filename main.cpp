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
    Mat original, gray, extractedLight, extractingRed, extractingGreen, extractingOrange, lampsearch,hsv;
    const static int ARRAYSIZE = 15;


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
    Rect* findColors(String, Mat);
    void findLamps();
    void saveResultsToFile();
};

/**
 * prints the color of the pixel in the middle of the screen
 * @param colored
 */


void OpenCVTest::getMiddlePixelColor(Mat colored)
{
    rectangle(colored, Point(318, 238), Point(322, 242), Scalar(255, 255, 0), 1, 8, 0);
    Vec3b pixel = colored.at<Vec3b>(240, 320);
    int b = pixel[0];
    int g = pixel[1];
    int r = pixel[2];

    cout << "B:" << b << "   G:" << g << "   R:" << r << endl;

}

/**
 * Turns the original RGB colored image into grayscale
 * @param colored
 */
void OpenCVTest::toGrayscale(Mat colored)
{

    int x, y, b, g, r, gray;
    Mat toGrayScale = colored;

    //loop witch contains every row and count
    for (y = 1; y < 480; ++y)
    {
        //loop witch contains every column of the picture
        //and turns every pixel in this column into grayscale
        for (x = 1; x < 640; ++x)
        {
            Vec3b pixel = toGrayScale.at<Vec3b>(y, x);
            b = pixel[0];
            g = pixel[1];
            r = pixel[2];
            gray = (0.299 * r) +(0.587 * g) + (0.114 * b);
            toGrayScale.at<Vec3b>(y, x)[0] = gray;
            toGrayScale.at<Vec3b>(y, x)[1] = gray;
            toGrayScale.at<Vec3b>(y, x)[2] = gray;
        }

    }
    OpenCVTest::gray = toGrayScale;
}

bool compareContourAreas(std::vector<cv::Point> contour1, std::vector<cv::Point> contour2)
{
    double i = fabs(contourArea(cv::Mat(contour1)));
    double j = fabs(contourArea(cv::Mat(contour2)));
    return ( i < j);
}

Rect* OpenCVTest::findColors(String color, Mat original)
{

    //Different colors object
    Scalar offMinBGR, offMaxBGR, onMinBGR, onMaxBGR,contColor;
    //matrixes
    Mat colorOn = original;
    Mat colorOff = original;
    
    Mat canny_output, output,extractedColor;
    //Rectangle array to store every founded bound rectangle of colors
    Rect *boundrect = new Rect[OpenCVTest::ARRAYSIZE];
    //count variable
    int y = 0;

    //defining the min and max values of colors and the color of the contours of the founded color area
    if (color == "red")
    {
      offMinBGR = Scalar(0, 0, 33);
      offMaxBGR = Scalar(0, 0, 62);
       onMinBGR = Scalar(0, 0, 230);
       onMaxBGR = Scalar(10, 8, 250);
        contColor = Scalar(0, 0, 255);

    }
    else if (color == "orange")
    {
      offMinBGR  = Scalar(0, 1, 59);
      offMaxBGR  = Scalar(1, 18,123);
       onMinBGR  = Scalar(0,88, 239);
       onMaxBGR  = Scalar(4, 139,248);
        contColor = Scalar(255, 0, 0);
    }
    else if (color == "green")
    {
       offMinBGR  = Scalar(40, 60, 10);
       offMaxBGR  = Scalar(90, 150,80);
        onMinBGR  = Scalar(40, 60, 10);
        onMaxBGR  = Scalar(90, 150,80);
        contColor = Scalar(0, 255, 0);
    }
    else
    {
        cout << "Wrong Input" << endl;
    }

    cout << color << endl;

    //searching for the color with the min max values
    inRange(colorOff, offMinBGR, offMaxBGR, colorOff);
    inRange(colorOn, onMinBGR, onMaxBGR, colorOn);
    bitwise_or(colorOff,colorOn,extractedColor);

    Mat erodeKernel = getStructuringElement(MORPH_RECT,
            Size(6, 6),
            Point(1, 1));
    Mat closingKernel = getStructuringElement(MORPH_RECT,
            Size(6, 6),
            Point(4, 4));

    Mat openingKernel = getStructuringElement(MORPH_RECT,
            Size(6, 6),
            Point(4, 4));

    //eroding opperation
    //morphologyEx(extractedColor, extractedColor, MORPH_OPEN, openingKernel);
    
    morphologyEx(extractedColor, extractedColor, MORPH_CLOSE, closingKernel);

    dilate(extractedColor, extractedColor, erodeKernel);

    bitwise_and(original, original, output, extractedColor);



    //contours matrix


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


        approxPolyDP(contours[i], contours[i], 40, true);
        Rect x = boundingRect(contours.at(i));

        // just draw contours where the area is bigger than 2500 pixels
        if (x.area() >=400)
        {
            //draw rectangle around the contours


            rectangle(output, Point(x.x, x.y), Point(x.x + x.width, x.y + x.height), contColor, 3, 8, 0);

            //draw the contours
            drawContours(output, contours, i, contColor, 2, 8, hierarchy, 0, Point(0, 0));

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
void OpenCVTest::extractingLight(Mat grayscale)
{

    //turns the dark gray areas to black and scales the brighter again with grayscale
    threshold(grayscale, OpenCVTest::extractedLight, 150, 255, THRESH_TOZERO);

    //turn the brightes whight spots to wight and the rest to black
    threshold(OpenCVTest::extractedLight, OpenCVTest::extractedLight, 250, 255, THRESH_BINARY);

    //defining the eroding model with the shape, size and the point to erode
    Mat element = getStructuringElement(MORPH_RECT,
            Size(6, 6),
            Point(4, 4));

    //eroding opperation
    erode(OpenCVTest::extractedLight, OpenCVTest::extractedLight, element);

    //contours matrix
    Mat canny_output;

    //Contours object
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    //Operation create a image witch is used to find the contours
   Canny(OpenCVTest::extractedLight, canny_output, 100, 200, 3);
    //Finding Countours opperation
    findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    
   


    for (int lampCount = 0; lampCount < lamps.size(); lampCount++)
    {
        // loop to draw contours
        for (int i = 0; i < contours.size(); i++)
        {
            //approxPolyDP(contours[i],contours[i],40,true);
            Rect contRect = boundingRect(contours.at(i));
            // just draw contours where the area is bigger than 2500 pixels  

            if (contRect.area() >= 2500
                    && contRect.x >= lamps[lampCount].red.x
                    && contRect.x <= lamps[lampCount].red.x + lamps[lampCount].red.width
                    && contRect.y >= lamps[lampCount].red.y
                    && contRect.y <= lamps[lampCount].red.y + lamps[lampCount].red.height)
            {

                lamps[lampCount].redOn = true;
                //draw rectangle around the contours
                rectangle(OpenCVTest::extractedLight, Point(contRect.x, contRect.y), Point(contRect.x + contRect.width, contRect.y + contRect.height), Scalar(255, 0, 255), 3, 8, 0);
                //draw the contours
                drawContours(OpenCVTest::extractedLight, contours, i, Scalar(255, 0, 255), 2, 8, hierarchy, 0, Point(0, 0));

            }
            else
            {
                lamps[lampCount].redOn = false;
            }



            if (contRect.area() >= 2500
                    && contRect.x >= lamps[lampCount].orange.x
                    && contRect.x <= lamps[lampCount].orange.x + lamps[lampCount].orange.width
                    && contRect.y >= lamps[lampCount].orange.y
                    && contRect.y <= lamps[lampCount].orange.y + lamps[lampCount].orange.height)
            {

                lamps[lampCount].orangeOn = true;
                //draw rectangle around the contours
                rectangle(OpenCVTest::extractedLight, Point(contRect.x, contRect.y), Point(contRect.x + contRect.width, contRect.y + contRect.height), Scalar(255, 0, 255), 3, 8, 0);
                //draw the contours
                drawContours(OpenCVTest::extractedLight, contours, i, Scalar(255, 0, 255), 2, 8, hierarchy, 0, Point(0, 0));

            }
            else
            {
                lamps[lampCount].orangeOn = false;
            }


            if (contRect.area() >= 2500
                    && contRect.x >= lamps[lampCount].green.x
                    && contRect.x <= lamps[lampCount].green.x + lamps[lampCount].green.width
                    && contRect.y >= lamps[lampCount].green.y
                    && contRect.y <= lamps[lampCount].green.y + lamps[lampCount].green.height)
            {

                lamps[lampCount].greenOn = true;
                //draw rectangle around the contours
                rectangle(OpenCVTest::extractedLight, Point(contRect.x, contRect.y), Point(contRect.x + contRect.width, contRect.y + contRect.height), Scalar(255, 0, 255), 3, 8, 0);
                //draw the contours
                drawContours(OpenCVTest::extractedLight, contours, i, Scalar(255, 0, 255), 2, 8, hierarchy, 0, Point(0, 0));

            }
            else
            {
                lamps[lampCount].greenOn = false;
            }


        }
    }

 imshow("extracted", OpenCVTest::extractedLight);

}

void OpenCVTest::findLamps()
{
    Rect* orange = new Rect[OpenCVTest::ARRAYSIZE];
    Rect* red = new Rect[OpenCVTest::ARRAYSIZE];
    Rect* green = new Rect[OpenCVTest::ARRAYSIZE];
    int orangeCount = 0, redCount = 0, greenCount = 0, lampCount = 0;

    //delete all elements in the map
    OpenCVTest::lamps.clear();

    orange = OpenCVTest::findColors("orange", OpenCVTest::original);

    if (orange[orangeCount].area() > 0)
    {

        red = OpenCVTest::findColors("red", OpenCVTest::original);
        green = OpenCVTest::findColors("green", OpenCVTest::original);
        cout << "asdf" << endl;
        while (orange[orangeCount].area() > 0 || orangeCount >= 20)
        {

            while (red[redCount].area() > 0 || redCount >= 20)
            {
                if (red[redCount].x >= orange[orangeCount].x - 0.2 * orange[orangeCount].width
                        && red[redCount].x <= orange[orangeCount].x + 0.2 * orange[orangeCount].width
                        && red[redCount].y >= orange[orangeCount].y + 1.2 * orange[orangeCount].height
                        && red[redCount].y <= orange[orangeCount].y + 0.8 * orange[orangeCount].height)
                {
                    while (green[greenCount].area() > 0)
                    {
                        if (green[greenCount].x >= orange[orangeCount].x - 0.2 * orange[orangeCount].width
                                && green[greenCount].x <= orange[orangeCount].x + 0.2 * orange[orangeCount].width
                                && green[greenCount].y >= orange[orangeCount].y - 0.8 * orange[orangeCount].height
                                && green[greenCount].y <= orange[orangeCount].y - 1.2 * orange[orangeCount].height)
                        {
                            lamps[lampCount].red = red[redCount];
                            lamps[lampCount].orange = orange[orangeCount];
                            lamps[lampCount].green = green[greenCount];
                            rectangle(OpenCVTest::original, Point(red[redCount].x, red[redCount].y), Point(green[greenCount].x + green[greenCount].width, green[greenCount].y + green[greenCount].height), Scalar(255, 0, 190), 3, 8, 0);
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
    std::ofstream ofs;
    ofs.open("output.txt", std::ofstream::out | std::ofstream::trunc);
    while (lamps[lampcount].orange.area() > 0)
    {
        ofs << lamps[lampcount].redOn << lamps[lampcount].orangeOn << lamps[lampcount].greenOn << "\n";
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
        stream.read(OpenCVTest::original);

        OpenCVTest::getMiddlePixelColor(OpenCVTest::original);
        //show the image on on a window

        imshow("cam", OpenCVTest::original);






        //OpenCVTest::findLamps();
        stream.read(OpenCVTest::original);
        
        
        OpenCVTest::findColors("orange",original);
        imshow("orange", extractingOrange);
        //OpenCVTest::findColors("red",original);
        //imshow("red", extractingRed);
        //OpenCVTest::findColors("green",original);
        //imshow("green", extractingGreen);
        //stream.read(OpenCVTest::original);
        //OpenCVTest::toGrayscale(OpenCVTest::original);
       // imshow("gray", OpenCVTest::gray);
       // OpenCVTest::extractingLight(OpenCVTest::gray);
         //stream.read(OpenCVTest::original);
        
      
        //OpenCVTest::saveResultsToFile();
        
        
        
        //add(extractingRed, extractingGreen, lampsearch);
        //imshow("lampsearch", lampsearch);


        // stream.read(OpenCVTest::original);
        // OpenCVTest::searchGreen(original);
        //imshow("green", OpenCVTest::extractingGreen);

        //Waiting after the loop is done
        if (waitKey(150) >= 0)
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

