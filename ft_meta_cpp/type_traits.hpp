#ifndef IS_INTEGER_HPP
#define IS_INTEGER_HPP

struct true_type { char a; };
struct false_type { char a[2]; };

template<typename, typename>
struct are_same
{
    enum { __value = 0 };
    typedef false_type type;
};

template<typename T>
struct are_same<T, T>
{
    enum { value = 1 };
    typedef true_type type;
};


template<typename T>
struct is_integer
{
    enum { value = 0 };
    typedef false_type type;
};

template<>
struct is_integer<bool>
{
    enum { value = 1 };
    typedef true_type type;
};

template<>
struct is_integer<char>
{
    enum { value = 1 };
    typedef true_type type;
};

template<>
struct is_integer<signed char>
{
    enum { value = 1 };
    typedef true_type type;
};

template<>
struct is_integer<unsigned char>
{
    enum { value = 1 };
    typedef true_type type;
};

template<>
struct is_integer<short>
{
    enum { value = 1 };
    typedef true_type type;
};


template<>
struct is_integer<unsigned short>
{
    enum { value = 1 };
    typedef true_type type;
};

template<>
struct is_integer<int>
{
    enum { value = 1 };
    typedef true_type type;
};


template<>
struct is_integer<unsigned int>
{
    enum { value = 1 };
    typedef true_type type;
};

template<>
struct is_integer<long>
{
    enum { value = 1 };
    typedef true_type type;
};


template<>
struct is_integer<unsigned long>
{
    enum { value = 1 };
    typedef true_type type;
};

template<>
struct is_integer<long long>
{
    enum { value = 1 };
    typedef true_type type;
};


template<>
struct is_integer<unsigned long long>
{
    enum { value = 1 };
    typedef true_type type;
};


template<typename T>
struct is_pointer
{
    enum { value = 0 };
    typedef false_type type;
};

template<typename T>
struct is_pointer<T*>
{
    enum { value = 1 };
    typedef true_type type;
};

#endif //IS_INTEGER_HPP
