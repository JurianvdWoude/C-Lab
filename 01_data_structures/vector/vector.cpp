#include <iostream>
#include <initializer_list>
#include <stdexcept>

template <class T>
class Vector {
  std::size_t size;
  std::size_t capacity;
  T* data = nullptr;

  void grow() {
    grow_capacity();
    T* new_data = new T[capacity];
    for (std::size_t i = 0; i < size; i++)
      new_data[i] = data[i];
    delete[] data;
    data = new_data;
  }

  void shrink() {
    if (shrink_capacity()) return;
    T* new_data = new T[capacity];
    for (std::size_t i = 0; i < size; i++) 
      new_data[i] = data[i];
    delete[] data;
    data = new_data;
  }

  void grow_capacity() {
    while (this->size >= this->capacity) {
      std::size_t current = this->capacity;
      this->capacity = current == 0 ? 1 : current * 2;
    }
  }

  bool shrink_capacity() {
    std::size_t current = this->capacity;
    if (current > 1) {
      this->capacity = current / 2;
      return true;
    }
    return false;
  }

  std::size_t initialize_capacity(std::size_t value) {
    if (value <= 1) 
      return 1;

    std::size_t container_size = 1;
    while (value > container_size) {
      container_size <<= 1;
    }
    return container_size;
  }

  public:
    Vector() : size(0), capacity(0) {
      data = nullptr;
    }

    Vector(std::initializer_list<T> init) : size(init.size()), capacity(initialize_capacity(init.size())), data(new T[capacity]) {
      std::size_t i = 0;
      for (const auto& value : init) {
        data[i++] = value;
      }
      this->size = i;
    }

    Vector(const Vector& vec) : size(vec.size), capacity(vec.capacity), data(new T[vec.capacity]) {
      for (std::size_t i = 0; i < vec.size; i++) {
        data[i] = vec.data[i];
      }
    }

    ~Vector() {delete[] data;}

    void push_back(T value) {
      if (size == capacity)
        grow();
      data[size] = value;
      ++size;
    }

    T pop_back() {
      if (size == 0) 
        throw std::out_of_range("pop_back() on vector when size is 0");
      T value = data[size - 1];
      --size;
      if (size * 2 == capacity) 
        shrink();
      return value;
    }

    std::size_t get_size() const {
      return size;
    }

    std::size_t get_capacity() const {
      return capacity;
    }

    template <class U>
    friend std::ostream& operator<< (std::ostream& os, const Vector<U>& vec);

    Vector& operator= (const Vector& vec);
};

template <class T>
std::ostream& operator<< (std::ostream& os, const Vector<T>& vec) {
  os << "{";
  for (std::size_t i = 0; i < vec.size; i++) {
    os << vec.data[i];
    if (i + 1 < vec.size) 
      os << ", ";
  }
  os << "}";
  return os;
}

template <class T>
Vector<T>& Vector<T>::operator= (const Vector<T>& vec) {
  this->size = vec.size;
  this->capacity = vec.capacity;
  T* new_data = new T[vec.capacity];
  for (std::size_t i = 0; i < vec.size; i++) 
    new_data[i] = vec.data[i];
  delete[] this->data;
  this->data = new_data;
  return *this;
}

