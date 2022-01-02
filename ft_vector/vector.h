#ifndef FT_VECTOR_H
#define FT_VECTOR_H

#include <memory>
#include <cstddef>
#include <iterator_traits.hpp>
#include <stdexcept>
#include <sstream>
#include <limits>

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

            //  is incrementable
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

        explicit vector(size_type n, const T& value = T(), const Allocator& alloc = Allocator()) : _size(n), _capacity(n), _allocator(alloc) {
            allocate(n);
            for (int i = 0; i < n; ++i)
                _allocator.construct(_data + i, value);
        };

        template< class InputIt >
        vector( InputIt first, InputIt last, const Allocator& alloc = Allocator() ) : _allocator(alloc) {
            size_type elements_count = std::distance(first, last);
            allocate(elements_count);
            size_type _counter = 0;
            _size = _capacity = elements_count;
            for (InputIt it = first; first != last; it ++)
                _allocator.construct(_data + _counter++, *it);
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
            for (int i = 0; i < other.size(); ++i)
                _allocator.construct(_data + i, other[i]);
        }

        void assign(size_type count, const_reference value) {
            /* No need to reallocate memory - just wipe old data and
             * place value's copy to vector data
             */
            if (count <= _capacity) {
                wipeData();
                for (int i = 0; i < count; ++i)
                    _data[i] = T(value);
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

        iterator begin() {
            iterator result(_data);
            return result;
        }

        iterator end() {
            vector<T, Allocator>::iterator result(_data + _size);
            return result;
        }

        iterator rbegin() {
            iterator result(_data + _size);
            return result;
        }

        iterator rend() {
            iterator result(_data);
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
            reallocateToSize(new_cap);
        }

        size_type capacity() const { return _capacity; }

        /*
         * Modifiers
         */

        void clear() {
            wipeData();
        }

        iterator insert(iterator pos, const_reference value) {

        }

        void insert(iterator pos, size_type count, const_reference value) {

        }

        template< class InputIt > void insert(iterator pos, InputIt first, InputIt last) {

        }

        iterator erase(iterator pos);
        iterator erase(iterator first, iterator last);

        void push_back(const_reference value) {
            if (_size == _capacity)
                reallocateDouble();
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

        void resize(size_type count) {
            if (count < _size) {
                reallocateToSize(count);
                return;
            }

            size_type old_size = _size;
            reallocateToSize(count);
            T _default;
            for (size_type i = old_size ; i < count; ++i)
                _allocator.construct(_data + i, _default);
            _size = count;
            _capacity = count;
        }
        void resize(size_type count, value_type value = T());

        void swap(vector& other);
    private:
        size_t _size;
        size_t _capacity;
        std::allocator<T> _allocator;
        T* _data;

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

        /* Destruct all objects, set size to 0
         * Keep allocated memory, don't affect capacity
         * Don't modify content in the data memory
         * */
        void wipeData() {
            destructPlacementObjects();
            _size = 0;
        }

        void allocate(size_t element_count) {
            if (_data) {
                destructPlacementObjects();
                _allocator.deallocate(_data, _capacity);
            }
            _data = _allocator.allocate(element_count);
            _size = 0;
            _capacity = element_count;
        }

        void reallocateToSize(size_type element_count) {
            /* If element_count zero - just deallocate memory and destruct objects */
            if (element_count == 0 && _data) {
                destructPlacementObjects();
                _allocator.deallocate(_data, _capacity);
                _data = 0;
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

    using std::swap;

    template<typename T>
    void swap(typename vector<T>::iterator& a, typename vector<T>::iterator& b) {
        T tmp = *a;
        *b = tmp;
        *a = *b;
    }

    template <typename T> typename vector<T>::iterator operator+(typename vector<T>::iterator::difference_type n, typename vector<T>::iterator &it) {
        return it - n;
    }

    /*
 * Vector non member functions and operators declarations
 */
#define vector_non_member_operator(op) template <class T, class Alloc> bool operator op(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);
    vector_non_member_operator(==)
    vector_non_member_operator(!=)
    vector_non_member_operator(<)
    vector_non_member_operator(<=)
    vector_non_member_operator(>)
    vector_non_member_operator(>=)
#undef vector_non_member_operator
    template< class T, class Alloc > void swap( vector<T,Alloc>& lhs, vector<T,Alloc>& rhs );
}


#endif
