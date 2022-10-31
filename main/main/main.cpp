//  Developer: Baturay Özgürün
//  Copyright © 2018 Baturay Özgürün
//  If you have any requests or questions please do not hesitate to contact me via baturay.ozgurun@gmail.com

//  Project Name: Computer Vision -- Camera Lens Distortion Correction Algorithm with OpenCV and C++
//  Project Desciption: This project aims to correct distorted images and obtain intrinsic and extrinsic parameters by capitalizing on a chessboard pattern and OpenCV functions such as findChessboardCorners, cornerSubPix, calibrateCamera, and undistort.

//Include Headers
#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>

//Include Namespaces
using namespace cv;
using namespace std;


int main(){
    //Create Variables
    int NumberHorizontalCorner=10; //Number of corners along horizontal line
    int NumberVerticalCorner=6; //Number of corners along vertical line
    int Square; //Number of sqaures which is number of corners along horizontal line times number of corners along vertical line
    Size CornerSize; //Variable for the size of the corners
    
    //Calculation of some variables
    Square = NumberHorizontalCorner * NumberVerticalCorner; //Define number of squares
    CornerSize = Size(NumberHorizontalCorner, NumberVerticalCorner); //Define the size of the corners
    
    vector<vector<Point3f>> object_points; //Create R^3 object points - Physical Position of the Corners
    vector<vector<Point2f>> image_points; //Create R^2 object points - Location of the Corners on in the Image
    vector<Point2f> corners; //Create list of corner vectors
    vector<Point3f> obj; //Create list of object vectors
    
    Mat image; //Create matrix named image
    Mat gray_image; //Create matrix named gray_image
    ///image = imread("distortion.jpeg"); //Read image and assign it to the image variable
    image = imread("data/distortion.jpeg"); //Read image and assign it to the image variable

    for(int j=0;j<Square;j++)
        obj.push_back(Point3f((j/NumberHorizontalCorner), (j%NumberHorizontalCorner), 0.0f)); //Generate coordinates like (0,0,0), (0,1,0)...
    
    cvtColor(image,gray_image,CV_BGR2GRAY); //Convert the image into gray scale image and keep it with the variable
    bool f; //Define f variable
    //This finds the Chess Board Corners. Inputs: Image (gray_image) and Corner Size (CornerSize). Output: Pixel Locations of Corners (corners)
    //The last terms are flags, and they improve to detect corner locations.
    f = findChessboardCorners(gray_image, CornerSize, corners, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
    
    if (f){
    //Sub Pixel Corners are detected with the inputs of the image (gray_image) and corner locations (corners).
       cornerSubPix(gray_image, corners, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
    //Show the corners on the image
       drawChessboardCorners(gray_image, CornerSize, corners, f);
    }
    
    imshow("Image", image); //Show the input image
    imshow("Gray Image", gray_image); //Show the gray scale image contains corners
    imwrite("results/Input_Image.jpg", image); //Write the input image
    imwrite("results/Chessboard_Corners.jpg", gray_image); //Write the gray scale image contains corners
    waitKey(2000); //Wait to show images
    
    if(f==1){
        image_points.push_back(corners); //Add corner variables to the image_points
        object_points.push_back(obj); //Add generated coordinates (obj.push_back) to the object_points
    }
    
    for (int i = 0; i < (int) corners.size(); i++)
        cout<< "Corners:" << corners.at(i) <<endl; //Show corner variables
    
    for (int i = 0; i < (int) object_points.size(); i++)
        cout<< "Object Points" << object_points.at(i) <<endl; //Show coordinates
    
    for (int i = 0; i < (int)image_points.size(); i++)
        cout<< "Image Points" << image_points.at(i) <<endl; //Show corners
    
    Mat Intrinsic, DistortionCoefficients, Undistorted_Image; //Create matrix variables
    vector<Mat> RotationVector, TranslationVector; //Create vectors
    
    //calibrateCamera function drives corners and object coordinates to create parameters such as Instrinsic, Distortion Coeff, Rotation and Trans Vect
    calibrateCamera(object_points, image_points, image.size(), Intrinsic, DistortionCoefficients, RotationVector, TranslationVector);
    undistort(image, Undistorted_Image, Intrinsic, DistortionCoefficients); //Undistorted image is obtained using Instrinsic and Distortion Coeff
    imwrite("results/Undistorted_Image.jpg", Undistorted_Image); //Write the undistorted image
    
    return 0;
}

