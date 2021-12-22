#ifndef FT_VECTOR_H
#define FT_VECTOR_H

#include <memory>
#include <cstddef>
#include <iterator_traits.hpp>

namespace ft {


    template <typename T, typename Allocator = std::allocator<T> >
    class vector;



    template <class T> struct iterator_traits<vector<T> >
    {
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef T* pointer;
        typedef T& reference;
        typedef random_access_iterator_tag iterator_category;
    };



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
            typedef T value_type;
            typedef ptrdiff_t difference_type;
            typedef T* pointer;
            typedef T& reference;
            typedef random_access_iterator_tag iterator_category;

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

            //  * C++ named requirements: Destructible
            ~iterator() { }

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
            bool operator==(iterator & other) {
                return true;
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
                return (m_data_pointer - other.m_data_pointer) / sizeof(T);
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

            iterator(T* data_ptr) : m_data_pointer(data_ptr) { }


        private:
            T* m_data_pointer;
        };

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
        size_t _size;
        size_t _capacity;
        std::allocator<T> _allocator;

        void allocate(size_t element_count) {
            if (_data)
                delete [] _data;
            _data = static_cast<T*> (operator new [] (sizeof(T) * element_count));
            _size = 0;
            _capacity = element_count;
        }

        void reallocate() {
            /* Allocate new data */
            T* new_data = static_cast<T*> (_allocator.allocate(_capacity * 2));

            /* Copy old elements to new allocated memory */
            for (int i = 0; i < _size; ++i)
                _allocator.construct(new_data + i, _data[i]);

            /* Destruct old elements */
            destructPlacementObjects();

            /* Deallocate old memory */
            _allocator.deallocate(_capacity);

            /* Update size and capacity based on new allocated memory */
            _capacity *= 2;
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
