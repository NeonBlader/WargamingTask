#ifndef WARGAMINGTASK_CIRCULARBUFFERARRAY_HPP
#define WARGAMINGTASK_CIRCULARBUFFERARRAY_HPP

#include <typeinfo>
#include <utility>
#include <stdexcept>

template <typename T>
class CircularBufferArray
{
public:
  explicit CircularBufferArray(std::size_t size);
  CircularBufferArray(const CircularBufferArray<T> &other);
  CircularBufferArray<T> &operator=(const CircularBufferArray<T> &other);
  ~CircularBufferArray();

  CircularBufferArray(CircularBufferArray<T> &&other) noexcept;
  CircularBufferArray<T> &operator=(CircularBufferArray<T> &&other) noexcept;

  void push(const T &addable) noexcept;
  void push(T &&addable) noexcept;
  void pop() noexcept;
  T& getTail() const noexcept;
  bool isEmpty() const noexcept;

private:
  CircularBufferArray();

  template<typename D>
  void perfectPushing(D &&addable);

  T *data_;
  T *head_;
  T *tail_;
  std::size_t size_;
  std::size_t capacity_;
};

template<typename T>
CircularBufferArray<T>::CircularBufferArray(): data_(nullptr),
    head_(nullptr),
    tail_(nullptr),
    size_(0),
    capacity_(0)
{}


template<typename T>
CircularBufferArray<T>::CircularBufferArray(std::size_t size): data_(new T[size]),
    head_(data_),
    tail_(data_),
    size_(0),
    capacity_(size)
{}

template<typename T>
CircularBufferArray<T>::CircularBufferArray(const CircularBufferArray<T> &other): data_(new T[other.maxElementsCount_]),
    head_(nullptr),
    tail_(nullptr),
    size_(other.size_),
    capacity_(other.capacity_)
{
  for (std::size_t i = 0; i < size_ ; ++i) {
    data_[i] = other.data_[i];
    if ((other.data_ + i) == other.head_) {
      head_ = (data_ + i);
    }
    if ((other.data_ + i) == other.tail_) {
      tail_ == (data_ + i);
    }
  }
}

template<typename T>
CircularBufferArray<T> &CircularBufferArray<T>::operator=(const CircularBufferArray<T> &other) {
  if (data_ == other.data_) {
    return *this;
  }
  CircularBufferArray<T> tmpBuffer(other);
  std::swap(*this, tmpBuffer);
}

template<typename T>
CircularBufferArray<T>::CircularBufferArray(CircularBufferArray<T> &&other) noexcept: CircularBufferArray() {
  std::swap(*this, other);
}

template<typename T>
CircularBufferArray<T> &CircularBufferArray<T>::operator=(CircularBufferArray<T> &&other) noexcept {
  if (data_ == other.data_) {
    return *this;
  }
  delete[] data_;
  std::swap(*this, other);
  other = CircularBufferArray<T>();
}

template<typename T>
CircularBufferArray<T>::~CircularBufferArray() {
  delete[] data_;
}

template<typename T>
void CircularBufferArray<T>::push(const T &addable) noexcept {
  perfectPushing(addable);
}

template<typename T>
void CircularBufferArray<T>::pop() noexcept {
  if (size_ == 0) {
    throw std::underflow_error("Buffer is empty!");
  }

  if (tail_ == (data_ + capacity_ - 1)) {
    tail_ = data_;
  } else {
    tail_++;
  }
  size_--;
}

template<typename T>
T& CircularBufferArray<T>::getTail() const noexcept {
  return tail_;
}

template<typename T>
void CircularBufferArray<T>::push(T &&addable) noexcept {
  perfectPushing(std::move(addable));
}

template<typename T>
template<typename D>
void CircularBufferArray<T>::perfectPushing(D &&addable) {
  if (size_ == capacity_) {
    throw std::overflow_error("Buffer is full!");
  }
  if ((head_ == tail_) && (size_ == 0)) {
    *head_ = std::forward<D>(addable);
  } else {
    if (head_ == (data_ + capacity_ - 1)) {
      head_ = data_;
    } else {
      head_++;
    }
  }
  *head_ = std::forward<D>(addable);
  size_++;
}

template<typename T>
bool CircularBufferArray<T>::isEmpty() const noexcept {
  return size_ == 0;
}

#endif //WARGAMINGTASK_CIRCULARBUFFERARRAY_HPP
