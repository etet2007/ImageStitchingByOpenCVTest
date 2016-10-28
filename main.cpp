#include <QCoreApplication>


#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/stitching.hpp>

using namespace std;
using namespace cv;
bool try_use_gpu = false;
vector<Mat> imgs;
string result_name = "result.jpg";
int image_count=2;                    /****    图像数量     ****/


int main(int argc, char *argv[])
{
    for( int i = 1;  i<= image_count ; i++)
    {
        string imageFileName;
        std::stringstream StrStm;
        StrStm<<i;
        StrStm>>imageFileName;
        imageFileName += ".jpg";

        Mat imgTemp = imread(imageFileName);
        cout<<imageFileName<<endl;
        cout<<"Frame #"<<i<<"...end"<<endl;
        if (imgTemp.empty())
        {
            cout << "Can't read image"<< endl;
            return -1;
        }
        imgs.push_back(imgTemp);
    }

    Stitcher stitcher = Stitcher::createDefault(try_use_gpu);

    // 使用stitch函数进行拼接
    Mat pano;
    Stitcher::Status status = stitcher.stitch(imgs, pano);

    cout<<status<<endl;
    /*          OK = 0,
                ERR_NEED_MORE_IMGS = 1,
                ERR_HOMOGRAPHY_EST_FAIL = 2,
                ERR_CAMERA_PARAMS_ADJUST_FAIL = 3
    */

    switch(status){
    case Stitcher::OK:
        cout << "OK."<< endl;
        break;
    case Stitcher::ERR_NEED_MORE_IMGS:
        cout << "ERR_NEED_MORE_IMGS."<< endl;
        return -1;

    case Stitcher::ERR_HOMOGRAPHY_EST_FAIL:
        cout << "HOMOGRAPHY_EST_FAIL."<< endl;
        return -1;

    case Stitcher::ERR_CAMERA_PARAMS_ADJUST_FAIL:
        cout << "ERR_CAMERA_PARAMS_ADJUST_FAIL."<< endl;
        return -1;
    }


    imwrite(result_name, pano);


    imshow("stitch image", pano);

    if(waitKey()==27)
        return 0;

}
