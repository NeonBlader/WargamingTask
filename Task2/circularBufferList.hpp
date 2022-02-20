#ifndef WARGAMINGTASK_CIRCULARBUFFERLIST_HPP
#define WARGAMINGTASK_CIRCULARBUFFERLIST_HPP

#include <vector>
#include <stdexcept>

template <typename T>
class CircularBufferList
{
public:
  explicit CircularBufferList(std::size_t size);
  CircularBufferList(const CircularBufferList<T> &other);
  CircularBufferList(CircularBufferList<T> &&other) noexcept;
  ~CircularBufferList();

  CircularBufferList<T> &operator=(const CircularBufferList<T> &other);
  CircularBufferList<T> &operator=(CircularBufferList<T> &&other) noexcept;

  void push(const T& addable);
  void push(T&& addable);
  void pop();
  T& getTail() const noexcept;
  void resize(std::size_t newSize);
  bool isEmpty() const noexcept;

private:
  struct Node {
    explicit Node (T key): key_(key),
        next_(nullptr)
    {}
    Node(): key_(),
        next_(nullptr)
    {}

    T key_;
    Node * next_;
  };

  Node *head_;
  Node *tail_;
  std::size_t size_;
  std::size_t capacity_;

  CircularBufferList();

  template<typename D>
  void perfectPushing(D &&addable);
};

template<typename T>
CircularBufferList<T>::CircularBufferList(std::size_t size): head_(new Node),
    tail_(head_),
    size_(0),
    capacity_(size)
{
  Node *current = head_;
  for (std::size_t i = 0; i < size; ++i) {
    if (i == (size - 1)) {
      current->next_ = head_;
      break;
    }
    current->next_ = new Node;
    current = current->next_;
  }
}

template<typename T>
CircularBufferList<T>::CircularBufferList(const CircularBufferList<T> &other): head_(nullptr),
    tail_(nullptr),
    size_(other.size_),
    capacity_(other.capacity_)
{
  tail_ = new Node(other.tail_->key_);
  Node *current = tail_;
  Node *oCurrent = other.tail_;
  for (std::size_t i = 0; i < capacity_; ++i) {
    if (i == (capacity_ - 1)) {
      current->next_ = head_;
      break;
    }
    if (i < size_) {
      current->next_ = new Node(oCurrent->next_->key_);
    } else {
      current->next_ = new Node;
    }
    current = current->next_;
    oCurrent = oCurrent->next_;
  }
}

template<typename T>
CircularBufferList<T>::CircularBufferList(CircularBufferList<T> &&other) noexcept: CircularBufferList() {
  std::swap(*this, other);
}

template<typename T>
CircularBufferList<T>::CircularBufferList(): head_(nullptr),
    tail_(nullptr),
    capacity_(0),
    size_(0)
{}

template<typename T>
CircularBufferList<T> &CircularBufferList<T>::operator=(const CircularBufferList<T> &other) {
  CircularBufferList<T> temp(other);
  std::swap(*this, temp);
}

template<typename T>
CircularBufferList<T> &CircularBufferList<T>::operator=(CircularBufferList<T> &&other) noexcept {
  CircularBufferList<T> tmp;
  std::swap(*this, tmp);
  std::swap(*this, other);
}

template<typename T>
CircularBufferList<T>::~CircularBufferList() {
  Node *current = tail_;
  Node *next = tail_->next_;
  while (current != tail_) {
    delete current;
    current = next;
    next = next->next_;
  }
}

template<typename T>
template<typename D>
void CircularBufferList<T>::perfectPushing(D &&addable) {
  if (size_ == capacity_) {
    throw std::overflow_error("Buffer is full. Try to increase capacity by .resize(std::size_t size)");
  }
  if (size_ != 0) {
    head_->next_->key_ = std::forward<D>(addable);
    head_ = head_->next_;
  } else {
    head_->key_ = std::forward<D>(addable);
  }
  size_++;
}

template<typename T>
void CircularBufferList<T>::push(const T &addable) {
  perfectPushing(addable);
}

template<typename T>
void CircularBufferList<T>::push(T &&addable) {
  perfectPushing(std::move(addable));
}

template<typename T>
void CircularBufferList<T>::pop() {
  if (size_ == 0) {
    throw std::underflow_error("You can't pop element from empty buffer");
  }

  if (head_ != tail_) {
    tail_ = tail_->next_;
  }
  size_--;
}

template<typename T>
T &CircularBufferList<T>::getTail() const noexcept {
  if (size_ == 0) {
    throw std::underflow_error("You can't get tail from empty buffer");
  }
  return tail_->key_;
}

template<typename T>
void CircularBufferList<T>::resize(std::size_t newSize) {
  if (newSize < size_) {
    throw std::invalid_argument("You can't make size less than current count of elements");
  }

  if (newSize < capacity_) {
    std::size_t diff = capacity_ - newSize;
    for (std::size_t i = 0; i < diff; ++i) {
      Node *postNext = head_->next_->next_;
      delete head_->next_;
      head_->next_ = postNext;
      capacity_--;
    }
  } else if (newSize > capacity_) {
    std::size_t diff = newSize - capacity_;
    for (std::size_t i = 0; i < diff; ++i) {
      Node *insertable = new Node;
      insertable->next_ = head_->next_;
      head_->next_ = insertable;
      capacity_++;
    }
  }
}

template<typename T>
bool CircularBufferList<T>::isEmpty() const noexcept {
  return size_ == 0;
}

#endif //WARGAMINGTASK_CIRCULARBUFFERLIST_HPP
