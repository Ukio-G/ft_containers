#ifndef SHARED_PTR_HPP
#define SHARED_PTR_HPP

namespace ft {
    template <class T, class Allocator = std::allocator<T> >
    class shared_ptr {
    public:
        shared_ptr() : _data(0), _ref_counter(0) { }
        shared_ptr() : _data(0), _ref_counter(0) { }

    private:

        T * _data;
        size_t _ref_counter;
    };
}

#endif
