#include <iostream>
#include "PlanningVisionUpdater.h"
#include "../../Common/config/Config.h"
#include "../../Common/utils/utils.h"
#include "../../Common/config/Handle.h"

using namespace std;

#include <time.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <dhcpcsdk.h>

int clientID;
float target_height = 0.24;
float target_platform = 0.205;
float end_platform = 0.875;
float plane = 0.445;
float platform_height;
float rate = 1.75;
float circle_offsetx;
float circle_offsety;
float targetx_left, targetx_right;
float targety_left, targety_right;
float target_offsetx;
float target_offsety;
float circlex;
float circley;
float t_startx, t_starty, t_finishx, t_finishy, t_targetx, t_targety;
Point position;
simxFloat angle[3];
Point tar_position;
Point p;
Point platform_position;
simxInt camera0, camera1, circle_dummy, target_dummy;
simxInt target, platform;
simxInt resolution[2];
simxUChar *image = 0;

float x_p_left, x_p_right, y_p_left, y_p_right;

PlanningVisionUpdater::PlanningVisionUpdater(int client_id) : PlanningUpdater(
        Config::Instance()->getIntParam("PlanningVisionUpdater", "TIME_STEP"), client_id)
{
    clientID = client_id;
    std::cout << "VisionPlanner constructed" << std::endl;
    camera0 = Handle::Instance()->getObjectHandle("zed_vision0", m_cid);
    camera1 = Handle::Instance()->getObjectHandle("zed_vision1", m_cid);
    target = Handle::Instance()->getObjectHandle("Target", m_cid);
    platform = Handle::Instance()->getObjectHandle("Target_platform", m_cid);
    circle_dummy = Handle::Instance()->getObjectHandle("CircleDummy", m_cid);
    target_dummy = Handle::Instance()->getObjectHandle("TargetDummy", m_cid);
    std::cout << "I'm here" << std::endl;
    simxGetVisionSensorImage(clientID, camera0, resolution, &image, 0, simx_opmode_streaming);
    simxGetVisionSensorImage(clientID, camera1, resolution, &image, 0, simx_opmode_streaming);
    std::cout << "I'm there" << std::endl;
};

void PlanningVisionUpdater::update()
{
    if (!use_vision)
    {
        return;
    }

    if (simxGetVisionSensorImage(clientID, camera0, resolution, &image, 0, simx_opmode_buffer) != simx_return_ok)
    {
        return;
    }

    cv::Mat channel0(resolution[1], resolution[0], CV_8UC3, image);
    //读回来的图像数据是垂直翻转的,问题应该是在cvMat 和 v-rep 垂直坐标轴的方向相反,flip一下就正常了
    flip(channel0, channel0, 0);
//    cv::imshow("img", channel0);
//    cv::waitKey(10);
    //读回来的图像数据时rgb通道分布的，而cvMat 默认bgr
    int startx = -1, starty = -1;
    int finishx = 0, finishy = 0;
    for (int i = 0; i < channel0.rows; i++)
        for (int j = 0; j < channel0.cols; j++)
            if (channel0.at<cv::Vec3b>(i, j)[0] > 170 && channel0.at<cv::Vec3b>(i, j)[1] < 50 &&
                channel0.at<cv::Vec3b>(i, j)[2] < 50)
            {
                if (startx == -1)
                {
                    starty = i;
                    startx = j;
                    t_startx = i;
                    t_starty = j;
                }
                finishy = i;
                finishx = j;
                t_finishx = i;
                t_finishy = j;
            }
    if (startx == -1)
    {
        cout << "左摄像机未发现抓取物体" << endl;
    } else
    {
        targetx_left = 1.0f * (startx + finishx) / 2 - 1.0f * channel0.cols / 2;
        targety_left = 1.0f * channel0.rows / 2 - 1.0f * (starty + finishy) / 2;

        position = utils::getObjectPosition(camera0, m_cid);
        t_targetx = (t_startx + t_finishx) / 2;
        t_targety = (t_starty + t_finishy) / 2;
        target_offsetx = (position[2] - target_height) * rate / 1280 * (640 - t_targety);
        target_offsety = (position[2] - target_height) * rate / 1280 * (t_targetx - 360);
        single_target_pos.setX(position[0] + target_offsetx * cos(angle[2]) + target_offsety * sin(angle[2]));
        single_target_pos.setY(position[1] + target_offsety * cos(angle[2]) - target_offsetx * sin(angle[2]));
        single_target_pos.setZ(plane);
    }

    cv::Mat gray0;
    cvtColor(channel0, gray0, cv::COLOR_RGB2GRAY);
    GaussianBlur(gray0, gray0, cv::Size(9, 9), 2, 2);
    vector<cv::Vec3f> circles0;
    HoughCircles(gray0, circles0, CV_HOUGH_GRADIENT, 1, 10, 100, 30, 10, 1000);
    if (circles0.size() > 0)
    {
        x_p_left = circles0[0][0] - 1.0f * channel0.cols / 2;
        y_p_left = 1.0f * channel0.rows / 2 - circles0[0][1];
    } else
    {
        cout << "左摄像机没找到圆" << endl;
        return;
    }

    if (simxGetVisionSensorImage(clientID, camera1, resolution, &image, 0, simx_opmode_buffer) != simx_return_ok)
    {
        return;
    }

    cv::Mat channel1(resolution[1], resolution[0], CV_8UC3, image);
    flip(channel1, channel1, 0);
    startx = -1, starty = -1;
    finishx = 0, finishy = 0;
    cout << "rows = " << channel1.rows << endl;
    cout << "cols = " << channel1.cols << endl;
    for (int i = 0; i < channel1.rows; i++)
        for (int j = 0; j < channel1.cols; j++)
            if (channel1.at<cv::Vec3b>(i, j)[0] > 170 && channel1.at<cv::Vec3b>(i, j)[1] < 50 &&
                channel1.at<cv::Vec3b>(i, j)[2] < 50)
            {
                if (startx == -1)
                {
                    starty = i;
                    startx = j;
                }
                finishy = i;
                finishx = j;
            }
    if (startx == -1)
    {
        cout << "右摄像机未发现抓取物体" << endl;
    } else
    {
        targetx_right = 1.0f * (startx + finishx) / 2 - 1.0f * channel1.cols / 2;
        targety_right = 1.0f * channel1.rows / 2 - 1.0f * (starty + finishy) / 2;
    }

    cv::Mat gray1;
    cvtColor(channel1, gray1, cv::COLOR_RGB2GRAY);
    GaussianBlur(gray1, gray1, cv::Size(9, 9), 2, 2);
    vector<cv::Vec3f> circles1;
    HoughCircles(gray1, circles1, CV_HOUGH_GRADIENT, 1, 10, 100, 30, 10, 1000);
    if (circles1.size() > 0)
    {
        x_p_right = circles1[0][0] - 1.0f * channel1.cols / 2;
        y_p_right = 1.0f * channel1.rows / 2 - circles1[0][1];
    } else
    {
        cout << "右摄像机未发现圆" << endl;
        return;
    }
    cout << "targetx_left = " << targetx_left << endl;
    cout << "targetx_right = " << targetx_right << endl;
    cout << "targety_left = " << targety_left << endl;
    cout << "targety_right = " << targety_right << endl;
    cout << "x_p_left = " << x_p_left << endl;
    cout << "x_p_right = " << x_p_right << endl;
    cout << "y_p_left =  = " << y_p_left << endl;
    cout << "y_p_right = " << y_p_right << endl;
    Point tmp = utils::getCoordinateInLeftCamera(x_p_left, x_p_right, (y_p_left + y_p_right) / 2);
    tmp.setX(-tmp.x());
    setPosition(tmp, circle_dummy, camera0);
    Point camera0_pos = utils::getObjectPosition(camera0, m_cid);
    recog_circle_pos = Point(camera0_pos.x() + tmp.y(), camera0_pos.y() + tmp.x(), camera0_pos.z() - tmp.z());
    cout << "camera0's position = " << camera0_pos << endl;
    cout << "                   = " << tmp <<endl;
//    cout << "circle_dummy's position = " << utils::getObjectPosition(circle_dummy, m_cid) << endl;
    tmp = utils::getCoordinateInLeftCamera(targetx_left, targetx_right, (targety_left + targety_right) / 2);
    tmp.setX(-tmp.x());
    setPosition(tmp, target_dummy, camera0);
    recog_target_pos = Point(camera0_pos.x() + tmp.y(), camera0_pos.y() + tmp.x(), camera0_pos.z() - tmp.z());
    cout << "camera0's position = " << camera0_pos << endl;
    cout << "                   = " << tmp <<endl;
//    cout << "target_dummy's position = " << utils::getObjectPosition(target_dummy, m_cid) << endl;
}