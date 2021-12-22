#include <iostream>
#include <string>
#include <deque>

#include "ft_vector/vector.h"
#include "ft_meta_cpp/iterator_traits.hpp"
#include "ft_meta_cpp/enable_if.hpp"
#include "ft_meta_cpp/reverse_iterator.hpp"

#include <vector>
#include <map>

int main(int argc, char** argv) {
    ft::vector<int> v;
    ft::vector<int>::iterator it;

    std::vector<int> a;
    for (int i = 0; i < 10; ++i) {
        a.push_back(i);
    }

    std::vector<int>::iterator std_id(a.data());
}
