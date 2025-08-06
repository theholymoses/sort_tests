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
    void shaker_sort_oneloop_uint64  (std::uint64_t*, std::size_t);
    void insertion_sort_uint64       (std::uint64_t*, std::size_t);
    void selection_sort_uint64       (std::uint64_t*, std::size_t);
    void gnome_sort_uint64           (std::uint64_t*, std::size_t);
}

template <class T>
void
get_algorithms_by_type(std::vector<s::algorithm*>& v)
{
    static std::vector<s::algorithm_generic> v_generic;

    static std::vector<s::algorithm_uint64>  v_uint64 =
    {
        s::algorithm_uint64("bubble sort",     "uint64",           bubble_sort_uint64),
        s::algorithm_uint64("shaker sort",     "uint64, one loop", shaker_sort_oneloop_uint64),
        s::algorithm_uint64("insertion sort",  "uint64",           insertion_sort_uint64),
        s::algorithm_uint64("selection sort",  "uint64",           selection_sort_uint64),
        s::algorithm_uint64("gnome sort",      "uint64",           gnome_sort_uint64)
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
run_tests_by_type(const char* type, std::vector<s::sort_report>& v_sr)
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
                    std::sort(test_array, test_array + n, [](T a, T b){ return (a > b); });
                    break;
                case s::array_state::SORTED_DESC:
                    std::sort(test_array, test_array + n, [](T a, T b){ return (a < b); });
                    break;
                default:
                    break;
            }

            try
            {
                v[i]->sort_test(static_cast<void*>(test_array), n);
                for (std::size_t i = 0; i < n - 1; ++i)
                {
                    if (test_array[i] > test_array[i + 1])
                    {
                        std::fprintf(stderr, "Erroneous sorting algorithm: %s\n", v[i]->get_name());
                        break;
                    }
                }

                s::sort_report sr(v[i], state, n, sizeof(T));
                v_sr.push_back(sr);
            }
            catch (const std::exception& ex)
            {
                std::fprintf(stderr, "%s : %s\n", ex.what(), v[i]->get_name());
            }
        }
    }
}

int
main()
{
    constexpr std::size_t array_length = 16;

    std::vector<s::sort_report> v_sr;
    run_tests_by_type<std::uint64_t, array_length>("uint64_t", v_sr);

    if (!v_sr.size())
    {
        std::fprintf(stderr, "No report produced.\n");
        std::exit(1);
    }

    v_sr[0].print_header();
    std::sort(&v_sr[0], &v_sr[0] + v_sr.size(), [](s::sort_report a, s::sort_report b){ return (a > b); });
    for (std::size_t i = 0; i < v_sr.size(); ++i)
    {
        v_sr[i].print_boarder();
        v_sr[i].print();
    }
}

