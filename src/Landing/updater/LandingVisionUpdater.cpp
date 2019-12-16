#include <iostream>
#include "LandingVisionUpdater.h"
#include <cmath>
#include "LandingUpdater.h"
#include "../../Common/utils/utils.h"

using namespace std;

#include <time.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int clientID;
float midx, midy;
Point p;
float height = 0.282;
float rate = 1.75;
float offsetx;
float offsety;
Point position;
simxFloat angle[3];
Point tar_position;
simxInt camera0;
simxInt target;
simxUChar *image = 0;
simxInt resolution[2];

LandingVisionUpdater::LandingVisionUpdater(int client_id) : LandingUpdater(
        Config::Instance()->getIntParam("LandingVisionUpdater", "TIME_STEP"), client_id)
{
    clientID = client_id;
    std::cout << "VisionPlanner constructed" << std::endl;
    simxGetObjectHandle(clientID, "zed_vision0", &camera0, simx_opmode_blocking);
    simxGetObjectHandle(clientID, "land_plane", &target, simx_opmode_blocking);
    std::cout << camera0 << ' ' << target << std::endl;
    simxGetObjectOrientation(clientID, camera0, -1, angle, simx_opmode_streaming);
    //simxGetVisionSensorImage(clientID, camera, resolution, &image, 0, simx_opmode_streaming);
}

void LandingVisionUpdater::update() {

    int Up = 0;
    int Down = 0;
    int Left = 0;
    int Right = 0;
    int LeftUp = -1;
    int LeftDown = -1;
    int UpLeft = -1;
    int UpRight = -1;
    int RightUp = -1;
    int RightDown = -1;
    int DownLeft = -1;
    int DownRight = -1;
    int ret = simxGetVisionSensorImage(clientID, camera0, resolution, &image, 0, simx_opmode_blocking);
    if (ret != simx_return_ok) {
        return;
    }
    position = utils::getObjectPosition(camera0, m_cid);
    float length = 0.8 * 1280 / rate / (position[2] - height);
    simxGetObjectOrientation(clientID, camera0, -1, angle, simx_opmode_buffer);
    cv::Mat channel(resolution[1], resolution[0], CV_8UC3, image);
    //鐠囪娲栭弶銉ф畱閸ユ儳鍎氶弫鐗堝祦閺勵垰鐎惄瀵哥倳鏉烆剛娈?闂傤噣顣芥惔鏃囶嚉閺勵垰婀猚vMat 閸?v-rep 閸ㄥ倻娲块崸鎰垼鏉炲娈戦弬鐟版倻閻╃寮?flip娑撯偓娑撳姘ㄥ锝呯埗娴?
    flip(channel, channel, 0);
    cv::threshold(channel, channel, 220, 255, CV_THRESH_TOZERO_INV);
    cv::imshow("img", channel);
    cv::waitKey(10);

    int count = 0;
    for (int j = 0; j < 1280; j++) {
        if (channel.at<cv::Vec3b>(0, j)[0] < 30 && channel.at<cv::Vec3b>(0, j)[1] < 30 &&
            channel.at<cv::Vec3b>(0, j)[2] < 30) {
            if (UpLeft == -1)
                UpLeft = j;
            UpRight = j;
            count++;
            if (count >= 3)
                Up = 1;
        }
    }
    count = 0;
    for (int j = 0; j < 1280; j++) {
        if (channel.at<cv::Vec3b>(719, j)[0] < 30 && channel.at<cv::Vec3b>(719, j)[1] < 30 &&
            channel.at<cv::Vec3b>(719, j)[2] < 30) {
            if (DownLeft == -1)
                DownLeft = j;
            DownRight = j;
            count++;
            if (count >= 3)
                Down = 1;
        }
    }
    count = 0;
    for (int i = 0; i < 720; i++) {
        if (channel.at<cv::Vec3b>(i, 0)[0] < 30 && channel.at<cv::Vec3b>(i, 0)[1] < 30 &&
            channel.at<cv::Vec3b>(i, 0)[2] < 30) {
            if (LeftUp == -1)
                LeftUp = i;
            LeftDown = i;
            count++;
            if (count >= 3)
                Left = 1;
        }
    }
    count = 0;
    for (int i = 0; i < 720; i++) {
        if (channel.at<cv::Vec3b>(i, 1279)[0] < 30 && channel.at<cv::Vec3b>(i, 1279)[1] < 30 &&
            channel.at<cv::Vec3b>(i, 1279)[2] < 30) {
            if (RightUp == -1)
                RightUp = i;
            RightDown = i;
            count++;
            if (count >= 3)
                Right = 1;
        }
    }
    cout << Up + Down + Left + Right << endl;
    cout << UpLeft << " " << UpRight << " " << RightUp << " " << RightDown << " " << DownRight << " " << DownLeft << " "
         << LeftDown << " " << LeftUp << endl;
    if (Up + Down + Left + Right == 4) {
        p.setX(position[0]);
        p.setY(position[1]);
        p.setZ(height);
        QRCode_pos = p;
        simxSetFloatSignal(clientID, "QRcode_x", p.x(), simx_opmode_oneshot);
        simxSetFloatSignal(clientID, "QRcode_y", p.y(), simx_opmode_oneshot);
        simxSetFloatSignal(clientID, "QRcode_z", p.z(), simx_opmode_oneshot);
        return;
    } else if (Up + Down + Left + Right == 3) {
        if (Up == 0) {
            int startx = -1, starty = -1;
            bool Break = false;
            for (int i = 10; i < channel.rows - 10; i++) {
                for (int j = 10; j < channel.cols - 10; j++)
                    if (channel.at<cv::Vec3b>(i, j)[0] < 30 && channel.at<cv::Vec3b>(i, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i, j)[2] < 30 && channel.at<cv::Vec3b>(i - 1, j)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j)[2] < 30 && channel.at<cv::Vec3b>(i - 2, j)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j)[2] < 30 && channel.at<cv::Vec3b>(i, j - 1)[0] < 30 &&
                        channel.at<cv::Vec3b>(i, j - 1)[1] < 30 && channel.at<cv::Vec3b>(i, j - 1)[2] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j - 1)[0] < 30 && channel.at<cv::Vec3b>(i - 1, j - 1)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j - 1)[2] < 30 && channel.at<cv::Vec3b>(i - 2, j - 1)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j - 1)[1] < 30 && channel.at<cv::Vec3b>(i - 2, j - 1)[2] < 30) {
                        startx = i;
                        starty = j;
                        Break = true;
                        break;
                    }
                if (Break)
                    break;
            }
            cout << "upx: " << startx << "  upy: " << starty << endl;
            if (starty < 15 || starty > 1265) {
                midx = (LeftUp + RightUp +
                        1280 * length / sqrt(1280 * 1280 + (LeftUp - RightUp) * (LeftUp - RightUp))) / 2;
                midy = (1280 +
                        (LeftUp - RightUp) * length / sqrt(1280 * 1280 + (LeftUp - RightUp) * (LeftUp - RightUp))) / 2;
            } else {
                float left = sqrt(starty * starty + (LeftUp - startx) * (LeftUp - startx));
                float right = sqrt((1280 - starty) * (1280 - starty) + (RightUp - startx) * (RightUp - startx));
                midx = startx + ((LeftUp - startx) / left + (RightUp - startx) / right) * length / 2;
                midy = starty + ((1280 - starty) / right - starty / left) * length / 2;
            }
        } else if (Down == 0) {
            int startx = -1, starty = -1;
            for (int i = 10; i < channel.rows - 10; i++) {
                for (int j = 10; j < channel.cols - 10; j++)
                    if (channel.at<cv::Vec3b>(i, j)[0] < 30 && channel.at<cv::Vec3b>(i, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i, j)[2] < 30 && channel.at<cv::Vec3b>(i - 1, j)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j)[2] < 30 && channel.at<cv::Vec3b>(i - 2, j)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j)[2] < 30 && channel.at<cv::Vec3b>(i, j - 1)[0] < 30 &&
                        channel.at<cv::Vec3b>(i, j - 1)[1] < 30 && channel.at<cv::Vec3b>(i, j - 1)[2] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j - 1)[0] < 30 && channel.at<cv::Vec3b>(i - 1, j - 1)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j - 1)[2] < 30 && channel.at<cv::Vec3b>(i - 2, j - 1)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j - 1)[1] < 30 && channel.at<cv::Vec3b>(i - 2, j - 1)[2] < 30) {
                        startx = i;
                        starty = j;
                    }
            }
            cout << "downx: " << startx << "  downy: " << starty << endl;
            if (starty < 15 || starty > 1265) {
                midx = (LeftDown + RightDown -
                        1280 * length / sqrt(1280 * 1280 + (LeftDown - RightDown) * (LeftDown - RightDown))) / 2;
                midy = (1280 +
                        (RightDown - LeftDown) * length /
                        sqrt(1280 * 1280 + (LeftDown - RightDown) * (LeftDown - RightDown))) / 2;
            } else {
                float left = sqrt(starty * starty + (startx - LeftDown) * (startx - LeftDown));
                float right = sqrt((1280 - starty) * (1280 - starty) + (startx - RightDown) * (startx - RightDown));
                midx = startx - ((startx - LeftDown) / left + (startx - RightDown) / right) * length / 2;
                midy = starty + ((1280 - starty) / right - starty / left) * length / 2;
            }
        } else if (Left == 0) {
            int startx = -1, starty = -1;
            bool Break = false;
            for (int j = 10; j < channel.cols - 10; j++) {
                for (int i = 10; i < channel.rows - 10; i++)
                    if (channel.at<cv::Vec3b>(i, j)[0] < 30 && channel.at<cv::Vec3b>(i, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i, j)[2] < 30 && channel.at<cv::Vec3b>(i - 1, j)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j)[2] < 30 && channel.at<cv::Vec3b>(i - 2, j)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j)[2] < 30 && channel.at<cv::Vec3b>(i, j - 1)[0] < 30 &&
                        channel.at<cv::Vec3b>(i, j - 1)[1] < 30 && channel.at<cv::Vec3b>(i, j - 1)[2] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j - 1)[0] < 30 && channel.at<cv::Vec3b>(i - 1, j - 1)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j - 1)[2] < 30 && channel.at<cv::Vec3b>(i - 2, j - 1)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j - 1)[1] < 30 && channel.at<cv::Vec3b>(i - 2, j - 1)[2] < 30) {
                        startx = i;
                        starty = j;
                        Break = true;
                        break;
                    }
                if (Break)
                    break;
            }
            cout << "leftx: " << startx << "  lefty: " << starty << endl;
            if (startx < 15 || startx > 705) {
                midx = (720 +
                        (UpLeft - DownLeft) * length / sqrt(720 * 720 + (UpLeft - DownLeft) * (UpLeft - DownLeft))) / 2;
                midy = (DownLeft + UpLeft +
                        720 * length / sqrt(720 * 720 + (UpLeft - DownLeft) * (UpLeft - DownLeft))) / 2;
            } else {
                float up = sqrt(startx * startx + (UpLeft - starty) * (UpLeft - starty));
                float down = sqrt((720 - startx) * (720 - startx) + (DownLeft - starty) * (DownLeft - starty));
                midx = startx + ((720 - startx) / down - startx / up) * length / 2;
                midy = starty + ((UpLeft - starty) / up + (DownLeft - starty) / down) * length / 2;
            }
        } else if (Right == 0) {
            int startx = -1, starty = -1;
            for (int j = 10; j < channel.cols - 10; j++) {
                for (int i = 10; i < channel.rows - 10; i++)
                    if (channel.at<cv::Vec3b>(i, j)[0] < 30 && channel.at<cv::Vec3b>(i, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i, j)[2] < 30 && channel.at<cv::Vec3b>(i - 1, j)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j)[2] < 30 && channel.at<cv::Vec3b>(i - 2, j)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j)[2] < 30 && channel.at<cv::Vec3b>(i, j - 1)[0] < 30 &&
                        channel.at<cv::Vec3b>(i, j - 1)[1] < 30 && channel.at<cv::Vec3b>(i, j - 1)[2] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j - 1)[0] < 30 && channel.at<cv::Vec3b>(i - 1, j - 1)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j - 1)[2] < 30 && channel.at<cv::Vec3b>(i - 2, j - 1)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j - 1)[1] < 30 && channel.at<cv::Vec3b>(i - 2, j - 1)[2] < 30) {
                        startx = i;
                        starty = j;
                    }
            }
            cout << "rightx: " << startx << "  righty: " << starty << endl;
            if (startx > 15 || startx < 705) {
                midx = (720 +
                        (DownRight - UpRight) * length /
                        sqrt(720 * 720 + (UpRight - DownRight) * (UpRight - DownRight))) /
                       2;
                midy = (DownRight + UpRight -
                        720 * length / sqrt(720 * 720 + (UpRight - DownRight) * (UpRight - DownRight))) / 2;
            } else {
                float up = sqrt(startx * startx + (UpRight - starty) * (UpRight - starty));
                float down = sqrt((720 - startx) * (720 - startx) + (DownRight - starty) * (DownRight - starty));
                midx = startx + ((720 - startx) / down - startx / up) * length / 2;
                midy = starty - ((starty - UpRight) / up + (starty - DownRight) / down) * length / 2;
            }
        }
    } else if (Up + Down + Left + Right == 2) {
        if (Up == 1 && Down == 1) {
            midx = 360;
            midy = (UpLeft + DownRight) / 2;
        } else if (Up == 1 && Right == 1) {
            int leftx = -1, lefty = 1280;
            int downx = -1, downy = -1;
            for (int i = 10; i < channel.rows - 10; i++)
                for (int j = 10; j < channel.cols - 10; j++)
                    if (channel.at<cv::Vec3b>(i, j)[0] < 30 && channel.at<cv::Vec3b>(i, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i, j)[2] < 30 && channel.at<cv::Vec3b>(i - 1, j)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j)[2] < 30 && channel.at<cv::Vec3b>(i - 2, j)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j)[2] < 30 && channel.at<cv::Vec3b>(i, j - 1)[0] < 30 &&
                        channel.at<cv::Vec3b>(i, j - 1)[1] < 30 && channel.at<cv::Vec3b>(i, j - 1)[2] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j - 1)[0] < 30 && channel.at<cv::Vec3b>(i - 1, j - 1)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j - 1)[2] < 30 && channel.at<cv::Vec3b>(i - 2, j - 1)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j - 1)[1] < 30 && channel.at<cv::Vec3b>(i - 2, j - 1)[2] < 30) {
                        if (i > RightDown) {
                            downx = i;
                            downy = j;
                        }
                        if (j < UpLeft) {
                            if (j < lefty) {
                                leftx = i;
                                lefty = j;
                            }
                        }
                    }
            cout << "leftx: " << leftx << "  lefty: " << lefty << "  downx: " << downx << "  downy: " << downy << endl;
            if (leftx == -1) {
                if (downx == -1) {
                    midx = (RightDown + (UpLeft - 1280) * length /
                                        sqrt(RightDown * RightDown + (1280 - UpLeft) * (1280 - UpLeft))) / 2;
                    midy = (UpLeft + 1280 +
                            RightDown * length / sqrt(RightDown * RightDown + (1280 - UpLeft) * (1280 - UpLeft))) / 2;
                } else {
                    float left = sqrt(downx * downx + (downy - UpLeft) * (downy - UpLeft));
                    float right = sqrt((1280 - downy) * (1280 - downy) + (downx - RightDown) * (downx - RightDown));
                    midx = downx - (downx / left + (downx - RightDown) / right) * length / 2;
                    midy = downy + ((1280 - downy) / right - (downy - UpLeft) / left) * length / 2;
                }
            } else {
                if (downx == -1) {
                    float up = sqrt(leftx * leftx + (lefty - UpLeft) * (lefty - UpLeft));
                    float down = sqrt((1280 - lefty) * (1280 - lefty) + (leftx - RightDown) * (leftx - RightDown));
                    midx = leftx + ((RightDown - leftx) / down - leftx / up) * length / 2;
                    midy = lefty + ((1280 - lefty) / down - (UpLeft - lefty) / up) * length / 2;
                } else {
                    midx = (leftx + downx + lefty - downy) / 2;
                    midy = (downx - leftx + lefty + downy) / 2;
                }
            }
        } else if (Up == 1 && Left == 1) {
            int rightx = -1, righty = -1;
            int downx = -1, downy = -1;
            for (int i = 10; i < channel.rows - 10; i++)
                for (int j = 10; j < channel.cols - 10; j++)
                    if (channel.at<cv::Vec3b>(i, j)[0] < 30 && channel.at<cv::Vec3b>(i, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i, j)[2] < 30 && channel.at<cv::Vec3b>(i - 1, j)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j)[2] < 30 && channel.at<cv::Vec3b>(i - 2, j)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j)[2] < 30 && channel.at<cv::Vec3b>(i, j - 1)[0] < 30 &&
                        channel.at<cv::Vec3b>(i, j - 1)[1] < 30 && channel.at<cv::Vec3b>(i, j - 1)[2] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j - 1)[0] < 30 && channel.at<cv::Vec3b>(i - 1, j - 1)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j - 1)[2] < 30 && channel.at<cv::Vec3b>(i - 2, j - 1)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j - 1)[1] < 30 && channel.at<cv::Vec3b>(i - 2, j - 1)[2] < 30) {
                        if (i > LeftDown) {
                            downx = i;
                            downy = j;
                        }
                        if (j > UpRight) {
                            if (j > righty) {
                                rightx = i;
                                righty = j;
                            }
                        }
                    }
            cout << "rightx: " << rightx << "  righty: " << righty << "  downx: " << downx << "  downy: " << downy
                 << endl;
            if (rightx == -1) {
                if (downx == -1) {
                    midx = (LeftDown - UpRight * length / sqrt(LeftDown * LeftDown + UpRight * UpRight)) / 2;
                    midy = (UpRight - LeftDown * length / sqrt(LeftDown * LeftDown + UpRight * UpRight)) / 2;
                } else {
                    float left = sqrt(downy * downy + (downx - LeftDown) * (downx - LeftDown));
                    float right = sqrt(downx * downx + (downy - UpRight) * (downy - UpRight));
                    midx = downx - (downx / right + (downx - LeftDown) / left) * length / 2;
                    midy = downy + ((UpRight - downy) / right - downy / left) * length / 2;
                }
            } else {
                if (downx == -1) {
                    float up = sqrt(rightx * rightx + (righty - UpRight) * (righty - UpRight));
                    float down = sqrt(righty * righty + (LeftDown - rightx) * (LeftDown - rightx));
                    midx = rightx + ((LeftDown - rightx) / down - rightx / up) * length / 2;
                    midy = righty - (righty / down + (righty - UpRight) / up) * length / 2;
                } else {
                    midx = (rightx + downx - righty + downy) / 2;
                    midy = (rightx - downx + righty + downy) / 2;
                }
            }
        } else if (Down == 1 && Right == 1) {
            int leftx = -1, lefty = 1280;
            int upx = -1, upy = -1;
            for (int i = 10; i < channel.rows - 10; i++)
                for (int j = 10; j < channel.cols - 10; j++)
                    if (channel.at<cv::Vec3b>(i, j)[0] < 30 && channel.at<cv::Vec3b>(i, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i, j)[2] < 30 && channel.at<cv::Vec3b>(i - 1, j)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j)[2] < 30 && channel.at<cv::Vec3b>(i - 2, j)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j)[2] < 30 && channel.at<cv::Vec3b>(i, j - 1)[0] < 30 &&
                        channel.at<cv::Vec3b>(i, j - 1)[1] < 30 && channel.at<cv::Vec3b>(i, j - 1)[2] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j - 1)[0] < 30 && channel.at<cv::Vec3b>(i - 1, j - 1)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j - 1)[2] < 30 && channel.at<cv::Vec3b>(i - 2, j - 1)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j - 1)[1] < 30 && channel.at<cv::Vec3b>(i - 2, j - 1)[2] < 30) {
                        if (i < RightUp && upx == -1) {
                            upx = i;
                            upy = j;
                        }
                        if (j < DownLeft && leftx == -1) {
                            leftx = i;
                            lefty = j;
                        }
                    }
            cout << "leftx: " << leftx << "  lefty: " << lefty << "  upx: " << upx << "  upy: " << upy << endl;
            if (leftx == -1) {
                if (upx == -1) {
                    midx = (RightUp + 720 + (1280 - DownLeft) * length / sqrt((720 - RightUp) * (720 - RightUp) +
                                                                              (1280 - DownLeft) * (1280 - DownLeft))) /
                           2;
                    midy = (DownLeft + 1280 + (720 - RightUp) * length / sqrt((720 - RightUp) * (720 - RightUp) +
                                                                              (1280 - DownLeft) * (1280 - DownLeft))) /
                           2;
                } else {
                    float left = sqrt((720 - upx) * (720 - upx) + (upy - DownLeft) * (upy - DownLeft));
                    float right = sqrt((1280 - upy) * (1280 - upy) + (upx - RightUp) * (upx - RightUp));
                    midx = upx + ((720 - upx) / left + (RightUp - upx) / right) * length / 2;
                    midy = upy + ((1280 - upy) / right - (upy - DownLeft) / left) * length / 2;
                }
            } else {
                if (upx == -1) {
                    float up = sqrt((1280 - lefty) * (1280 - lefty) + (RightUp - leftx) * (RightUp - leftx));
                    float down = sqrt((720 - leftx) * (720 - leftx) + (DownLeft - lefty) * (DownLeft - lefty));
                    midx = leftx + ((720 - leftx) / down - (RightUp - leftx) / up) * length / 2;
                    midy = lefty + ((1280 - lefty) / up - (DownLeft - lefty) / down) * length / 2;
                } else {
                    midx = (leftx + upx + upy - lefty) / 2;
                    midy = (leftx - upx + lefty + upy) / 2;
                }
            }
        } else if (Down == 1 && Left == 1) {
            int rightx = -1, righty = -1;
            int upx = -1, upy = -1;
            for (int i = 10; i < channel.rows - 10; i++)
                for (int j = 10; j < channel.cols - 10; j++)
                    if (channel.at<cv::Vec3b>(i, j)[0] < 30 && channel.at<cv::Vec3b>(i, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i, j)[2] < 30 && channel.at<cv::Vec3b>(i - 1, j)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j)[2] < 30 && channel.at<cv::Vec3b>(i - 2, j)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j)[2] < 30 && channel.at<cv::Vec3b>(i, j - 1)[0] < 30 &&
                        channel.at<cv::Vec3b>(i, j - 1)[1] < 30 && channel.at<cv::Vec3b>(i, j - 1)[2] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j - 1)[0] < 30 && channel.at<cv::Vec3b>(i - 1, j - 1)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j - 1)[2] < 30 && channel.at<cv::Vec3b>(i - 2, j - 1)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j - 1)[1] < 30 && channel.at<cv::Vec3b>(i - 2, j - 1)[2] < 30) {
                        if (i < LeftUp && upx == -1) {
                            upx = i;
                            upy = j;
                        }
                        if (j > DownRight && j > righty) {
                            rightx = i;
                            righty = j;
                        }
                    }
            cout << "rightx: " << rightx << "  righty: " << righty << "  upx: " << upx << "  upy: " << upy << endl;
            if (rightx == -1) {
                if (upx == -1) {
                    midx = (LeftUp + 720 +
                            DownRight * length / sqrt((720 - LeftUp) * (720 - LeftUp) + DownRight * DownRight)) / 2;
                    midy = (DownRight -
                            (720 - LeftUp) * length / sqrt((720 - LeftUp) * (720 - LeftUp) + DownRight * DownRight)) /
                           2;
                } else {
                    float left = sqrt((LeftUp - upx) * (LeftUp - upx) + upy * upy);
                    float right = sqrt((DownRight - upy) * (DownRight - upy) + (upx - 720) * (upx - 720));
                    midx = upx + ((720 - upx) / right + (LeftUp - upx) / left) * length / 2;
                    midy = upy + ((DownRight - upy) / right - upy / left) * length / 2;
                }
            } else {
                if (upx == -1) {
                    float up = sqrt(righty * righty + (LeftUp - rightx) * (LeftUp - rightx));
                    float down = sqrt((720 - rightx) * (720 - rightx) + (DownRight - righty) * (DownRight - righty));
                    midx = rightx + ((720 - rightx) / down - (rightx - LeftUp) / up) * length / 2;
                    midy = righty - (righty / up + (righty - DownRight) / down) * length / 2;
                } else {
                    midx = (rightx + upx + righty - upy) / 2;
                    midy = (upx - rightx + righty + upy) / 2;
                }

            }
        }
    } else if (Up + Down + Left + Right == 1) {
        if (Up == 1) {
            int leftx = -1, lefty = 1280;
            int rightx = -1, righty = -1;
            int downx = -1, downy = -1;
            for (int i = 0; i < channel.rows; i++) {
                for (int j = 0; j < channel.cols; j++)
                    if (channel.at<cv::Vec3b>(i, j)[0] < 30 && channel.at<cv::Vec3b>(i, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i, j)[2] < 30 && channel.at<cv::Vec3b>(i - 1, j)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j)[2] < 30 && channel.at<cv::Vec3b>(i - 2, j)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j)[2] < 30 && channel.at<cv::Vec3b>(i, j - 1)[0] < 30 &&
                        channel.at<cv::Vec3b>(i, j - 1)[1] < 30 && channel.at<cv::Vec3b>(i, j - 1)[2] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j - 1)[0] < 30 && channel.at<cv::Vec3b>(i - 1, j - 1)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j - 1)[2] < 30 && channel.at<cv::Vec3b>(i - 2, j - 1)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j - 1)[1] < 30 && channel.at<cv::Vec3b>(i - 2, j - 1)[2] < 30) {
                        if (j < UpLeft && j < lefty) {
                            leftx = i;
                            lefty = j;
                        }
                        if (j > UpRight && j > righty) {
                            rightx = i;
                            righty = j;
                        }
                        downx = i;
                        downy = j;
                    }
            }
            cout << "leftx: " << leftx << "  lefty: " << lefty << "  downx: " << downx << "  downy: " << downy
                 << " rightx: " << rightx << "  righty: " << righty << endl;
            if (leftx < 10) {
                if (rightx < 10) {
                    float left = sqrt(downx * downx + (downy - UpLeft) * (downy - UpLeft));
                    float right = sqrt(downx * downx + (downy - UpRight) * (downy - UpRight));
                    midx = downx - downx * length / 2 * (1 / left + 1 / right);
                    midy = downy + ((UpRight - downy) / right - (downy - UpLeft) / left) * length / 2;
                } else {
                    midx = (downx + rightx + downy - righty) / 2;
                    midy = (downy + righty + rightx - downx) / 2;
                }
            } else {
                if (rightx < 10) {
                    midx = (leftx + downx + lefty - downy) / 2;
                    midy = (lefty + downy + downx - leftx) / 2;
                } else {
                    midx = (leftx + rightx) / 2;
                    midy = (lefty + righty) / 2;
                }
            }
        } else if (Down == 1) {
            int leftx = -1, lefty = 1280;
            int rightx = -1, righty = -1;
            int upx = -1, upy = -1;
            for (int i = 0; i < channel.rows; i++) {
                for (int j = 0; j < channel.cols; j++)
                    if (channel.at<cv::Vec3b>(i, j)[0] < 30 && channel.at<cv::Vec3b>(i, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i, j)[2] < 30 && channel.at<cv::Vec3b>(i - 1, j)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j)[2] < 30 && channel.at<cv::Vec3b>(i - 2, j)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j)[2] < 30 && channel.at<cv::Vec3b>(i, j - 1)[0] < 30 &&
                        channel.at<cv::Vec3b>(i, j - 1)[1] < 30 && channel.at<cv::Vec3b>(i, j - 1)[2] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j - 1)[0] < 30 && channel.at<cv::Vec3b>(i - 1, j - 1)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j - 1)[2] < 30 && channel.at<cv::Vec3b>(i - 2, j - 1)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j - 1)[1] < 30 && channel.at<cv::Vec3b>(i - 2, j - 1)[2] < 30) {
                        if (j < UpLeft && j < lefty) {
                            leftx = i;
                            lefty = j;
                        }
                        if (j > UpRight && j > righty) {
                            rightx = i;
                            righty = j;
                        }
                        if (upx == -1) {
                            upx = i;
                            upy = j;
                        }
                    }
            }
            cout << "leftx: " << leftx << "  lefty: " << lefty << "  upx: " << upx << "  upy: " << upy << " rightx: "
                 << rightx << "  righty: " << righty << endl;
            if (leftx < 10) {
                if (rightx < 10) {
                    float left = sqrt((720 - upx) * (720 - upx) + (upy - DownLeft) * (upy - DownLeft));
                    float right = sqrt((720 - upx) * (720 - upx) + (upy - DownRight) * (upy - DownRight));
                    midx = upx + (720 - upx) * length / 2 * (1 / left + 1 / right);
                    midy = upy + ((DownRight - upy) / right - (upy - DownLeft) / left) * length / 2;
                } else {
                    midx = (upx + rightx + righty - upy) / 2;
                    midy = (upy + righty - rightx + upx) / 2;
                }
            } else {
                if (rightx < 10) {
                    midx = (leftx + upx + upy - lefty) / 2;
                    midy = (lefty + upy + leftx - upx) / 2;
                } else {
                    midx = (leftx + rightx) / 2;
                    midy = (lefty + righty) / 2;
                }
            }
        } else if (Left == 1) {
            int upx = -1, upy = -1;
            int rightx = -1, righty = -1;
            int downx = -1, downy = -1;
            for (int i = 0; i < channel.rows; i++) {
                for (int j = 0; j < channel.cols; j++)
                    if (channel.at<cv::Vec3b>(i, j)[0] < 30 && channel.at<cv::Vec3b>(i, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i, j)[2] < 30 && channel.at<cv::Vec3b>(i - 1, j)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j)[2] < 30 && channel.at<cv::Vec3b>(i - 2, j)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j)[2] < 30 && channel.at<cv::Vec3b>(i, j - 1)[0] < 30 &&
                        channel.at<cv::Vec3b>(i, j - 1)[1] < 30 && channel.at<cv::Vec3b>(i, j - 1)[2] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j - 1)[0] < 30 && channel.at<cv::Vec3b>(i - 1, j - 1)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j - 1)[2] < 30 && channel.at<cv::Vec3b>(i - 2, j - 1)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j - 1)[1] < 30 && channel.at<cv::Vec3b>(i - 2, j - 1)[2] < 30) {
                        if (i < LeftUp && upx == -1) {
                            upx = i;
                            upy = j;
                        }
                        if (i > LeftDown) {
                            downx = i;
                            downy = j;
                        }
                        if (j > righty) {
                            rightx = i;
                            righty = j;
                        }
                    }
            }
            cout << "upx: " << upx << "  upy: " << upy << "  downx: " << downx << "  downy: " << downy << " rightx: "
                 << rightx << "  righty: " << righty << endl;
            if (upx < 10) {
                if (downx < 10) {
                    float up = sqrt(righty * righty + (rightx - LeftUp) * (rightx - LeftUp));
                    float down = sqrt(righty * righty + (rightx - LeftDown) * (rightx - LeftDown));
                    midx = rightx + ((LeftDown - rightx) / down - (rightx - LeftUp) / up) * length / 2;
                    midy = righty - righty * length / 2 * (1 / up + 1 / down);
                } else {
                    midx = (downx + rightx - righty + downy) / 2;
                    midy = (downy + righty + rightx - downx) / 2;
                }
            } else {
                if (downx < 10) {
                    midx = (upx + rightx + righty - upy) / 2;
                    midy = (upy + righty - rightx + upx) / 2;
                } else {
                    midx = (upx + downx) / 2;
                    midy = (upy + downy) / 2;
                }
            }
        } else {
            int upx = -1, upy = -1;
            int leftx = -1, lefty = 1280;
            int downx = -1, downy = -1;
            for (int i = 0; i < channel.rows; i++) {
                for (int j = 0; j < channel.cols; j++)
                    if (channel.at<cv::Vec3b>(i, j)[0] < 30 && channel.at<cv::Vec3b>(i, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i, j)[2] < 30 && channel.at<cv::Vec3b>(i - 1, j)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j)[2] < 30 && channel.at<cv::Vec3b>(i - 2, j)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j)[2] < 30 && channel.at<cv::Vec3b>(i, j - 1)[0] < 30 &&
                        channel.at<cv::Vec3b>(i, j - 1)[1] < 30 && channel.at<cv::Vec3b>(i, j - 1)[2] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j - 1)[0] < 30 && channel.at<cv::Vec3b>(i - 1, j - 1)[1] < 30 &&
                        channel.at<cv::Vec3b>(i - 1, j - 1)[2] < 30 && channel.at<cv::Vec3b>(i - 2, j - 1)[0] < 30 &&
                        channel.at<cv::Vec3b>(i - 2, j - 1)[1] < 30 && channel.at<cv::Vec3b>(i - 2, j - 1)[2] < 30) {
                        if (i < LeftUp && upx == -1) {
                            upx = i;
                            upy = j;
                        }
                        if (i > LeftDown) {
                            downx = i;
                            downy = j;
                        }
                        if (j < lefty) {
                            leftx = i;
                            lefty = j;
                        }
                    }
            }
            cout << "leftx: " << leftx << "  lefty: " << lefty << "  downx: " << downx << "  downy: " << downy
                 << " upx: " << upx << "  upy: " << upy << endl;
            if (upx < 10) {
                if (downx < 10) {
                    float up = sqrt((1280 - lefty) * (1280 - lefty) + (leftx - RightUp) * (leftx - RightUp));
                    float down = sqrt((1280 - lefty) * (1280 - lefty) + (leftx - RightDown) * (leftx - RightDown));
                    midx = leftx + ((RightDown - leftx) / down - (leftx - RightUp) / up) * length / 2;
                    midy = lefty + (1280 - lefty) * length / 2 * (1 / up + 1 / down);
                } else {
                    midx = (downx + leftx - downy + lefty) / 2;
                    midy = (downy + lefty + downx - leftx) / 2;
                }
            } else {
                if (downx < 10) {
                    midx = (upx + leftx + upy - lefty) / 2;
                    midy = (upy + lefty + leftx - upx) / 2;
                } else {
                    midx = (upx + downx) / 2;
                    midy = (upy + downy) / 2;
                }
            }
        }
    } else {
        int startx = -1, starty = -1;
        int finishx = 0, finishy = 0;
        for (int i = 0; i < channel.rows; i++)
            for (int j = 0; j < channel.cols; j++)
                if (channel.at<cv::Vec3b>(i, j)[0] < 30 && channel.at<cv::Vec3b>(i, j)[1] < 30 &&
                    channel.at<cv::Vec3b>(i, j)[2] < 30 && channel.at<cv::Vec3b>(i - 1, j)[0] < 30 &&
                    channel.at<cv::Vec3b>(i - 1, j)[1] < 30 &&
                    channel.at<cv::Vec3b>(i - 1, j)[2] < 30 && channel.at<cv::Vec3b>(i - 2, j)[0] < 30 &&
                    channel.at<cv::Vec3b>(i - 2, j)[1] < 30 &&
                    channel.at<cv::Vec3b>(i - 2, j)[2] < 30 && channel.at<cv::Vec3b>(i, j - 1)[0] < 30 &&
                    channel.at<cv::Vec3b>(i, j - 1)[1] < 30 && channel.at<cv::Vec3b>(i, j - 1)[2] < 30 &&
                    channel.at<cv::Vec3b>(i - 1, j - 1)[0] < 30 && channel.at<cv::Vec3b>(i - 1, j - 1)[1] < 30 &&
                    channel.at<cv::Vec3b>(i - 1, j - 1)[2] < 30 && channel.at<cv::Vec3b>(i - 2, j - 1)[0] < 30 &&
                    channel.at<cv::Vec3b>(i - 2, j - 1)[1] < 30 && channel.at<cv::Vec3b>(i - 2, j - 1)[2] < 30) {
                    if (startx == -1) {
                        startx = i;
                        starty = j;
                    }
                    finishx = i;
                    finishy = j;
                }
        if (startx == -1) {
            cout << "未发现无人机" << endl;
            p.setX(-1);
            p.setY(-1);
            p.setZ(-1);
            QRCode_pos = p;
            simxSetFloatSignal(clientID, "QRcode_x", p.x(), simx_opmode_oneshot);
            simxSetFloatSignal(clientID, "QRcode_y", p.y(), simx_opmode_oneshot);
            simxSetFloatSignal(clientID, "QRcode_z", p.z(), simx_opmode_oneshot);
            return;
        } else {
            midx = (startx + finishx) / 2;
            midy = (starty + finishy) / 2;
        }
    }
    cout << midx << " " << midy << endl;
    offsetx = (position[2] - height) * rate / 1280 * (640 - midy);
    offsety = (position[2] - height) * rate / 1280 * (midx - 360);
    p.setX(position[0] + offsetx * cos(angle[2]) + offsety * sin(angle[2]));
    p.setY(position[1] + offsety * cos(angle[2]) - offsetx * sin(angle[2]));
    p.setZ(height);
    tar_position = utils::getObjectPosition(target, m_cid);
    cout << p.x() << " " << p.y() << "  " << tar_position[0] << " " << tar_position[1] << endl;
    QRCode_pos = p;
    simxSetFloatSignal(clientID, "QRcode_x", p.x(), simx_opmode_oneshot);
    simxSetFloatSignal(clientID, "QRcode_y", p.y(), simx_opmode_oneshot);
    simxSetFloatSignal(clientID, "QRcode_z", p.z(), simx_opmode_oneshot);
}