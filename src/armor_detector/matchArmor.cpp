#include "detect.h"

inline float distance( cv::Point2f point1, cv::Point2f point2 ) {
    return sqrt(pow((point1.x-point2.x),2)+pow((point1.y-point2.y),2));
}


//匹配装甲板
void Armor_detector::match_armour(Light &l,Armour &a) {
    std::vector<std::pair<cv::RotatedRect,cv::RotatedRect>> really_armor;
    for ( auto & r : a.two_Light ) {
        float height1 = std::max ( r.first.size.height, r.first.size.width );
        float height2 = std::max ( r.second.size.height, r.second.size.width );
        cv::Point2f point1=r.first.center;
        cv::Point2f point2=r.second.center;
        float Distance = distance( point1, point2 );
        float ratio1 = Distance / height1;
        float ratio2 = Distance / height2;

        if( ratio1 < ( a.m_width / l.m_height +0.7 ) && ratio1 > 0 &&
            ratio2 < ( a.m_width/ l.m_height +0.7 ) && ratio2 > 0 && 
            Distance > 20 ) {
                really_armor.push_back( std::make_pair( r.first,r.second ));
        }
    }
    a.two_Light.clear();
    a.two_Light=really_armor;
}