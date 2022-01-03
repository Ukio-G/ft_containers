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
    std::vector<int> sveci;

    for (int i = 0; i < 10; ++i) {
        sveci.push_back(i);
    }

    ft::vector<int> vec(sveci.begin(), sveci.end());
    ft::vector<int> vec2(5, 10);
    print_vector_status(vec);
    print_vector_status(vec2);
    return 0;
}
