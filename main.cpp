#include <iostream>
#include <string>
#include <deque>
#include <vector>
#include <stack>
#include <map>
#include "check_utils.hpp"

#ifdef __TEST__STD__ //CREATE A REAL STL EXAMPLE
namespace ft = std;
#else
    #include "map.hpp"
	#include "stack.hpp"
	#include "vector.hpp"
#endif

#include <stdlib.h>
#include <ctime>
#include <cassert>

#define MAX_RAM (4294967296 / 4)
#define BUFFER_SIZE 4096
struct Buffer
{
    int idx;
    char buff[BUFFER_SIZE];
};


class A {
public:
    A() : i(0) { }

    A(int i) : i (i) {}

    bool operator==(const A & other) const { return i == other.i; }
    bool operator!=(const A & other) const { return i != other.i; }

    int i;
};


#define COUNT (MAX_RAM / (int)sizeof(Buffer))

template<typename T>
class MutantStack : public ft::stack<T>
{
public:
    MutantStack() {}
    MutantStack(const MutantStack<T>& src) { *this = src; }
    MutantStack<T>& operator=(const MutantStack<T>& rhs)
    {
        this->c = rhs.c;
        return *this;
    }
    ~MutantStack() {}

    typedef typename ft::stack<T>::container_type::iterator iterator;

    iterator begin() { return this->c.begin(); }
    iterator end() { return this->c.end(); }
};


void vectors_test() {
    ft::vector<int> svec2(10, 5);

    ft::vector<int> vec1; // Never used

    ft::vector<int> vec2(svec2.begin(), svec2.end());
    ft::vector<int> vec3(vec2);
    ft::vector<int> vec4(vec2.begin(), vec2.end());
    ft::vector<int> vec5(vec2.begin(), vec2.end());

    assert(vectorEq(vec2, vec3));
    assert(vectorEq(vec2, vec4));
    assert(vectorEq(vec2, vec5));
    assert(vectorEq(vec4, vec5));
    assert(vectorEq(vec4, vec3));
    print_vector_status(vec2);

    /* Modify and reserve test */
    ft::vector<int> tmp(8);
    tmp[0] = 23;
    tmp[1] = 23;
    tmp[2] = 2;
    tmp[3] = 2;
    tmp[4] = 23;
    tmp[5] = 23;
    tmp[6] = 23;
    tmp[7] = 10;
    tmp.reserve(100500);

    vec5.assign(5, 23);
    vec5.push_back(10);
    vec5.insert(vec5.begin() + 2, 2, 2);
    assert(vectorEq(tmp, vec5));
    print_vector_status(vec5);
    print_vector_status(tmp);

    /* erase test */
    vec5.push_back(123456);
    vec5.push_back(123456);
    vec5.pop_back();
    vec5.erase(vec5.end() - 1);
    assert(vectorEq(tmp, vec5));
}

void map_test() {
    ft::map<int, A> smap;

    for (ft::map<int, A>::size_type i = 0; i < 100500; ++i) {
        smap[rand()] = rand();
    }

    /* Constructor test */
    ft::map<int, A> map1(smap.begin(), smap.end());
    ft::map<int, A> map2;
    assert(!mapEq(map1, map2));

    /* Modify test */
    map2.insert(map1.begin(), map1.end());
    assert(mapEq(map1, map2));
    map1.insert(ft::make_pair(123, 456));
    map2.insert(ft::make_pair(123, 456));

    /* Erase test */
    ft::map<int, A>::iterator m_it = map1.begin();
    for (ft::map<int, A>::size_type i = 0; i < 15; ++i) m_it++;

    map1.erase(map1.find(m_it->first));
    map2.erase(map2.find(m_it->first));

    assert(mapEq(map1, map2));
}

int main(int argc, char** argv) {
    if (argc != 2)
	{
		std::cerr << "Usage: ./test seed" << std::endl;
		std::cerr << "Provide a seed please" << std::endl;
		std::cerr << "Count value:" << COUNT << std::endl;
		return 1;
	}

    #ifdef __TEST__STD__
		std::cout << "STD version" << std::endl;
    #else
		std::cout << "FT version" << std::endl;
    #endif

	const int seed = atoi(argv[1]);
	srand(seed);

    ft::vector<std::string> vector_str;
    ft::vector<int> vector_int;
    ft::stack<int> stack_int;
    ft::vector<Buffer> vector_buffer;
    ft::stack<Buffer, std::deque<Buffer> > stack_deq_buffer;
    ft::map<int, int> map_int;

    for (int i = 0; i < COUNT; i++)
    {
        vector_buffer.push_back(Buffer());
    }

    for (int i = 0; i < COUNT; i++)
    {
        const int idx = rand() % COUNT;
        vector_buffer[idx].idx = 5;
    }
    ft::vector<Buffer>().swap(vector_buffer);

    try
    {
        for (int i = 0; i < COUNT; i++)
        {
            const int idx = rand() % COUNT;
            vector_buffer.at(idx);
            std::cerr << "Error: THIS VECTOR SHOULD BE EMPTY!!" <<std::endl;
        }
    }
    catch(const std::exception& e)
    {
        //NORMAL ! :P
    }

    for (int i = 0; i < COUNT; ++i)
    {
        map_int.insert(ft::make_pair(rand(), rand()));
    }

    int sum = 0;
    for (int i = 0; i < 10000; i++)
    {
        int access = rand();
        sum += map_int[access];
    }
    std::cout << "should be constant with the same seed: " << sum << std::endl;

    {
        ft::map<int, int> copy = map_int;
    }
    MutantStack<char> iterable_stack;
    for (char letter = 'a'; letter <= 'z'; letter++)
        iterable_stack.push(letter);
    for (MutantStack<char>::iterator it = iterable_stack.begin(); it != iterable_stack.end(); it++)
    {
        std::cout << *it;
    }
    std::cout << std::endl;

    vectors_test();
    map_test();

    return (0);
}