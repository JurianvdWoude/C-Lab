#include "vector.cpp"

int main()
{
    Vector<int> a{1, 2, 3};

    std::cout << a << '\n';
    std::cout << "size: " << a.get_size() << '\n';
    std::cout << "capacity: " << a.get_capacity() << '\n';

    a.push_back(4);
    a.push_back(5);

    std::cout << a << '\n';

    Vector<int> b = a;       // copy constructor
    b.push_back(6);

    std::cout << "a: " << a << '\n';
    std::cout << "b: " << b << '\n';

    Vector<int> c;
    c = a;                   // copy assignment

    std::cout << "c: " << c << '\n';

    c.pop_back();

    std::cout << "a: " << a << '\n';
    std::cout << "c: " << c << '\n';
}
