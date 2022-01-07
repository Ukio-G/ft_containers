#include <iostream>
#include <string>
#include <deque>

#include "vector.hpp"

#include <vector>
#include <algorithm>
#include "ft_pair/pair.hpp"
#include <map.hpp>
#include <map>
#include "stack.hpp"


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


std::ostream & operator<<(std::ostream & a, A& b) {
    a << b.i << " " << b.num;
    return a;
}

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

template <typename T>
void print_map_status(T & map) {
    std::cout << "size: " << map.size() << std::endl;
    for (typename T::iterator it = map.begin(); it != map.end() ; it++) {
        std::cout << "[" << it->first << "] " << it->second << std::endl;
    }
}

template <typename T>
void print_stack_status(T & stack) {
    std::cout << "stack status" << std::endl;
    std::cout << "size: " << stack.size() << std::endl;
    while (stack.size() > 0) {
        std::cout << stack.top() << std::endl;
        stack.pop();
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
typedef ft::vector<int>::reverse_iterator RIt;
typedef ft::map<int, std::string>::reverse_iterator RMIt;
typedef ft::map<int, std::string>::iterator MIt;
typedef std::vector<int>::iterator SIt;


int main(int argc, char** argv) {
    ft::map<int, std::string> sdasamap;

    sdasamap[0] = "sada";
    sdasamap[1] = "fsdg";
    sdasamap[2] = "dg";
    sdasamap[3] = "sfg";
    sdasamap[4] = "1234";
    sdasamap[1] = "1sfgf";
    sdasamap[1] = "1adsfgsfg234";


    print_map_status(sdasamap);

    ft::vector<int> veci;

    for (int i = 0; i < 20; i+=2) {
        veci.push_back(i);
    }

    ft::stack<int, ft::vector<int> > ftstack(veci);

    print_stack_status(ftstack);


    return 0;
}
