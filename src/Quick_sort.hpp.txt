#ifndef QUICK_SORT_HPP
#define QUICK_SORT_HPP

#include <algorithm>
#include <cstdlib>
#include <stdexcept>
#include <utility>
#include <vector>

template <typename Comparable>
void quick_sort_helper(std::vector<Comparable>& vec, int explorer, int frontier, int pivot){
    
    if (pivot - explorer <= 0) return;

    int next_start = explorer;
    int next_end = pivot;

    while (explorer != pivot){
        if (vec[explorer] <= vec[pivot]){
            std::swap(vec[++frontier], vec[explorer]);
        }
        
        ++explorer;
    }

    std::swap(vec[++frontier], vec[pivot]);

    quick_sort_helper(vec, next_start, next_start - 1, frontier - 1);
    quick_sort_helper(vec, frontier + 1, frontier, next_end);
    
}

template <typename Comparable>
void quick_sort(std::vector<Comparable>& vec){
    quick_sort_helper(vec, 0, -1, vec.size() - 1);

}



#endif