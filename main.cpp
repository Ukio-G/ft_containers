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
    std::vector<A> veca;
    std::cout << veca.data() << std::endl;
    A a;

    std::vector<A> vecb(10, a);

    veca = vecb;

    ft::vector<int> vecc;

    for (int i = 0; i < 10; ++i) {
        veca[i].num = i;
        vecc.push_back(veca[i].num);
    }

    for (int i = 0; i < 10; ++i) {
        std::cout << vecc.at(i) << " " << vecc.at(i) << std::endl;
    }

    print_vector_status(vecc);
    std::cout << "iterator size = " << sizeof(ft::vector<int>::iterator) << " std::iterator " << sizeof (std::vector<int>::iterator ) << std::endl;
    return 0;
}
