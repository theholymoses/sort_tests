#include <cstdio>
#include <cstring>
#include <vector>
#include <array>
#include <algorithm>
#include "sort_test.hpp"

namespace s = sort_test;

// --------------------------------------------------------------------------------
extern "C"
{
    void bubble_sort_uint64          (std::uint64_t*, std::size_t);
    void shaker_sort_uint64          (std::uint64_t*, std::size_t);
    void shaker_sort_oneloop_uint64  (std::uint64_t*, std::size_t);
    void insertion_sort_uint64       (std::uint64_t*, std::size_t);
    void selection_sort_uint64       (std::uint64_t*, std::size_t);
    void gnome_sort_uint64           (std::uint64_t*, std::size_t);
    void odd_even_sort_uint64        (std::uint64_t*, std::size_t);
    void cycle_sort_uint64           (std::uint64_t*, std::size_t);
}

template <class T>
void
get_algorithms_by_type(std::vector<s::algorithm*>& v)
{
    static std::vector<s::algorithm_generic> v_generic;

    static std::vector<s::algorithm_uint64>  v_uint64 =
    {
        s::algorithm_uint64("bubble sort",     "uint64",           bubble_sort_uint64),
        s::algorithm_uint64("shaker sort",     "uint64",           shaker_sort_uint64),
        s::algorithm_uint64("shaker sort",     "uint64, one loop", shaker_sort_oneloop_uint64),
        s::algorithm_uint64("insertion sort",  "uint64",           insertion_sort_uint64),
        s::algorithm_uint64("selection sort",  "uint64",           selection_sort_uint64),
        s::algorithm_uint64("gnome sort",      "uint64",           gnome_sort_uint64),
        s::algorithm_uint64("odd even sort",   "uint64",           odd_even_sort_uint64),
        s::algorithm_uint64("cycle sort",      "uint64",           cycle_sort_uint64)
    };

    for (std::size_t i = 0; i < v_generic.size(); ++i)
    {
        v.push_back(&v_generic[i]);
    }

    if constexpr (std::is_same<std::uint64_t, T>::value)
    {
        for (std::size_t i = 0; i < v_uint64.size(); ++i)
        {
            v.push_back(&v_uint64[i]);
        }
    }
}

template <class T, std::size_t n>
void
run_tests_by_type_and_length(const char* type, std::vector<s::sort_report>& v_sr)
{
    alignas(64) static T main_array[n];
    alignas(64) static T test_array[n];

    std::vector<s::algorithm*> v;
    get_algorithms_by_type<T>(v);

    if (!v.size())
    {
        std::fprintf(stderr, "No algorithms for %s type.\n", type);
        return;
    }

    for (std::size_t i = 0; i < n; ++i)
    {
        main_array[i] = s::random_value<T>();
    }

    for (std::size_t i = 0; i < v.size(); ++i)
    {
        for (s::array_state state = s::array_state_initial; state <= s::array_state_final; ++state)
        {
            std::memcpy(test_array, main_array, n);

            switch (state)
            {
                case s::array_state::SORTED_ASC:
                    std::sort(test_array, test_array + n, [](T a, T b){ return (a < b); });
                    break;
                case s::array_state::SORTED_DESC:
                    std::sort(test_array, test_array + n, [](T a, T b){ return (a > b); });
                    break;
                default:
                    break;
            }

            if (!v[i]->sort_test(static_cast<void*>(test_array), n, sizeof(T)))
            {
                std::fprintf(stderr, "No sorting algorithm provided.\n");
                std::exit(1);
            }

            for (std::size_t i = 0; i < n - 1; ++i)
            {
                if (test_array[i] > test_array[i + 1])
                {
                    std::fprintf(stderr, "Erroneous sorting algorithm: %s\n", v[i]->get_name());
                    std::exit(1);
                }
            }

            s::sort_report sr(v[i], state, n, sizeof(T));
            v_sr.push_back(sr);
        }
    }
}

int
main()
{
    std::vector<s::sort_report> v_sr;

    constexpr std::size_t len_large  = (1 << 16);

    run_tests_by_type_and_length<std::uint64_t, len_large> ("uint64_t", v_sr);

    if (!v_sr.size())
    {
        std::fprintf(stderr, "No report produced.\n");
    }
    else
    {
        std::sort(&v_sr[0], &v_sr[0] + v_sr.size(), [](s::sort_report a, s::sort_report b){ return (a > b); });

        v_sr[0].print_boarder('-');
        v_sr[0].print_header();

        for (std::size_t i = 0; i < v_sr.size(); ++i)
        {
            if (i > 0 && v_sr[i - 1].state != v_sr[i].state)
            {
                v_sr[i].print_boarder('=');
            }
            else
            {
                v_sr[i].print_boarder('-');
            }
            v_sr[i].print();
        }
        v_sr[0].print_boarder('-');
    }
}

