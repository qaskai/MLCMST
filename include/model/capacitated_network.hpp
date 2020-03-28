#pragma once

#include <vector>


class CapacitatedNetwork
{
    int edge_capacity;
    std::vector<std::vector<int>> costs;

public:
    CapacitatedNetwork(int edge_capacity, std::vector<std::vector<int>> costs);
    ~CapacitatedNetwork();

    int getEdgeCapacity() const;
};