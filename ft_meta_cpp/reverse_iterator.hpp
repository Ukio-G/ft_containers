#ifndef REVERSE_ITERATOR_HPP
#define REVERSE_ITERATOR_HPP


template<typename Itr>
class reverse_iterator {
    Itr itr;
public:
    reverse_iterator(Itr itr): itr(itr) {}
    auto& operator*() {
        return *std::prev(itr); // <== returns the content of prev
    }
    constexpr auto& operator++() {
        --itr;
        return *this;
    }
    constexpr friend bool operator!=(reverse_iterator<Itr> a, reverse_iterator<Itr> b) {
        return a.itr != b.itr;
    }
};

#endif
