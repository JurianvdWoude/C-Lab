#include <iostream>
#include "../vector/vector.cpp"

template <class T>
class Stack {
  Vector<T> data;
  public: 
    Stack() = default;
    Stack(const Stack<T>& st) : data(st.data) {}
    void push(const T& value) {
      data.push_back(value);
    }
    T pop() {
      return data.pop_back();
    }
    
    template <class U>
    friend std::ostream& operator<< (std::ostream& os, const Stack<U>& st);
    
    Stack& operator= (const Stack& st);
};

template <class T>
std::ostream& operator<< (std::ostream& os, const Stack<T>& st) {
  os << st.data;
  return os;
}

template <class T>
Stack<T>& Stack<T>::operator= (const Stack<T>& st) {
  this->data = st.data;
  return *this;
}

int main() {
  Stack<int> a;
  std::cout << "a: " << a << std::endl;

  a.push(1);
  a.push(2);
  std::cout << "a: " << a << std::endl;
  
  Stack<int> b = a;
  std::cout << "b: " << b << std::endl;

  Stack<int> c;
  c.push(3);
  c = a;
  std::cout << "c: " << c << std::endl;
}
