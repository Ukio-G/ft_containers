#ifndef FT_CONTAINERS_FT_STACK_H
#define FT_CONTAINERS_FT_STACK_H

#include <deque>
#include "ft_meta_cpp/lexicographical_compare.hpp"

namespace ft {
    template <class T, class Container = std::deque<T> >
    class stack {
    public:
        typedef Container container_type;
        typedef typename Container::value_type value_type;
        typedef typename Container::size_type size_type;
        typedef typename Container::reference reference;
        typedef typename Container::const_reference const_reference;

        /* 2 */
        explicit stack( const Container& cont = Container() ) : c(cont) { }

        /* 4 */
        stack( const stack& other ) : c(other.c) { }

        ~stack() { }

        stack& operator=( const stack& other ) {
            if (&other == this)
                return *this;
            c = other.c;
        }

        reference top() {
            return const_cast<reference>(static_cast<const stack&>(*this).top());
        }

        const_reference top() const {
            return c.back();
        }

        bool empty() const {
            return c.size() == 0;
        }

        size_type size() const {
            return c.size();
        }

        void push( const value_type& value ) {
            c.push_back(value);
        }

        void pop() {
            c.pop_back();
        }


    protected:
        container_type c;
    };

    template< class T, class Container >
    bool operator==( const stack<T,Container>& lhs,
                     const stack<T,Container>& rhs ) {
        if (lhs.size() != rhs.size())
            return false;
        for (int i = 0; i < lhs.size(); ++i)
            if (lhs[i] != rhs[i])
                return false;
        return true;
    }

    template< class T, class Container >
    bool operator!=( const stack<T,Container>& lhs,
                     const stack<T,Container>& rhs ) {
        return !(lhs == rhs);
    }

    template< class T, class Container >
    bool operator<( const stack<T,Container>& lhs,
                    const stack<T,Container>& rhs ) {
        return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template< class T, class Container >
    bool operator<=( const stack<T,Container>& lhs,
                     const stack<T,Container>& rhs ) {
        return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()) || (lhs == rhs);
    }

    template< class T, class Container >
    bool operator>( const stack<T,Container>& lhs,
                    const stack<T,Container>& rhs ) {
        return ft::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end());
    }

    template< class T, class Container >
    bool operator>=( const stack<T,Container>& lhs,
                     const stack<T,Container>& rhs ) {
        return ft::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end()) || (lhs == rhs);
    }
    
}


#endif
