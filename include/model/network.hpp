#pragma once

#include <vector>


class Network
{
    std::vector<std::vector<int>> costs;
    int center;
    int edge_capacity;

public:
    Network(std::vector<std::vector<int>> costs, int center, int edge_capacity);
    ~Network();

    int getCenter() const;
    int getEdgeCapacity() const;
};