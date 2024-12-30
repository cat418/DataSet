#include "detect.h"

//构造函数
Armor_detector::Armor_detector(): m_cap(0) {
}

//读取图像
cv::Mat Armor_detector::read_frame() {
    cv::Mat frame;
    m_cap >> frame;
    if(frame.empty()) {
        std::cout<<"end"<<std::endl;
        return cv::Mat();
    }
    return frame;
}

//预处理
cv::Mat Armor_detector::findLight_deal_frame(cv::Mat frame,int limit) {
    //转化为灰度图
    cv::Mat gray; 
    cv::cvtColor(frame,gray,cv::COLOR_BGR2GRAY);
    //二值化图像
    cv::Mat binary;
    cv::threshold(gray,binary,limit,255,cv::THRESH_BINARY);

    //通道分离
    cv::Mat img[3];
    cv::split( frame, img);
    cv::Mat r_b = img[2] - img[0]; 
    //二值化
    cv::threshold( r_b, r_b,30,255,cv::THRESH_BINARY);
    //闭运算
    cv::Mat kernel = cv::getStructuringElement( 2, cv::Size(10,10));
    cv::morphologyEx( r_b, r_b, cv::MORPH_CLOSE, kernel);
    //膨胀
    cv::Mat kernel2 = cv::getStructuringElement( 2, cv::Size(3,3));
    cv::dilate( r_b, r_b, kernel2);
    //和上面二值化图取交集
    cv::Mat result;
    cv::bitwise_and( r_b,binary, result );
    cv::imshow("result",result);
    return result;
}

    //检测轮廓
    std::vector<std::vector<cv::Point>> Armor_detector::check_contours(cv::Mat preprocessing_frame) {
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(preprocessing_frame,contours,hierarchy,cv::RETR_TREE,cv::CHAIN_APPROX_SIMPLE);
    return contours;
}