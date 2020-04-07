#pragma once


namespace MLCMST {
namespace geometry {

template <typename T>
struct Point final
{
    T x, y;

    Point();
    Point(T x, T y);
    ~Point();
    
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

}
}
