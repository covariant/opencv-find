#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

 
using namespace cv;
using namespace std;

int thresh = 100;
string window_title;
string wafer;

RNG rng(12345);

bool compareContourAreas ( vector<Point> contour1, vector<Point> contour2 ) {
    double i = fabs( contourArea(Mat(contour1)) );
    double j = fabs( contourArea(Mat(contour2)) );
    return ( i < j );
}
 
int main()
{
    int i = 1;
    for (i = 1; i < 6 ; i++) {
        wafer = "wafer" + to_string(i) + ".jpg";
        std::string image_path = samples::findFile(wafer);
        Mat img = imread(image_path, IMREAD_COLOR);
    
        Mat dst, dilated, thresholded;
        Mat orig = img.clone();
        Mat with_contours = img.clone();
        Mat with_circle = img.clone();
     
        if(img.empty())
        {
            std::cout << "Could not read the image: " << image_path << std::endl;
            return 1;
        }
    
        cvtColor( img, img, COLOR_BGR2GRAY );
    
        threshold( img, img, 100, 255, 0);
        
        Point p1(0,0);
        Point p2(180,500);
    
        rectangle(img, p1, p2, Scalar(255, 255, 255), -1, LINE_8);
        thresholded = img.clone();
    
        Mat canny;
        Canny( img, canny, thresh, thresh*2 );
    
        int erosion_size = 1;
    
        Mat element = getStructuringElement( MORPH_RECT,Size( 2*erosion_size+1, 2*erosion_size+1), Point( erosion_size, erosion_size ) );
        dilate( canny, dilated, element );
    
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        findContours( dilated, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE );
        //printf("%lu",contours.size());
        sort(contours.begin(), contours.end(), compareContourAreas);
    
        int largest = contours.size()-1;
    
        drawContours(with_contours, contours, largest, Scalar(0,255,0), 3);
    
        Point2f c;
        float radius;
    
        minEnclosingCircle(contours[largest],c,radius);
        circle(with_circle,c,radius,Scalar(0,255,0),5);

	window_title = string("locate ") + string("wafer");
    
        imshow(window_title, orig);
        int a = waitKey(0); 
        imshow(window_title, thresholded);
        a = waitKey(0); 
        imshow(window_title, canny);
        a = waitKey(0); 
        imshow(window_title, dilated);
        a = waitKey(0); 
        imshow(window_title, with_contours);
        a = waitKey(0); 
        imshow(window_title, with_circle);
        a = waitKey(0); 
    
        /*
        Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
        for( size_t i = 0; i< contours.size(); i++ )
        {
            Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
            drawContours( drawing, contours, (int)i, color, 2, LINE_8, hierarchy, 0 );
        }
    
     
        imshow("Display window", drawing);
        int p = waitKey(0); // Wait for a keystroke in the window
        */ 
    }

    return 0;
}
