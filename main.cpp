#include <iostream>
#include <string>
#include <deque>

#include "ft_vector/vector.h"

#include <vector>
#include <algorithm>
#include "ft_pair/pair.hpp"
#include <map.h>
#include <map>



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

class BVal {
public:
    BVal() : i(0) {
        std::cout << __func__ << std::endl;
    }

    BVal(int i_) : i(i_) {
        std::cout << __func__ << std::endl;
    }

    BVal(const BVal & other) : i(other.i) {
        std::cout << "copy " << __func__ << std::endl;
    }

    ~BVal() {
        std::cout << __func__ << " " << i << std::endl;
    }

    int i;
};

template <class T, class TT, class Allocator = std::allocator<ft::pair<T,TT> > >
class B {
public:
    B(const ft::pair<T,TT> & p) : allocator() {
        std::cout << "1" << std::endl;
        set_value(p);
        std::cout << "2" << std::endl;
    }

    void set_value(const ft::pair<T,TT> & p) {
        std::cout << "3" << std::endl;
        data.~pair();
        allocator.construct(&data, p);
        std::cout << "4" << std::endl;
    }

    ~B() {
        std::cout << "5" << std::endl;
    }

private:
    ft::pair<T, TT> data;
    Allocator allocator;
};

template <typename T>
void print_vector_status(T & vec) {
    std::cout << "capacity: "  << vec.capacity() << std::endl;
    std::cout << "size: "  << vec.size() << std::endl;
    for (typename T::iterator it = vec.begin(); it != vec.end() ; it++) {
        std::cout << "[" << std::distance(vec.begin(), it) << "] "<< *it << std::endl;
    }
}

template<class T, class TT>
bool vec_eq(T & v1, TT & v2) {
    if (v1.size() != v2.size())
        return false;
    for (int i = 0; i < v1.size(); ++i)
        if (v1[i] != v2[i])
            return false;
    return true;
}

typedef ft::vector<int>::iterator It;
typedef std::vector<int>::iterator SIt;

int main(int argc, char** argv) {
    std::vector<int> sveci;

    for (int i = 0; i < 10; ++i)
        sveci.push_back(i);
    ft::vector<int> vec(sveci.begin(), sveci.end());


    print_vector_status(vec);

    It vi = vec.erase(vec.begin(), vec.begin());
    SIt svi = sveci.erase(sveci.begin(), sveci.begin());
    if (!vec_eq(vec, sveci)) {
        std::cout << "Failed" << std::endl;
    } else {
        std::cout << "Ok" << std::endl;
    }

    if (std::distance(vec.begin(), vi) != std::distance(sveci.begin(), svi)) {
        std::cout << "Failed" << std::endl;
    } else {
        std::cout << "Ok" << std::endl;
    }

    std::map<int, int> asf;

    ft::map<int, std::string> a;

    a[0] = "121233";
    a[1] = "123";
    a[2] = "56";
    a[3] = "sdfs";
    a[4] = "g642f";
    a[5] = "12aaffc";
    a[5] = "12";


    for (int i = 0; i < 6; ++i) {
        std::cout << a[i] << std::endl;
    }

    return 0;
}
