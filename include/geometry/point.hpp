#pragma once


namespace MLCMST::geometry {

struct Point final
{
    double x, y;

    Point();
    Point(double x, double y);
    ~Point();
    
    friend bool operator==(const Point& p, const Point& q);
};

}
