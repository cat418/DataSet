#include "detect.h"

//展示
void Armor_detector::show( Armour &a,Light l,int limit) {
    m_cap.open("4.mp4");
    if( !m_cap.isOpened() ) {
        std::cout << "视频打开失败" << std::endl;
        return;
    }
    while(true) {
        l.light_rect.clear();
        a.two_Light.clear();
        cv::Mat img = read_frame();
        if (img.empty()) {
            continue;
        }
        //预处理
        cv::Mat frame = findLight_deal_frame(img,limit);
        //如果处理后的图像为空，直接显示图像
        if (frame.empty()) {
            std::cout<<"frame是空的"<<std::endl;
            cv::imshow("armor1",img);
        }else{
        //检测轮廓
        std::vector<std::vector<cv::Point>> contours = check_contours(frame);
        //检测灯条 
        detect_light(contours,l);
        //匹配灯条
        match_light( frame, l, a );
        //匹配装甲板
        match_armour(l,a);
        //绘制
        draw_armor(img,l,a);
        cv::imshow("armor",img);
        a.four_point.clear();
        }
        int fps = m_cap.get(cv::CAP_PROP_FPS);
        cv::waitKey( 1000/fps );
    }
    return;
}

