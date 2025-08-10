#ifndef SORT_TEST_HPP
#define SORT_TEST_HPP

#include <random>
#include <limits>
#include <stdexcept>
#include <ctime>

namespace sort_test
{

// --------------------------------------------------------------------------------
template <class T>
constexpr bool always_false = false;

template <class T>
inline T
random_value(T min = std::numeric_limits<T>::min(),
             T max = std::numeric_limits<T>::max())
{
    static std::random_device   rd;
    static std::mt19937_64      engine(rd());

    if      constexpr (std::is_integral<T>::value)
    {
        static std::uniform_int_distribution<T> distribution(min, max);
        return (distribution(engine));
    }
    else if constexpr (std::is_floating_point<T>::value)
    {
        static std::uniform_real_distribution<T> distribution(min, max);
        return (distribution(engine));
    }
    else
    {
        static_assert(always_false<T>, "unsupported type");
    }
}

// --------------------------------------------------------------------------------
enum class array_state
{
    START,

    SORTED_ASC,
    UNSORTED,
    SORTED_DESC,

    END
};

constexpr array_state
array_state_initial = array_state::SORTED_ASC;

constexpr array_state
array_state_final   = array_state::SORTED_DESC;

array_state&
operator++(array_state& state)
{
    switch (state)
    {
        case array_state::START:        return (state = array_state::SORTED_ASC);
        case array_state::SORTED_ASC:   return (state = array_state::UNSORTED);
        case array_state::UNSORTED:     return (state = array_state::SORTED_DESC);
        case array_state::SORTED_DESC:  return (state = array_state::END);
        default:                        return (state = array_state::START);
    }
}

const char*
array_state_to_str(const array_state& state)
{
    switch (state)
    {
        case array_state::SORTED_ASC:   return ("sorted");
        case array_state::UNSORTED:     return ("unsorted");
        case array_state::SORTED_DESC:  return ("sorted backwards");
        default:                        return ("unknown");
    }
}

// --------------------------------------------------------------------------------
extern "C"
{
    typedef int  (*f_cmp)       (const void* const a, const void* const b);
    typedef void (*f_generic)   (void* addr, std::size_t n, std::size_t size, f_cmp cmp);
    typedef void (*f_uint64)    (std::uint64_t* addr, std::size_t n);
}

inline double
timespec_human_readable(const struct timespec& b, const struct timespec& e)
{
    return ((e.tv_nsec - b.tv_nsec) / 1000000000.0 + (e.tv_sec - b.tv_sec));
}

class algorithm
{
protected:
    const char*     name        = nullptr;
    const char*     info        = nullptr;
    double          exec_time   = 0.0;

public:
    algorithm(const char* _name, const char* _info)
    :name(_name), info(_info)
    { }

    virtual const char*
    get_name() const { return (name); }

    virtual const char*
    get_info() const { return (info); }

    virtual double
    get_exec_time() const { return (exec_time); }

    virtual
    bool
    run_sort([[maybe_unused]]void *addr, [[maybe_unused]]std::size_t n, [[maybe_unused]]std::size_t size)
    { return (false); }

    bool
    sort_test(void *addr, std::size_t n, std::size_t size)
    {
        bool rv;
        struct timespec b, e;

        clock_gettime(CLOCK_MONOTONIC_RAW, &b);
        rv = run_sort(addr, n, size);
        clock_gettime(CLOCK_MONOTONIC_RAW, &e);

        exec_time = timespec_human_readable(b, e);

        return (rv);
    }

};

class algorithm_generic : public algorithm
{
    f_generic f = nullptr;
    f_cmp     c = nullptr;

public:
    algorithm_generic(const char* _name, const char* _info, f_generic _f, f_cmp _c)
    :algorithm(_name, _info), f(_f), c(_c)
    { }

    virtual
    bool
    run_sort(void* addr, std::size_t n, std::size_t size) override
    {
        if (!f || !c)
        {
            return (false);
        }
        f(addr, n, size, c);
        return (true);
    }
};

class algorithm_uint64 : public algorithm
{
    f_uint64 f = nullptr;

public:
    algorithm_uint64(const char* _name, const char* _info, f_uint64 _f)
    :algorithm(_name, _info), f(_f)
    { }

    virtual
    bool
    run_sort(void* addr, std::size_t n, [[maybe_unused]]std::size_t size) override
    {
        if (!f)
        {
            return (false);
        }
        f(static_cast<std::uint64_t*>(addr), n);
        return (true);
    }
};

// --------------------------------------------------------------------------------
struct sort_report
{
    const char*     name        = nullptr;
    const char*     info        = nullptr;
    const char*     st          = nullptr;
    array_state     state       = array_state::UNSORTED;
    std::size_t     n           = 0;
    std::size_t     size        = 0;
    double          exec_time   = 0.0;

    sort_report(const algorithm* a, array_state& _state, std::size_t _n, std::size_t _size)
    :state(_state), n(_n), size(_size)
    {
        name        = a->get_name();
        info        = a->get_info();
        exec_time   = a->get_exec_time();
        st          = array_state_to_str(state);
    }

    void
    print_boarder(char c)
    {
        for (std::size_t i = 0; i < 100; ++i)
        {
            putchar(c);
        }
        putchar('\n');
    }

    void
    print_header()
    {
        std::printf("| %-15s | %-25s | %-20s | %5s | %5s | %9s |\n",
                    "ALGORITHM NAME",
                    "ADDITIONAL INFO",
                    "ARRAY INITIAL STATE",
                    "N",
                    "SZ",
                    "EXEC T.");
    }

    void
    print()
    {
        std::printf("| %-15s | %-25s | %-20s | %5zu | %5zu | %9.6f |\n",
                       name,   info,   st,     n,     size,  exec_time);
    }

    bool
    operator==(const sort_report& other)
    {
        return (n == other.n && state == other.state && size == other.size && exec_time == other.exec_time);
    }

    bool
    operator>(const sort_report& other)
    {
        if (n > other.n)                    return (true);
        if (n < other.n)                    return (false);
        if (state > other.state)            return (true);
        if (state < other.state)            return (false);
        if (size > other.size)              return (true);
        if (size < other.size)              return (false);
        if (exec_time > other.exec_time)    return (false);
        if (exec_time < other.exec_time)    return (true);

        return (false);
    }

    bool
    operator!=(const sort_report& other)
    {
        return !(*this == other);
    }

    bool
    operator<(const sort_report& other)
    {
        return !(*this > other);
    }

    bool
    operator>=(const sort_report& other)
    {
        return (*this > other || *this == other);
    }

    bool
    operator<=(const sort_report& other)
    {
        return (*this < other || *this == other);
    }
};

}

#endif // SORT_TEST_HPP
