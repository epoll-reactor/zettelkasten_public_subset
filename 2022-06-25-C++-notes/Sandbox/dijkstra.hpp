// Aug 6, 2021
#ifndef SANDBOX_DIJKSTRA_HPP
#define SANDBOX_DIJKSTRA_HPP

#include <iostream>
#include <limits>
#include <array>
#include <memory>

class dijkstra_implementation
{
public:
    dijkstra_implementation(const size_t* array, size_t vertices)
        : array(array), vertices(vertices) {}

    bool valid_graph() noexcept
    {
        for (size_t i = 0; i < vertices; ++i) {
            for (size_t j = 0; j < vertices; ++j) {
                if (at_pointer(i, j) != at_pointer(j, i)) {
                    return false;
                }
            }
        }
        return true;
    }

    void compute(size_t src)
    {
        size_t dist[vertices];
        bool spt_set[vertices];

        for (size_t i = 0; i < vertices; ++i) {
            dist[i] = std::numeric_limits<int32_t>::max();
            spt_set[i] = false;
        }
        dist[src] = 0;

        auto min_distance = [this](const size_t* dist, const bool* spt_set) noexcept {
            size_t min = std::numeric_limits<size_t>::max();
            size_t min_index = 0;

            for (size_t i = 0; i < vertices; ++i) {
                if (!spt_set[i] && dist[i] <= min) {
                    min = dist[i];
                    min_index = i;
                }
            }
            return min_index;
        };

        for (size_t i = 0; i < vertices - 1; ++i) {
            size_t u = min_distance(dist, spt_set);
            spt_set[u] = true;
            for (size_t v = 0; v < vertices; ++v) {
                if (!spt_set[v]
                &&  at_pointer(u, v)
                &&  dist[u] != std::numeric_limits<size_t>::max()
                &&  dist[u] + at_pointer(u, v) < dist[v]
                ) {
                    dist[v] = dist[u] + at_pointer(u, v);
                }
            }
        }

        std::array assert_array{ 0, 4, 12, 19, 21, 11, 9, 8, 14 };

        for (size_t i = 0; i < vertices; ++i) {
            if (assert_array[i] != dist[i]) {
                using namespace std::string_literals;
                throw std::runtime_error(
                    "assertion "s + std::to_string(assert_array[i]) + " "s +
                    std::to_string(dist[i]) + " failed");
            }
        }
    }

private:
    size_t at_pointer(size_t x, size_t y) noexcept
    {
        return *(array + (vertices * y + x));
    }


    const size_t* array;
    const size_t vertices;
};

void dijkstra_test()
{
    std::string array = R"__(
        0,  4,  0,  0,  0,  0,  0,  8,  0,
        4,  0,  8,  0,  0,  0,  0, 11,  0,
        0,  8,  0,  7,  0,  4,  0,  0,  2,
        0,  0,  7,  0,  9, 14,  0,  0,  0,
        0,  0,  0,  9,  0, 10,  0,  0,  0,
        0,  0,  4, 14, 10,  0,  2,  0,  0,
        0,  0,  0,  0,  0,  2,  0,  1,  6,
        8, 11,  0,  0,  0,  0,  1,  0,  7,
        0,  0,  2,  0,  0,  0,  6,  7,  0
    )__";
    const size_t vertices = 9;
    std::unique_ptr<size_t[]> graph(new size_t[vertices * vertices]);

    dijkstra_implementation impl(graph.get(), vertices);
    if (!impl.valid_graph()) {
        std::cerr << "Invalid graph\n";
        return;
    }
    impl.compute(0);
}

#endif //SANDBOX_DIJKSTRA_HPP
