#include <iostream>
#include <string>
#include <deque>

#include "ft_vector/vector.h"

#include <vector>

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
    std::cout << "capacity :"  << vec.capacity() << std::endl;
    std::cout << "size :"  << vec.capacity() << std::endl;
    for (typename T::iterator it = vec.begin(); it != vec.end() ; it++) {
        std::cout << "[" << std::distance(vec.begin(), it) << "] "<< *it << std::endl;
    }
}

int main(int argc, char** argv) {
    std::vector<int> sveci;
    sveci.push_back(1);
    print_vector_status(sveci);



    return 0;
}
