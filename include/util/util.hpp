#pragma once

#include <algorithm>
#include <string>
#include <istream>
#include <vector>
#include <unordered_map>

namespace MLCMST::util {

std::string read_stream(std::istream& stream);
std::vector<int> firstN(unsigned int N);
long clockMilliseconds();
double mean(const std::vector<double> &v);
double stdev(const std::vector<double>& v);


template <typename T>
void erase(std::vector<T>& v, T val);

template <typename T>
std::vector<std::vector<T>> break_up(unsigned int N, std::vector<T> vec);
template <typename T>
std::unordered_map<T, int> valueToIndex(const std::vector<T>& v);
template <typename V>
std::unordered_map<V, std::vector<int> > groupIndexesByValue(const std::vector<V>& v);
template <typename T>
std::vector<T> concat(const std::vector<std::vector<T>>& vectors);


template <typename T>
void erase(std::vector<T>& v, T val)
{
    v.erase(std::find(v.begin(), v.end(), val));
}

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

template <typename T>
std::unordered_map<T, int> valueToIndex(const std::vector<T>& v)
{
    std::unordered_map<T, int> mapping;
    for (int i=0; i<v.size(); i++) {
        mapping[v[i]] = i;
    }
    return mapping;
}

template <typename V>
std::unordered_map<V, std::vector<int> > groupIndexesByValue(const std::vector<V>& v)
{
    std::unordered_map<V, std::vector<int> > groups;
    for (int i=0; i < v.size(); i++) {
        groups[v[i]].push_back(i);
    }
    return groups;
}

template <typename T>
std::vector<T> concat(const std::vector<std::vector<T>>& vectors)
{
    std::vector<T> result;
    for (const std::vector<T>& v : vectors) {
        result.insert(result.end(), v.begin(), v.end());
    }
    return result;
}

}
