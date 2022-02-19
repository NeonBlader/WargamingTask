#ifndef WARGAMINGTASK_QUICKSORT_HPP
#define WARGAMINGTASK_QUICKSORT_HPP

#include <vector>
#include <random>

#include "insertionSort.hpp"

template<typename T>
void quickSort(std::vector<T> &collection, std::size_t start, std::size_t finish)
{
  if (finish <= start) {
    return;
  }

  std::random_device rd;
  std::mt19937 mt(rd());
  std::vector<decltype(mt())> rands;
  for (int i = 0; i < 3; ++i) {
    rands.push_back((mt() % (finish - start) + 1) + start);
  }
  insertionSort(rands);
  std::swap(collection[finish], collection[rands[1]]);

  std::size_t undefinedBorder = start;
  std::size_t lessMoreBorder = start;
  for (std::size_t i = start; i <= finish; ++i) {
    if (i == finish) {
      std::swap(collection[i], collection[lessMoreBorder]);
      break;
    }
    if (collection[undefinedBorder] > collection[finish]) {
      undefinedBorder++;
    } else {
      std::swap(collection[lessMoreBorder], collection[i]);
      lessMoreBorder++;
      undefinedBorder++;
    }
  }

  quickSort(collection, start, lessMoreBorder - 1);
  quickSort(collection, lessMoreBorder + 1, finish);
}

#endif //WARGAMINGTASK_QUICKSORT_HPP
