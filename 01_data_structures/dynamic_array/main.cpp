#include "vector.cpp"

int main() {
  Vector<int> a;
  a.push_back(10);
  a.push_back(20);

  std::cout << a << ", size: " << a.get_size() << ", capacity: " << a.get_capacity() << std::endl;


  Vector<int> b{1, 2, 3};

  a = b;

  std::cout << a << ", size: " << a.get_size() << ", capacity: " << a.get_capacity() << std::endl;
}
