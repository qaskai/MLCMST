#pragma once


namespace MLCMST::geometry {

template <typename T>
struct Point final
{
    T x, y;

    Point();
    Point(T x, T y);
    ~Point();
    
    template <typename T1>
    friend bool operator==(const Point<T1>& p, const Point<T1>& q);
};


// *********** IMPLEMENTATION *********** //

template <typename T>
Point<T>::Point()
{

}

template <typename T>
Point<T>::Point(T x, T y) : x(x), y(y)
{

}


template <typename T>
Point<T>::~Point()
{

}

template <typename T>
bool operator==(const Point<T>& p, const Point<T>& q)
{
    return (p.x == q.x) && (p.y == q.y);
}


}
