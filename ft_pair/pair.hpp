#ifndef PAIR_HPP
#define PAIR_HPP

namespace ft {
    template <class T1, class T2>
    struct pair {
        /* 1 */
        pair() { }

        /* 2 */
        pair( const T1& x, const T2& y ) : first(x), second(y) { }

        /* 4 */
        template< class U1, class U2 >
        pair( const pair<U1, U2>& p ) : first(p.first), second(p.second) { }

        /* 7 */
        pair(const pair & other) : first(other.first), second(other.second) { }

        pair& operator=( const pair& other ) {
            if (&other == this)
                return *this;
            first = other.first;
            second = other.second;
        }

        T1 first;
        T2 second;
    };

    template< class T1, class T2 >
    pair<T1, T2> make_pair( T1 t, T2 u ) {
        return pair<T1, T2>(t, u);
    }

    template< class T1, class T2 >
    bool operator==( const pair<T1, T2>& lhs,
                     const pair<T1, T2>& rhs ) {
        return (lhs.first == rhs.first && lhs.second == rhs.second);
    }

    template< class T1, class T2 >
    bool operator!=( const pair<T1, T2>& lhs,
                     const pair<T1, T2>& rhs ) {
        return !(lhs == rhs);
    }

    /*
     * From cppreference:
     * If lhs.first<rhs.first, returns true.
     * Otherwise, if rhs.first<lhs.first, returns false.
     * Otherwise, if lhs.second<rhs.second, returns true.
     * Otherwise, returns false.
     * */
    template< class T1, class T2 >
    bool operator<( const pair<T1, T2>& lhs,
                    const pair<T1, T2>& rhs ) {
        if (lhs.first < rhs.first) return true;
        if (rhs.first < lhs.first) return false;
        if (rhs.second < lhs.second) return true;
        return false;
    }

    template< class T1, class T2 >
    bool operator<=( const pair<T1, T2>& lhs,
                     const pair<T1, T2>& rhs ) {
        return (lhs < rhs) || (lhs == rhs);
    }

    template< class T1, class T2 >
    bool operator>( const pair<T1, T2>& lhs,
                    const pair<T1, T2>& rhs ) {
        return rhs < lhs;
    }

    template< class T1, class T2 >
    bool operator>=( const pair<T1, T2>& lhs,
                     const pair<T1, T2>& rhs ) {
        return !(lhs < rhs);
    }
    
}

#endif
