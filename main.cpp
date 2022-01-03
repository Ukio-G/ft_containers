#include <iostream>
#include <string>
#include <deque>

#include "ft_vector/vector.h"

#include <vector>
#include <algorithm>

class A {
public:
    A() : i(this), num(0) {
        std::cout << "DEFAULT" << std::endl;
    }

    A(const A& a) : i(this), num(a.num) {
        std::cout << "COPY" << std::endl;
    }

    A* i;
    int num;
};

template <typename T>
void print_vector_status(T & vec) {
    std::cout << "capacity: "  << vec.capacity() << std::endl;
    std::cout << "size: "  << vec.size() << std::endl;
    for (typename T::iterator it = vec.begin(); it != vec.end() ; it++) {
        std::cout << "[" << std::distance(vec.begin(), it) << "] "<< *it << std::endl;
    }
}

int main(int argc, char** argv) {
    ft::vector<int> vec;
    vec.reserve(10);

    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(4);
    vec.push_back(5);
    vec.push_back(6);
    vec.push_back(7);

    print_vector_status(vec);

    ft::vector<int>::iterator a = vec.begin() + 2;

    vec.insert(a, 3);
    vec.insert(vec.end(), 8);
    vec.insert(vec.end(), 9);

    print_vector_status(vec);

    return 0;
}
