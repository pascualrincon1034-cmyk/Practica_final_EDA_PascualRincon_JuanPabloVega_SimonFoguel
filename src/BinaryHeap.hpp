#ifndef BINARY_HEAP_HPP
#define BINARY_HEAP_HPP

#include <algorithm>
#include <cstdlib>
#include <stdexcept>
#include <utility>
#include <vector>

template <typename Comparable>
class Binary_heap {

private:

    std::vector<Comparable> array;

    void heapify() {
        heapify(array.size());
    }

    void heapify(int n) {
        int start_index = (n / 2) - 1;

        for (int i = start_index; i >= 0; --i) {
            heapify_helper(n, i);
        }
    }

    void heapify_helper(int my_size, int n) {
        int largest = n;

        int child1 = child(n, 1);
        int child2 = child(n, 2);

        if (child1 < my_size && array[child1] > array[largest]) {
            largest = child1;
        }

        if (child2 < my_size && array[child2] > array[largest]) {
            largest = child2;
        }

        if (largest != n) {
            std::swap(array[n], array[largest]);
            heapify_helper(my_size, largest);
        }
    }

    int child(int n, int child_number) {
        return 2 * n + child_number;
    }

    int parent(int n) {
        return (n - 1) / 2;
    }

public:

    Binary_heap()
        : array{} {}

    Binary_heap(const std::vector<Comparable>& v)
        : array{v} {
        heapify();
    }

    Binary_heap(std::vector<Comparable>&& v)
        : array{std::move(v)} {
        heapify();
    }

    Binary_heap(int n)
        : array{} {
        array.reserve(n);
    }

    int size() const {
        return array.size();
    }

    std::vector<Comparable> sort_and_retrieve() {
        
        for (int i = size() - 1; i > 0; --i){
            std::swap(array[0], array[i]);
            heapify_helper(i, 0);
        }
        
        return std::move(array);    
    }
    
};

#endif