#ifndef FT_VECTOR_H
#define FT_VECTOR_H

#include <memory>
namespace ft {
    template <typename T, typename Allocator = std::allocator<T> >
    class vector {
    public:

        class iterator {
        public:
            typename ft::vector<T>::iterator& operator+(int offset);
            typename ft::vector<T>::iterator& operator-(int offset);
            T& operator*();

        private:
        };

        typedef T value_type;
        typedef Allocator allocator_type;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T& reference;
        typedef const T& const_reference;
        typedef typename Allocator::pointer  pointer;
        typedef const typename Allocator::pointer  const_pointer;

        vector();
        ~vector();

        vector<T, Allocator>& operator=(const vector& other);

        void assign(size_type count, const_reference value);

        allocator_type get_allocator() const;

        /*
         * Element access
         */

        reference at(size_type pos);
        const reference at(size_type pos) const;

        reference operator[](size_type pos);
        const_reference operator[](size_type pos) const;

        reference front();
        const_reference front() const;

        reference back();
        const_reference back() const;

        T* data();
        const T* data() const;

        /*
         * Iterators
         */

        iterator begin();
        iterator end();
        iterator rbegin();
        iterator rend();

        /*
         * Capacity
         */

        bool empty() const;

        size_type size() const;

        size_type max_size() const;

        void reserve(size_type new_cap);

        size_type capacity() const;

        /*
         * Modifiers
         */

        void clear();

        iterator insert(iterator pos, const_reference value);
        void insert(iterator pos, size_type count, const_reference value);
        template< class InputIt > void insert(iterator pos, InputIt first, InputIt last);

        iterator erase(iterator pos);
        iterator erase(iterator first, iterator last);

        void push_back(const_reference value );

        void pop_back();

        void resize(size_type count);
        void resize(size_type count, value_type value = T());

        void swap(vector& other);
    private:
        T* _data;

    };
}

template <typename T> typename ft::vector<T>::iterator& operator-(int, typename ft::vector<T>::iterator &it);
template <typename T> typename ft::vector<T>::iterator& operator+(int, typename ft::vector<T>::iterator &it);

/*
 * Vector non member functions and operators declarations
 */
#define vector_non_member_operator(op) template <class T, class Alloc> bool operator op(const ft::vector<T, Alloc>& lhs, const ft::vector<T, Alloc>& rhs);
vector_non_member_operator(==)
vector_non_member_operator(!=)
vector_non_member_operator(<)
vector_non_member_operator(<=)
vector_non_member_operator(>)
vector_non_member_operator(>=)
#undef vector_non_member_operator
template< class T, class Alloc > void swap( ft::vector<T,Alloc>& lhs, ft::vector<T,Alloc>& rhs );

#endif
