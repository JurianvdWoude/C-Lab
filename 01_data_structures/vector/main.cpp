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

    Vector<int> d{1, 2, 4};
    Vector<int> e{1, 1, 1};

    std::cout << "d: " << d << '\n';
    std::cout << "e: " << e << '\n';

    d += e;

    std::cout << "d += e: " << d << '\n';

    d = e + e;

    std::cout << "d = e + e: " << d << '\n';
    std::cout << "a[2]: " << a[2] << '\n';
}
