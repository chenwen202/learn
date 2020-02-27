#include <iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/imgproc.hpp>
using namespace cv;
using namespace std;
void HoughM(const Mat &img, vector<Vec2f>&Ve, int theta0,int MinNum);
int main()
{
    Mat srcImage = imread("data2.jpg");
    if (!srcImage.data)
    {
        cout << "no srcImage" << endl;
        return 0;
    }
    imshow("orinageImage", srcImage);
    Mat midImage, dstImage;
    cvtColor(srcImage, midImage, CV_BGR2GRAY);
    GaussianBlur(midImage, midImage, Size(3, 3), 0, 0);
    Canny(midImage, dstImage, 100, 200,3);
    //cvtColor(midImage, dstImage, CV_GRAY2BGR);
    imshow("edgeDetect", dstImage);
    vector<Vec2f> lines;
    //HoughLines(dstImage, lines, 1, CV_PI / 360, 170, 0, 0);
    HoughM(dstImage, lines, 1, 170);
    float fRate = (float)(CV_PI / 180);
    vector<Vec2f>::const_iterator it = lines.begin();
    cout << lines.size() << endl;
    //while (it != lines.end())
    //{
    //    float rho = (*it)[0];
    //    float theta = (*it)[1];
    //    if (theta < CV_PI / 4. || theta>3. * CV_PI / 4)
    //    {
    //        Point pt1(rho / cos(theta), 0);
    //        Point pt2((rho - dstImage.rows*sin(theta)) / cos(theta), dstImage.rows);
    //        line(dstImage, pt1, pt2, Scalar(100,0,255), 2);

    //    }
    //    it++;
    //    cout << ".";
    //}
    for (size_t i = 0; i < lines.size(); i++)
    {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta*fRate), b = sin(theta*fRate);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000 * (-b));
        pt1.y = cvRound(y0 + 1000 * (a));
        pt2.x = cvRound(x0 - 1000 * (-b));
        pt2.y = cvRound(y0 - 1000 * (a));
        //if (fabs(a)<0.001)
        //{
        //    pt1.x = pt2.x = cvRound(rho);
        //    pt1.y = 0;
        //    pt2.y = dstImage.rows;
        //}
        //else if (fabs(b)<0.001)
        //{
        //    pt1.y = pt2.y = cvRound(rho);
        //    pt1.x = 0;
        //    pt2.x = dstImage.cols;
        //}
        //else
        //{
        //    pt1.x = 0;
        //    pt1.y = cvRound(rho / b);
        //    pt2.x = cvRound(rho / a);
        //    pt2.y = 0;
        //}

        line(dstImage, pt1, pt2, Scalar(255, 100, 5), 2, CV_AA);
        //line(dstImage,pt1,pt2,Scalar(55,100,195),1,LINE_AA);
    }
    
    imshow("HoughLine", dstImage);
    waitKey(0);
    return 0;
}
void HoughM(const Mat &img, vector<Vec2f>&Ve,int theta0,int MinNum)
{
    int width = img.cols;
    int height = img.rows;
    int iRMax = (int)sqrt(width*width + height*height) + 1;
    int iThMax = 360;// theta0;
    int *pArray;
    pArray = new int[2*iRMax*iThMax];
    memset(pArray, 0, sizeof(int)*2*iRMax*iThMax);
    float fRate = (float)(CV_PI / 180);
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
        {
            if (img.at<unsigned char>(y,x) == 255)
            {
                for (int iTh = 0; iTh < iThMax; iTh += 1)
                {
                    int iR = (int)(x*cos(iTh*fRate) + y*sin(iTh*fRate));
                    //if (iR < 0)
                    //    continue;
                    iR = iR + iRMax;
                    pArray[2*iRMax*iTh + iR]++;
                }
            }
        }
    for (int theta = 0; theta < 360; theta++)
    {
        for (int r = 0; r < iRMax*2; r++)
        {
            int thetaL = max(0, theta - 1);
            int thetaR = min(359, theta + 1);
            int rL = max(0, r - 1);
            int rR = min(iRMax*2 - 1, r + 1);
            int tmp = pArray[theta*iRMax*2 + r];
            //if ((tmp>MinNum) 
            //    && tmp>pArray[thetaL*iRMax * 2 + rL] && tmp>pArray[thetaL*iRMax * 2 + r] 
            //    && (tmp>pArray[thetaL*iRMax * 2 + rR])&& (tmp > pArray[theta*iRMax * 2 + rL]) 
            //    && (tmp > pArray[theta*iRMax * 2 + rR])&& (tmp > pArray[thetaR*iRMax * 2 + rL]) 
            //    && (tmp > pArray[thetaR*iRMax * 2 + r]) && (tmp > pArray[thetaR*iRMax * 2 + rR])
            if ((tmp>MinNum)
                &&tmp>pArray[thetaL*iRMax * 2 + r]
                && (tmp >= pArray[theta*iRMax * 2 + rR]) && (tmp > pArray[theta*iRMax * 2 + rL])
                && (tmp >= pArray[thetaR*iRMax * 2 + r]))
            {
                Vec2f t = Vec2f(float(r-iRMax), float(theta));
                Ve.push_back(t);
            }
        }
    }
}



int main() {
    std::cout << "Hello Easy C++ project!" << std::endl;
}