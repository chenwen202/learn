#include <iostream>
#include <string>
#include "opencv2/opencv.hpp"
#include <opencv2\imgproc\types_c.h>
#include <zbar.h>

using namespace cv;
using namespace std;
using namespace zbar;


int main(int argc,char* argv[])
{
    string fileNameString("./bin/barcode.jpg");
    string windowNameString("result ");
    string resultFileNameSring("./bin/barcodeResult.jpg");
    string title;
    Mat srcImage,grayImage,blurImage,thresholdImage,gradientXImage,gradientYImage,gradientImage,morphImage;
 
    srcImage = imread(fileNameString);
    if (srcImage.empty())
    {
        cout << "image file read error" << endl;
        return -1;
    }

    if (srcImage.channels() == 3)
    {
        cvtColor(srcImage, grayImage, CV_RGB2GRAY);
    }
    else
    {
        grayImage = srcImage.clone();
    }
    // title = string(windowNameString) + string("gray");   
    // imshow(title.c_str(),grayImage);

    //build gradient amplitude image
    Scharr(grayImage, gradientXImage, CV_32F, 1, 0);
    Scharr(grayImage, gradientYImage, CV_32F, 0, 1);

    addWeighted(gradientXImage, 0.5, gradientYImage, 0.5, 0, gradientImage);
    //normalize to 8-bit image
    convertScaleAbs(gradientImage, gradientImage);
    // title = string(windowNameString) + string("gradientImage_sum");
    // imshow(title.c_str(),gradientImage);


    //bluring in order to descrease noise
    blur(gradientImage, blurImage, Size(9, 9));
    // title = string(windowNameString) + string("blur");
    // imshow(title.c_str(),blurImage);

    //convert into binary image, the threshold maybe adjusted according to image
    threshold(blurImage, thresholdImage, 160, 255, THRESH_BINARY);
    title = string(windowNameString) + string("threshold");   
    imshow(title.c_str(), thresholdImage);

    //二值化以后的图像,条形码之间的黑白没有连接起来,就要进行形态学运算,消除缝隙,选择闭运算
    //因为填充物体内细小空洞，连接邻近物体
    Mat kernel = getStructuringElement(MORPH_RECT, Size(21, 7));
    morphologyEx(thresholdImage, morphImage, MORPH_CLOSE, kernel);
    // title = string(windowNameString) + string("morphImage");
    // imshow(title.c_str(),morphImage);

    // //现在要让条形码区域连接在一起,所以选择膨胀腐蚀,而且为了保持图形大小基本不变,应该使用相同次数的膨胀腐蚀
    // //先腐蚀,让其他区域的亮的地方变少最好是消除,然后膨胀回来,消除干扰,迭代次数根据实际情况选择
    // erode(morphImage, morphImage, getStructuringElement(MORPH_RECT, Size(3,3)),Point(-1,-1),4);
    // dilate(morphImage, morphImage, getStructuringElement(MORPH_RECT, Size(3,3)),Point(-1,-1),4);

    // title = string(windowNameString) + string("morphImage_then");
    // imshow(title.c_str(),morphImage);

    vector<vector<Point2i>> contours;
    vector<float> contourArea;
    //接下来对目标轮廓进行查找,目标是为了计算图像面积
    findContours(morphImage, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    //计算轮廓的面积并且存放
    for (int i = 0; i < contours.size(); i++)
    {
        contourArea.push_back(cv::contourArea(contours[i]));
    }

    //find the largest area contour
    double maxValue;
    Point maxLoc;
    minMaxLoc(contourArea, NULL, &maxValue, NULL, &maxLoc);

    //计算面积最大的轮廓的最小的外包矩形
    RotatedRect minRect = minAreaRect(contours[maxLoc.x]);

    //为了防止找错,要检查这个矩形的偏斜角度不能超标
    //如果超标,那就是没找到
    if (minRect.angle < 2.0)
    {
        //找到了矩形的角度,但是这是一个旋转矩形,所以还要重新获得一个外包最小矩形
        Rect myRect = boundingRect(contours[maxLoc.x]);
        // //把这个矩形在源图像中画出来
        // rectangle(srcImage,myRect,Scalar(0,255,255),3,LINE_AA);
        // //看看显示效果,找的对不对
        // imshow(windowNameString,srcImage);
        //将扫描的图像裁剪下来,并保存为相应的结果,保留一些X方向的边界,所以对rect进行一定的扩张
        //  myRect.x= myRect.x;
        //  myRect.y= myRect.y;
        myRect.width = myRect.width;
        myRect.height = myRect.height;
        Mat resultImage = Mat(srcImage, myRect);
        if (!imwrite(resultFileNameSring, resultImage))
        {
            cout << "file save error!" << endl;
            return -2;
        }
    }

    //start to barcode recognition
    FileStorage file("./bin/result.xml",FileStorage::WRITE);
    Mat result = imread(resultFileNameSring);
    if (!result.empty())
    {
        cvtColor(result, grayImage, CV_RGB2GRAY);

        int width = grayImage.cols; // extract dimensions
        int height = grayImage.rows;

        Image image(width, height, "Y800", grayImage.data, width * height);
        ImageScanner scanner;
        scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
        int n = scanner.scan(image);
        for (Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol)
        {
            std::cout << "pic name:\t" << resultFileNameSring << std::endl
                      << "code type:\t" << symbol->get_type_name() << std::endl
                      << "decode string:\t" << symbol->get_data() << std::endl;
            image.set_data(NULL, 0);
        }
    }
    waitKey(0);
    return 1;
}


