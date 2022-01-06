#ifndef SHARED_PTR_HPP
#define SHARED_PTR_HPP

namespace ft {
    template <class T, class Allocator = std::allocator<T> >
    class shared_ptr {
    public:
        shared_ptr() : _data(0), _ref_counter(0) { }

        shared_ptr(T * target, Allocator & allocator ) : _allocator(allocator) {
            _data = target;
            _ref_counter = new size_t;
            *_ref_counter = 1;
        }

        shared_ptr(const shared_ptr & other) : _data(other->_data), _ref_counter(other._ref_counter) {
            (*_ref_counter)++;
        }

        ~shared_ptr() {
            (*_ref_counter)--;
            if (_ref_counter == 0) {
                _allocator.destruct(_data);
                _allocator.deallocate(_data, 1);
            }
        }

    private:
        Allocator & _allocator;
        T * _data;
        size_t * _ref_counter;
    };
}

#endif
