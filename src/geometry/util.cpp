#include <geometry/util.hpp>


namespace MLCMST {
namespace geometry {
namespace util {

using std::vector;

vector<vector<double>> createDistanceMatrix(const vector<Point<double>>& points)
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

}
}
}