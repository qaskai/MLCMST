#pragma once

#include <string>
#include <istream>
#include <vector>

namespace MLCMST::util {

std::string read_stream(std::istream& stream);
std::vector<int> firstN(unsigned int N);

template <typename T>
std::vector<std::vector<T>> break_up(unsigned int N, std::vector<T> vec);


// template function implementation
template <typename T>
std::vector<std::vector<T>> break_up(unsigned int N, std::vector<T> vec)
{
    std::vector<std::vector<T>> out;
    unsigned int idx = 0;
    while (idx < vec.size()) {
        out.emplace_back();
        for (unsigned int i = 0; i < N && idx < vec.size(); i++) {
            out.back().push_back(vec[idx++]);
        }
    }
    return out;
}

}
