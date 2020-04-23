#pragma once

#include <string>
#include <istream>
#include <vector>

namespace MLCMST::util {

std::string read_stream(std::istream& stream);

template <typename T>
std::vector<std::vector<T>> break_up(int N, std::vector<T> vec);


// template implementation
template <typename T>
std::vector<std::vector<T>> break_up(int N, std::vector<T> vec)
{
    std::vector<std::vector<T>> out;
    int idx = 0;
    while (idx < vec.size()) {
        out.emplace_back();
        for (int i = 0; i < N && idx < vec.size(); i++) {
            out.back().push_back(vec[idx++]);
        }
    }
    return out;
}

}
