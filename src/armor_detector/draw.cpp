#include "detect.h"
#include <vector>
#include <thread>

std::vector<cv::Point2f> armor_points; 
int count = 0;

bool comparePoints(const cv::Point2f& p1, const cv::Point2f& p2) {
    return (p1.x+p1.y)<(p2.x+p2.y);
}

inline float distance( cv::Point2f point1, cv::Point2f point2 ) {
    return sqrt(pow((point1.x-point1.x),2)+pow((point2.y-point2.y),2));
}

void FrameTailor(cv::Mat frame) {
    // 原图上的四个点（需要根据实际情况调整）
    std::vector<cv::Point2f> dst_points = {
        cv::Point2f( 0, 0),   // 左上角
        cv::Point2f( 0, frame.rows/2),  // 左下角
        cv::Point2f(frame.cols/2, frame.rows/2),  // 右下角
        cv::Point2f(frame.cols/2, 0)  // 右上角
    };
    cv::Mat cut_frame;
    cv::Mat matrix = cv::getPerspectiveTransform(armor_points,dst_points);
    cv::warpPerspective( frame, cut_frame, matrix, frame.size()/2);
    armor_points.clear();
    std::string path_head = "/home/xzq/project/DataSet/frame/img";
    std::string num = std::to_string( count );
    std::string path_tail = ".jpg";
    std::string path_name = path_head + num +path_tail;
    if( !cv::imwrite( path_name, cut_frame) ) {
        std::cout << "error" << std::endl;
    }
    count++;
}

//绘制
void Armor_detector::draw_armor(cv::Mat &frame,Light &l,Armour &a) {
    //根据x+y的值排序 可以判断出八个点的位置
    for( auto & r : a.two_Light) {
        r.first.size.width *= 1.5;
        r.first.size.height *=2;
        cv::Point2f point_rect[4];
        r.first.points(point_rect);

        r.second.size.width *= 1.5;
        r.second.size.height *= 2;
        cv::Point2f point_rect2[4];
        r.second.points(point_rect2);

        std::vector<cv::Point2f> points(point_rect,point_rect+4 );
        std::vector<cv::Point2f> points2(point_rect2,point_rect2+4 );

        std::sort(points.begin(),points.end(),comparePoints);
        std::sort(points2.begin(),points2.end(),comparePoints);
        cv::Point2f point(50,50);
        if(points[0].x<points2[1].x) {
            std::vector<cv::Point> contour;
            contour.push_back( points[0] );
            contour.push_back( points2[1] );
            contour.push_back( points2[3] );
            contour.push_back( points[2] );
            float tan = std::max(abs( points[0].y - points2[1].y ) / abs( points[0].x - points2[1].x ) ,
            abs( points[0].y - points[2].y ) / abs( points[0].x - points[2].x ));
            if( cv::contourArea(contour) > 500 && tan > 1 ) {
                cv::line(frame,points[0],points2[1],cv::Scalar(0,255,0),2,8,0);
                cv::line(frame,points2[1],points2[3],cv::Scalar(0,255,0),2,8,0);
                cv::line(frame,points2[3],points[2],cv::Scalar(0,255,0),2,8,0);
                cv::line(frame,points[2],points[0],cv::Scalar(0,255,0),2,8,0);
                //特征点:左上 左下 右下 右上
                a.four_point.push_back( points[0] );
                a.four_point.push_back( points[2] );
                a.four_point.push_back( points2[3] );
                a.four_point.push_back( points2[1] );

                armor_points.push_back(points[0]);
                armor_points.push_back(points[2]);
                armor_points.push_back(points2[3]);
                armor_points.push_back(points2[1]);
                FrameTailor(frame);
            }
        }else {
            std::vector<cv::Point> contour;
            contour.push_back( points[0] );
            contour.push_back( points2[1] );
            contour.push_back( points2[3] );
            contour.push_back( points[2] );
            float tan = std::max(abs( points[1].y - points2[0].y ) / abs( points[1].x - points2[0].x ) ,
            abs( points2[2].y - points2[0].y ) / abs( points2[2].x - points2[0].x ));
            if( cv::contourArea(contour) > 500 && tan > 1 ) {
                cv::line(frame,points2[0],points[1],cv::Scalar(0,255,0),2,8,0);
                cv::line(frame,points[1],points[3],cv::Scalar(0,255,0),2,8,0);
                cv::line(frame,points[3],points2[2],cv::Scalar(0,255,0),2,8,0);
                cv::line(frame,points2[2],points2[0],cv::Scalar(0,255,0),2,8,0);
                //特征点:左上 左下 右下 右上
                a.four_point.push_back( points2[0]);
                a.four_point.push_back( points2[2] );
                a.four_point.push_back( points[3] );
                a.four_point.push_back( points[1] );

                armor_points.push_back(points2[0]);
                armor_points.push_back(points2[2]);
                armor_points.push_back(points[3]);
                armor_points.push_back(points[1]);
                FrameTailor(frame);
            }
        }
    }
}