#include <iostream>
#include <vector>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

using namespace std;
using namespace cv;
class CameraCalibration
{
private:
    int nunCornersHor;
    int nunCornersVer;
    int square_size;
    cv::Size board_size;
    cv::Size image_size;
    int flag;
    vector<vector<Point2f> > image_points;
    vector<vector<Point3f> > object_points;

    vector<cv::Mat> rvecs, tvecs;
    cv::Mat camera_matrix;
    cv::Mat dist_coeffs;

public:
    CameraCalibration()
    {
        nunCornersHor = 6;
        nunCornersVer = 9;
        square_size = 1;
        board_size = cv::Size(nunCornersHor,nunCornersVer);
        flag = 0;
    }
    void DetectCorners(bool is_Draw = false)
    {
        //read images
        std::cout<<"Reading chessboard images.."<<endl;
        vector<cv::String> fn;
        cv::glob("img/chessboard/*.JPG", fn, false);

        std::cout<<"Detecting and drawing chessboard corners..."<<endl;
        for (size_t i=0; i < fn.size(); i++){
            cv::Mat image = cv::imread(fn[i]);
            image_size = image.size();
            cv::Mat gray;
            vector<Point2f> cornersBuf;
            cv::cvtColor(image, gray, CV_BGR2GRAY);
            bool patternfound = cv::findChessboardCorners(gray, board_size, cornersBuf);

            //if patternfound
            if(patternfound)
            {
                cv::cornerSubPix(gray, cornersBuf, cv::Size(11, 11), cv::Size(-1, -1),
                cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
                image_points.push_back(cornersBuf);
            }

            if(is_Draw){
                cv::drawChessboardCorners(image, board_size, cv::Mat(cornersBuf), patternfound);
                cv::imwrite("res/chessboard_" + std::to_string(i) + ".jpg", image);
            }

        }
    }
    void CalObjPoints()
    {
        vector<Point3f> corners_buf;
        for( int i = 0; i < board_size.height; ++i )
            for( int j = 0; j < board_size.width; ++j )
                corners_buf.push_back(cv::Point3f(j*square_size, i*square_size, 0.0f));
        object_points.push_back(corners_buf);
        object_points.resize(image_points.size(),object_points[0]);
    }
    void Calibration()
    {
        std::cout<<"Runing Calibration..."<<endl;

        double rms = calibrateCamera(object_points, image_points, image_size, camera_matrix,
                                dist_coeffs, rvecs, tvecs, 0);
        std::cout<<"RMS = "<<rms<<endl;
    }


};

int main()
{


    CameraCalibration cc;
    cc.DetectCorners();

    //calculate object points
    cc.CalObjPoints();

    //calibration
    cc.Calibration();


    std::cout<<"Done:)))"<<endl;







}
