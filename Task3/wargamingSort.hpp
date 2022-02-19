#ifndef WARGAMINGTASK_WARGAMINGSORT_HPP
#define WARGAMINGTASK_WARGAMINGSORT_HPP

#include <vector>

#include "insertionSort.hpp"
#include "quickSort.hpp"

const int MAX_ELEMENTS_FOR_INSERTION_SORT = 16;

template <typename T>
void wargamingSort(std::vector<T> &collection)
{
  if (collection.size() < MAX_ELEMENTS_FOR_INSERTION_SORT) {
    insertionSort(collection);
  } else {
    quickSort(collection, 0, (collection.size() - 1));
  }
}

#endif //WARGAMINGTASK_WARGAMINGSORT_HPP
