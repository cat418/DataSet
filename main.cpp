#include "detect.h"

void test( float light_height,float armor_width,float armor_height,float limit) {
    Armour a;
    a.m_width=armor_width;
    a.m_height=armor_height;
    Light l;
    l.m_height=light_height;

    Armor_detector ArmorDetector;
    ArmorDetector.show( a,l,limit);
}

int main()
{
    test( 5.5,13.04,12,150);
    return 0;
}