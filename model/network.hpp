#pragma once

#include <vector>


class Network
{
    int center;
    int edge_capacity;
    std::vector<std::vector<int>> costs;

public:
    Network(std::vector<std::vector<int>> costs, int center, int edge_capacity);
    ~Network();

    int getCenter();
    int getEdgeCapacity();
};