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

    Vector(Vector&& vec) noexcept : size(vec.size), capacity(vec.capacity), data(vec.data) { 
      vec.capacity = 0;
      vec.size = 0;
      vec.data = nullptr;
    }

    ~Vector() {delete[] data;}

    void push_back(T value) {
      if (size == capacity)
        grow();
      data[size] = std::move(value);
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
    template <class U>
    friend Vector<U> operator+ (const Vector<U>& lhs, const Vector<U>& rhs);

    T& operator[] (std::size_t i);
    Vector& operator= (const Vector& vec);
    Vector& operator= (Vector&& vec);
    Vector& operator+= (const Vector& rhs);
};
template <class T>
T& Vector<T>::operator[] (std::size_t i) {
  return data[i];
}

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
Vector<T>& Vector<T>::operator+= (const Vector<T>& vec) {
  if (vec.size != size)
    throw std::invalid_argument("cannot add Vectors of different sizes");
  for(std::size_t i = 0; i < vec.size; i++) {
    this->data[i] += vec.data[i];
  }
  return *this;
}

template <class T>
Vector<T>& Vector<T>::operator= (const Vector<T>& vec) {
  T* new_data = new T[vec.capacity];
  for (std::size_t i = 0; i < vec.size; i++) 
    new_data[i] = vec.data[i];
  delete[] this->data;

  data = new_data;
  size = vec.size;
  capacity = vec.capacity;

  return *this;
}

template <class T>
Vector<T>& Vector<T>::operator= (Vector<T>&& vec) {
  if (this != &vec) {
    delete[] data;

    data = vec.data;
    size = vec.size;
    capacity = vec.capacity;

    vec.data = nullptr;
    vec.size = 0;
    vec.capacity = 0;
  }
  return *this;
}

template <class T>
Vector<T> operator+ (const Vector<T>& lhs, const Vector<T>& rhs) {
  Vector<T> result = lhs;
  result += rhs;
  return result;
}
