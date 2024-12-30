#include "detect.h"

float distance( cv::Point2f points1, cv::Point2f points2 ) {
    float two_point_distance = sqrt(pow(abs(points1.x - points2.x),2) - pow(abs(points1.y - points2.y),2));
    return two_point_distance;
}

//检测灯条 
void Armor_detector::detect_light(std::vector<std::vector<cv::Point>> contours,Light &l) { 
    //最小外接矩形拟合灯条
    std::vector<cv::RotatedRect> allRotatedRect;
    //根据面积筛选
    for(const auto & l : contours) {
        if( cv::contourArea( l ) < 800) {
            cv::RotatedRect rect = cv::minAreaRect(l);
            //根据旋转角度筛选
            if( rect.angle < 20 || rect.angle > 70 ) {
                allRotatedRect.push_back(rect);
            }
        }
    }
    l.light_rect = allRotatedRect;
}

