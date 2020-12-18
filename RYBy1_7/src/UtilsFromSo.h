#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <map>

namespace StackOverflow {

    template <typename Out>
    void split(const std::string& s, char delim, Out result) {
        std::istringstream iss(s);
        std::string item;
        while (std::getline(iss, item, delim)) {
            *result++ = item;
        }
    }

    std::vector<std::string> split(const std::string& s, char delim) {
        std::vector<std::string> elems;
        split(s, delim, std::back_inserter(elems));
        return elems;
    }
}

namespace Internal {
    static const map<string, int> mood_map = {
        { "meh", 1 },
        { "good", 2 },
        { "rad", 3 },
        { "bad", -1 },
        { "awful", -2 },
    };

    int mood_value(const string& mood) {
        auto value = mood_map.find(mood);
        if (value == mood_map.end()) return 0;
        return value->second;
    }
}