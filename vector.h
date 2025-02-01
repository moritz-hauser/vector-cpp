#ifndef VECTOR_H
#define VECTOR_H
#include <iostream>
#include <stdexcept>

template <typename T> class Vector {
public:
  class ConstIterator;
  class Iterator;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using iterator = Vector::Iterator;
  using const_iterator = Vector::ConstIterator;
  using value_type = T;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = value_type *;
  using const_pointer = const value_type *;

private:
  static constexpr size_type MIN_SIZE = 10;
  size_type capacity_;
  size_type size_;
  pointer values_;

  void realloc(size_type new_size) {
    pointer new_values{new value_type[new_size]};
    std::copy(values_, values_ + size_, new_values);
    delete[] values_;
    values_ = new_values;
    capacity_ = new_size;
  }

  size_type grow() { return capacity_ * 2; }

public:
  // Konstruktoren
  Vector() : capacity_{MIN_SIZE}, size_{0}, values_{new value_type[MIN_SIZE]} {}
  Vector(size_type max_sz)
      : capacity_{max_sz < MIN_SIZE ? MIN_SIZE : max_sz}, size_{0},
        values_{new value_type[this->capacity_]} {}
  Vector(std::initializer_list<value_type> ilist) : Vector(ilist.size()) {
    for (const auto &elem : ilist)
      values_[size_++] = elem;
  }
  // Kopierkonstruktor
  Vector(const Vector &src) : Vector(src.size_) {
    for (; size_ < src.size_; ++size_)
      values_[size_] = src.values_[size_];
  }
  // Destruktor
  ~Vector() { delete[] values_; }

  // operator=
  Vector &operator=(Vector src) {
    std::swap(capacity_, src.capacity_);
    std::swap(size_, src.size_);
    std::swap(values_, src.values_);
    return *this;
  }

  // push_back
  void push_back(const_reference val) {
    if (size_ >= capacity_)
      realloc(grow());
    values_[size_++] = val;
  }

  // operator[]
  reference operator[](size_type index) {
    if (index >= size_)
      throw std::runtime_error{"index out of bounds"};
    return values_[index];
  }

  // operator[] für const vektoren
  const_reference operator[](size_type index) const {
    if (index >= size_)
      throw std::runtime_error{"index out of bounds"};
    return values_[index];
  }

  // size()
  size_t size() const { return size_; }

  // empty()
  bool empty() const { return size_ == 0; }

  // clear()
  void clear() { size_ = 0; }

  // reserve()
  void reserve(size_t n) {
    if (capacity_ < n)
      realloc(n);
  }

  // shrink_to_fit()
  void shrink_to_fit() {
    // Kapazität auf Anzahl der Elemente reduzieren
    /* ALT
    if(size_<MIN_SIZE) realloc(MIN_SIZE);
    else realloc(size_);
    */
    size_ < MIN_SIZE ? realloc(MIN_SIZE) : realloc(size_);
  }

  // pop_back
  void pop_back() {
    if (size_ == 0)
      throw std::runtime_error{"vector already empty"};
    else
      --size_;
  }

  // capacity()
  size_t capacity() const { return capacity_; }

  // print()
  // [0, 1, 2, 3]
  std::ostream &print(std::ostream &o) const {
    if (empty()) {
      o << "[]";
      return o;
    }
    o << "[";
    for (size_type i{0}; i < (size_ - 1); ++i) {
      o << values_[i] << ", ";
    }
    o << values_[size_ - 1] << "]";
    return o;
  }

  // iterator begin()
  iterator begin() {
    return (empty() ? end()
                    : iterator(values_, values_ + size_)); // end mitgeben
  }

  const_iterator begin() const {
    return (empty() ? end() : const_iterator(values_, values_ + size_));
  }

  // iterator end()
  iterator end() { return iterator(values_ + size_, values_ + size_); }

  const_iterator end() const {
    return const_iterator(values_ + size_, values_ + size_);
  }

  // kopierte Methoden
  iterator insert(const_iterator pos, const_reference val) {
    auto diff = pos - begin();
    if (diff < 0 || static_cast<size_type>(diff) > size_)
      throw std::runtime_error("Iterator out of bounds");
    size_type current{static_cast<size_type>(diff)};
    if (size_ >= capacity_)
      reserve(capacity_ *
              2); // Achtung Sonderfall, wenn keine Mindestgroesze definiert ist
    for (auto i{size_}; i-- > current;)
      values_[i + 1] = values_[i];
    values_[current] = val;
    ++size_;
    return iterator{values_ + current, values_ + size_}; // korrekt?
                                                         // values+sz???
  }

  iterator erase(const_iterator pos) {
    auto diff = pos - begin();
    if (diff < 0 || static_cast<size_type>(diff) >= size_)
      throw std::runtime_error("Iterator out of bounds");
    size_type current{static_cast<size_type>(diff)};
    for (auto i{current}; i < size_ - 1; ++i)
      values_[i] = values_[i + 1];
    --size_;
    return iterator{values_ + current, values_ + size_}; // korrekt?
                                                         // values+sz???
  }

  // iteratoren
  class Iterator {
  public:
    using value_type = Vector::value_type;
    using reference = Vector::reference;
    using pointer = Vector::pointer;
    using difference_type = Vector::difference_type;
    using iterator_category = std::forward_iterator_tag;

  private:
    // Instanzvariablen
    pointer ptr;
    pointer end = nullptr;

  public:
    // Konstruktor
    Iterator() : ptr{nullptr} {}
    Iterator(pointer ptr, pointer end) : ptr{ptr}, end{end} {}
    // Methoden
    reference operator*() {
      if (ptr == end)
        throw std::runtime_error(
            "end pointer kann nicht dereferenziert werden");
      return *ptr;
    }

    pointer operator->() { return ptr; }

    bool operator==(const const_iterator &rop) const { return ptr == rop.ptr; }

    bool operator!=(const const_iterator &rop) const { return ptr != rop.ptr; }

    iterator &operator++() {
      // für secure (auch in const):
      if (ptr == end)
        throw std::runtime_error("iterator ist bereits am ende angelangt");
      // standard:
      ++ptr;
      return *this;
    }

    iterator operator++(int) {
      // für secure (auch in const):
      if (ptr == end)
        throw std::runtime_error("iterator ist bereits am ende angelangt");
      // standard:
      Iterator ret(ptr, end);
      ++ptr;
      return ret;
    }

    operator const_iterator() const { return const_iterator(ptr, end); }

    difference_type operator-(const const_iterator &rop) const {
      return ptr - rop.ptr;
    }
  };
  class ConstIterator {
  public:
    friend class Iterator; // erlaubt??? sonst bei zugriff auf ptr static casten
    using value_type = Vector::value_type;
    using reference = Vector::const_reference;
    using pointer = Vector::const_pointer;
    using difference_type = Vector::difference_type;
    using iterator_category = std::forward_iterator_tag;

  private:
    // Instanzvariablen
    pointer ptr;
    pointer end = nullptr;

  public:
    // Konstruktor
    ConstIterator() : ptr{nullptr} {}
    ConstIterator(pointer ptr, pointer end) : ptr{ptr}, end{end} {}
    // Methoden
    reference operator*() {
      if (ptr == end)
        throw std::runtime_error(
            "end pointer kann nicht dereferenziert werden");
      return *ptr;
    }

    pointer operator->() { return ptr; }

    bool operator==(const const_iterator &rop) const { return ptr == rop.ptr; }

    bool operator!=(const const_iterator &rop) const { return ptr != rop.ptr; }

    const_iterator &operator++() {
      // für secure:
      if (ptr == end)
        throw std::runtime_error("iterator ist bereits am ende angelangt");
      // standard:
      ++ptr;
      return *this;
    }

    const_iterator operator++(int) {
      // für secure:
      if (ptr == end)
        throw std::runtime_error("iterator ist bereits am ende angelangt");
      // standard:
      const_iterator ret(ptr, end);
      ++ptr;
      return ret;
    }

    difference_type operator-(const const_iterator &rop) const {
      return ptr - rop.ptr;
    }
  };
};

// operator<<
template <typename T>
std::ostream &operator<<(std::ostream &o, const Vector<T> &v) {
  return v.print(o);
}
#endif