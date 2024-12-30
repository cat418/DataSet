#include "detect.h"

//匹配灯条 必须重复匹配，因为根据角度匹配的可能是灯条和环境中的杂物匹配的
void Armor_detector::match_light( cv::Mat frame, Light &l, Armour &a) {
    //将灯条的矩形两两配对
    for( int i =0; i < l.light_rect.size(); i ++ ) {
        cv::RotatedRect rect1 = l.light_rect[i];
        for( int j = i+1; j < l.light_rect.size(); j ++) {
        cv::RotatedRect rect2 = l.light_rect[j];
        //根据平行匹配灯条
        float angle_diff = abs(rect1.angle - rect2.angle);
        if( angle_diff > 180.0 ) {
            angle_diff = 360.0 - angle_diff;
        }
        if( angle_diff > 45 ) {
            angle_diff = 90 - angle_diff;
        }
        
        //根据面积匹配 
        float area1 = l.light_rect[i].size.height * l.light_rect[i].size.width ;
        float area2 = l.light_rect[j].size.height * l.light_rect[j].size.width ;

        //根据灯条中心连线的tan值匹配
        float tan = abs(l.light_rect[i].center.y - l.light_rect[j].center.y) / abs(l.light_rect[i].center.x - l.light_rect[j].center.x);
        //根据相邻直线之间的角度
        cv::Point2f points[4];
        float tan1 = abs(l.light_rect[i].center.y - l.light_rect[j].center.y) / abs(l.light_rect[i].center.x - l.light_rect[j].center.x);
        if( angle_diff < 20 && abs(area1 - area2) < 300 && tan < 0.5 && tan > -1) {
                a.two_Light.push_back( std::make_pair( rect1, rect2 ) );
            }
        }
    }
}