#include <model/geometry/point.hpp>

namespace MLCMST {
namespace model {
namespace geometry {

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
}
