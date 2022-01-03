#ifndef FT_VECTOR_H
#define FT_VECTOR_H

#include <memory>
#include <cstddef>
#include <iterator_traits.hpp>
#include "ft_meta_cpp/type_traits.hpp"
#include <stdexcept>
#include <sstream>
#include <limits>
#include <algorithm>
#include "ft_meta_cpp/reverse_iterator.hpp"
#include "ft_meta_cpp/lexicographical_compare.hpp"

namespace ft {


    template <typename T, typename Allocator = std::allocator<T> >
    class vector;

    template <typename T, typename Allocator>
    class vector {
    public:
        typedef T value_type;
        typedef Allocator allocator_type;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T& reference;
        typedef const T& const_reference;
        typedef typename Allocator::pointer  pointer;
        typedef const typename Allocator::pointer  const_pointer;

    class iterator {
        public:
            typedef T value_type;
            typedef std::ptrdiff_t difference_type;
            typedef T* pointer;
            typedef T& reference;
            typedef std::random_access_iterator_tag iterator_category;
            /* C++ named requirements: LegacyIterator */

            //  * C++ named requirements: CopyConstructible
            iterator(const iterator & other) : m_data_pointer(other.m_data_pointer) { }

            //  * C++ named requirements: CopyAssignable
            iterator& operator=(const iterator& other) {
                if (&other == this)
                    return *this;
                m_data_pointer = other.m_data_pointer;
                return *this;
            }

            // is dereferenceable
            T& operator*() {
                return *m_data_pointer;
            }

            //  is incrementable
            iterator& operator++() {
                m_data_pointer++;
                return *this;
            }

            iterator operator++(int) {
                iterator &result = *this;
                m_data_pointer++;
                return result;
            }

            /* C++ named requirements: LegacyInputIterator */

            // C++ named requirements: EqualityComparable
            bool operator==(const iterator & other) {
                return m_data_pointer == other.m_data_pointer;
            }

            bool operator!=(const iterator & other) {
                return m_data_pointer != other.m_data_pointer;
            }

            /* LegacyForwardIterator */

            // C++ named requirements: DefaultConstructible
            iterator() : m_data_pointer(0) {}

            /* C++ named requirements: LegacyBidirectionalIterator */
            iterator& operator--() {
                m_data_pointer--;
                return *this;
            }

            iterator operator--(int) {
                iterator &result = *this;
                m_data_pointer--;
                return result;
            }

            /* C++ named requirements: LegacyRandomAccessIterator */
            iterator& operator+=(difference_type n) {
                m_data_pointer += n;
                return *this;
            }

            iterator& operator-=(difference_type n) {
                m_data_pointer -= n;
                return *this;
            }

            iterator operator+(difference_type n) {
                iterator result  = *this;
                result.m_data_pointer += n;
                return result;
            }

            iterator operator-(difference_type n) {
                iterator result  = *this;
                result.m_data_pointer -= n;
                return result;
            }

            difference_type operator-(const iterator & other) {
                return (m_data_pointer - other.m_data_pointer);
            }

            T& operator[](int i) {
                return m_data_pointer[i];
            }

            // The below rules imply that LegacyRandomAccessIterator also implements LessThanComparable.
            bool operator<(const iterator &other) {
                return m_data_pointer < other.m_data_pointer;
            }

            bool operator>(const iterator &other) {
                return m_data_pointer > other.m_data_pointer;
            }

            bool operator>=(const iterator &other) {
                return m_data_pointer >= other.m_data_pointer;
            }

            bool operator<=(const iterator &other) {
                return m_data_pointer <= other.m_data_pointer;
            }
            //  * C++ named requirements: Destructible
            ~iterator() { }

            iterator(T* data_ptr) : m_data_pointer(data_ptr) { }

        private:
            T* m_data_pointer;
        };

        vector() : _size(0), _capacity(0), _allocator(), _data(0) { };

        explicit vector( const Allocator& alloc ) : _size(0), _capacity(0), _allocator(alloc), _data(0)  { }

        template< class InputIt >
        vector( InputIt first, InputIt last, const Allocator& alloc = Allocator() ) : _size(0), _capacity(0), _allocator(alloc), _data(0) {
            typedef typename is_integer<InputIt>::type Integer;
            constructor_dispatch(first, last, Integer());
        }

        vector( const vector& other ) : _size(other._size), _capacity(other._capacity), _allocator(other._allocator), _data(0) {
            allocate(other._capacity);
            for (int i = 0; i < other._size; ++i)
                _allocator.construct(_data + i, other[i]);
        }

        ~vector() {
            reallocateToSize(0);
        }

        vector<T, Allocator>& operator=(const vector& other) {
            if (&other == this)
                return *this;
            reallocateToSize(0);
            allocate(other._capacity);
            _size = other._size;
            _capacity = other._capacity;
            for (int i = 0; i < other._size; ++i)
                _allocator.construct(_data + i, other[i]);
        }

        void assign(size_type count, const_reference value) {
            /* No need to reallocate memory - just wipe old data and
             * place value's copy to vector data
             */
            if (count <= _capacity) {
                wipeData();
                for (int i = 0; i < count; ++i)
                    _allocator.construct(_data + i, value);
                _size = count;
                return;
            }

            allocate(count);

            for (int i = 0; i < count; ++i)
                _allocator.construct(_data + i, value);
            _size = count;
        }

        allocator_type get_allocator() {
            return _allocator;
        };

        /*
         * Element access
         */

        reference at(size_type pos) {
            if (pos >= _size) {
                std::string error_msg = generateOutOfRangeStr(pos, _size);
                throw std::out_of_range(error_msg);
            }
            return _data[pos];
        }

        const reference at(size_type pos) const {
            if (pos >= _size) {
                std::string error_msg = generateOutOfRangeStr(pos, _size);
                throw std::out_of_range(error_msg);
            }
            return _data[pos];
        }

        reference operator[](size_type pos) { return _data[pos]; }
        const_reference operator[](size_type pos) const { return _data[pos]; }

        reference front() { return _data[0]; }
        const_reference front() const { return _data[0]; }

        reference back() { return _data[_size - 1]; }
        const_reference back() const { return _data[_size - 1]; }

        T* data() { return _data; }
        const T* data() const { return _data; }

        /*
         * Iterators
         */

        iterator begin() const {
            iterator result(_data);
            return result;
        }

        iterator end() const {
            iterator result(_data + _size);
            return result;
        }

        ft::reverse_iterator<iterator> rbegin() const {
            reverse_iterator<iterator> result = end() - 1;
            return result;
        }

        ft::reverse_iterator<iterator> rend() const {
            reverse_iterator<iterator> result = begin() - 1;
            return result;
        }

        /*
         * Capacity
         */

        bool empty() const { return _size == 0; }

        size_type size() const { return _size; }

        size_type max_size() const {
            return std::numeric_limits<difference_type>::max();
        }

        void reserve(size_type new_cap) {
            if (new_cap <= _capacity)
                return;
            size_type need_to_append = new_cap - _capacity;
            appendMemoryIfNeededForElements(need_to_append);
        }

        size_type capacity() const { return _capacity; }

        /*
         * Modifiers
         */

        void clear() {
            wipeData();
        }

        iterator insert(iterator pos, const_reference value) {
            size_type offset = pos - begin();
            push_back(value);
            std::rotate(begin() + offset, end() - 1, end());
            return begin() + offset;
        }

        template< class InputIt > void insert(iterator pos, InputIt first, InputIt last) {
            typedef typename is_integer<InputIt>::type Integral;
            insert_dispatch(pos, first, last, Integral());
        }

        iterator erase(iterator pos) {
            size_type offset = pos - begin();
            std::rotate(pos, pos + 1, end());
            _allocator.destroy(_data + _size - 1);
            _size--;
            return begin() + offset;
        }

        iterator erase(iterator first, iterator last) {
            iterator result = 0;
            if (first == last)
                return first;
            size_type elements_count = std::distance(first, last);
            size_type offset = first - begin();
            for (iterator it = first; it != last; it++)
                _allocator.destroy(&(*it));
            std::rotate(first, last, end());
            _size -= elements_count;
            return begin() + offset;
        }

        void push_back(const_reference value) {
            appendMemoryIfNeededForElements(1);
            _allocator.construct(_data + _size, value);
            _size++;
        }

        void pop_back() {
            if (_size <= 0) {
                return;
            }
            _allocator.destroy(_data[_size - 1]);
            _size--;
        }

        /**
         * Resizes the container to contain count elements.
         * If the current size is greater than count, the container is reduced to its first count elements.
         * If the current size is less than count.
         * @param count
         */
        void resize(size_type count, T value = T()) {
            if (count == _size)
                return;

            if (count < _size) {
                reallocateToSize(count);
                return;
            }

            size_type old_size = _size;
            appendMemoryIfNeededForElements(count - _size);
            for (size_type i = old_size ; i < count; ++i)
                _allocator.construct(_data + i, value);
            _size = count;
            _capacity = count;
        }

        void swap(vector& other) {
            std::swap(_data, other._data);
            std::swap(_capacity, other._capacity);
            std::swap(_size, other._size);
            std::swap(_allocator, other._allocator);
        }
    private:
        size_t _size;
        size_t _capacity;
        std::allocator<T> _allocator;
        T* _data;

        template<typename Integer> void constructor_dispatch(Integer count, Integer value, true_type)
        { fill_construct(count, value); }

        template<typename InputIt> void constructor_dispatch(InputIt start, InputIt end, false_type)
        { range_construct(start, end); }

        void fill_construct(size_type count, const_reference value) {
            fill_insert(begin(), count, value);
        }

        template<class InputIt>
        void range_construct(InputIt start, InputIt end) {
            range_insert(begin(), start, end);
        }

        template<typename Integer, class It> void insert_dispatch(It pos, Integer n, Integer val, true_type)
        { fill_insert(pos, n, val); }

        template<typename InputIt, class It> void insert_dispatch(It pos, InputIt first, InputIt last, false_type)
        { range_insert(pos, first, last); }

        void fill_insert(iterator pos, size_type count, const_reference value) {
            size_type offset = pos - begin();
            appendMemoryIfNeededForElements(count);

            T* placement_data = _data + _size;
            for (int i = 0; i < count; ++i)
                _allocator.construct(placement_data + i, value);
            _size += count;

            std::rotate(begin() + offset, end() - 1, end());
        }

        template<class InputIt>
        void range_insert(iterator pos, InputIt first, InputIt last) {
            size_type elements_count = std::distance(first, last);
            size_type offset = pos - begin();
            appendMemoryIfNeededForElements(elements_count);

            T* placement_data = _data + _size;
            size_type counter = 0;
            for (InputIt it = first; it != last; it++)
                _allocator.construct(placement_data + counter++, *it);
            _size += elements_count;

            std::rotate(begin() + offset, end() - elements_count, end());
        }


        std::string generateOutOfRangeStr(size_type pos, size_type size) {
            std::string _size_str;
            std::string pos_str;
            std::stringstream ss;

            ss << size;
            ss >> _size_str;
            ss.clear();
            ss << pos;
            ss >> pos_str;

            std::string result = "Error: vector size: " + _size_str + ", but try get " + pos_str + " element.";

            return result;
        }

        /**
         * Destruct all objects, set size to 0
         * Keep allocated memory, don't affect capacity
         * Don't modify content in the data memory
         */
        void wipeData() {
            destructPlacementObjects();
            _size = 0;
        }

        /**
         * Call this before add elements to vector
         * Check, what we can do this - otherwise allocate/reallocate
         * memory
         *
         * @param element_count - check that we can place this count of object to the vector
         */
        void appendMemoryIfNeededForElements(size_type element_count) {
            size_type _available = _capacity - _size;

            if (!_data) {
                allocate(element_count);
                return;
            }

            if (element_count > _available) {
                if (element_count == 1)
                    reallocateDouble();
                else
                    reallocateToSize(_capacity + (element_count - _available));
            }
        }

        /**
         * Allocate new memory and destroy all old objects.
         * Doesn't reallocate contained objects to allocated memory.
         * Doesn't construct default objects on new allocated memory.
         *
         * @param element_count - allocate memory enough for element_count elements.
         */
        void allocate(size_t element_count) {
            /* If have data - destroy objects and free memory */
            if (_data) {
                destructPlacementObjects();
                _allocator.deallocate(_data, _capacity);
            }

            /* Allocate new memory */
            _data = _allocator.allocate(element_count);

            /* Update size to zero - no elements created yet */
            _size = 0;

            /* Update capacity to element_count */
            _capacity = element_count;
        }

        void reallocateToSize(size_type element_count) {
            /* If element_count zero - just deallocate memory and destruct objects */
            if (element_count == 0 && _data) {
                destructPlacementObjects();
                _allocator.deallocate(_data, _capacity);
                _data = 0;
                _capacity = _size = 0;
                return;
            }

            /* Reallocation doesn't work with uninitialized data! */
            if (!_data)
                throw std::runtime_error("Cannot reallocate vector from zero pointer!");

            /* Allocate new data */
            T* new_data = static_cast<T*> (_allocator.allocate(element_count));

            /* Determinate count elements to copy (possible case - passed to function
             * element_count less, than _size)
             */
            size_type copy_elements = std::min(element_count, _size);

            /* Copy old elements to new allocated memory */
            for (int i = 0; i < copy_elements; ++i)
                _allocator.construct(new_data + i, _data[i]);

            /* Destruct old elements */
            destructPlacementObjects();

            /* Deallocate old memory */
            _allocator.deallocate(_data, _capacity);

            /* Update capacity based on new allocated memory */
            _capacity = element_count;

            /* Update size based on copied elements */
            _size = copy_elements;

            /* Replace pointer in vector to new data */
            _data = new_data;
        }

        void reallocateDouble() {
            reallocateToSize(_capacity * 2);
        }

        void destructPlacementObjects() {
            for (int i = 0; i < _size; ++i)
                _allocator.destroy(_data + i);
        }
    };

    /*
     * Vector non member functions and operators
     */

    template<typename T>
    void swap(vector<T>& a,vector<T>& b) {
        a.swap(b);
    }

    template <typename T> typename vector<T>::iterator operator+(typename vector<T>::iterator::difference_type n, typename vector<T>::iterator &it) {
        return it - n;
    }

    template< class T, class Alloc >
    bool operator==( const vector<T,Alloc>& lhs,
                     const vector<T,Alloc>& rhs ) {
        if (lhs.size() != rhs.size())
            return false;
        for (int i = 0; i < lhs.size(); ++i)
            if (lhs[i] != rhs[i])
                return false;
        return true;
    }

    template< class T, class Alloc >
    bool operator!=( const vector<T,Alloc>& lhs,
                     const vector<T,Alloc>& rhs ) {
        return !(lhs == rhs);
    }

    template< class T, class Alloc >
    bool operator<( const vector<T,Alloc>& lhs,
                    const vector<T,Alloc>& rhs ) {
        return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template< class T, class Alloc >
    bool operator<=( const vector<T,Alloc>& lhs,
                    const vector<T,Alloc>& rhs ) {
        return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()) || (lhs == rhs);
    }

    template< class T, class Alloc >
    bool operator>( const vector<T,Alloc>& lhs,
                    const vector<T,Alloc>& rhs ) {
        return ft::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end());
    }

    template< class T, class Alloc >
    bool operator>=( const vector<T,Alloc>& lhs,
                    const vector<T,Alloc>& rhs ) {
        return ft::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end()) || (lhs == rhs);
    }
}

namespace std {
    template<typename T, class Alloc>
    void swap(ft::vector<T, Alloc> & a,ft::vector<T, Alloc> & b) {
        a.swap(b);
    }
}

#endif
