#include <geometry/util.hpp>


namespace MLCMST::geometry::util {

using std::vector;

vector<vector<double>> createDistanceMatrix(const vector<Point>& points)
{
    int N = points.size();
    vector<vector<double>> distances(N, vector<double>(N));
    for (int i=0; i<N; i++) {
        for (int j=0; j<N; j++) {
            distances[i][j] = euclidDistance(points[i], points[j]);
        }
    }
    return distances;
}

double euclidDistance(const Point& p, const Point& q) {
    double a = p.x - q.x;
    double b = p.y - q.y;
    return sqrt(a*a + b*b);
}

}
