#ifndef WARGAMINGTASK_INSERTIONSORT_HPP
#define WARGAMINGTASK_INSERTIONSORT_HPP

#include <vector>

template<typename T>
void insertionSort(std::vector<T> &collection)
{
  for (std::size_t i = 1; i < collection.size(); ++i) {
    for (std::size_t j = i; (collection[j - 1] > collection[j]) && (j > 0); --j) {
      std::swap(collection[j - 1], collection[j]);
    }
  }
}

#endif //WARGAMINGTASK_INSERTIONSORT_HPP
