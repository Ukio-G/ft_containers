#ifndef CHECK_UTILS_HPP
#define CHECK_UTILS_HPP

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

template<typename T, typename TT>
bool vectorEq(T & vec1, TT & vec2) {
    if (vec1.size() != vec2.size())
        return false;
    for (int i = 0; i < vec1.size(); ++i)
        if (vec1[i] != vec2[i]) return false;
    return true;
}

template<typename T, typename TT>
bool mapEq(T & map1, TT & map2) {
    if (map1.size() != map2.size())
        return false;
    for (typename T::iterator it = map1.begin(); it != map1.end(); it++) {
        typename T::iterator find_it = map2.find(it->first);
        if (find_it == map2.end() || (find_it->second) != (it->second) )
            return false;
    }
    return true;
}

#endif
