#include <iostream>
#include <initializer_list>
#include <stdexcept>

template <class T>
class LinkedList {
  struct Node {
    T data; 
    Node* next;
    Node(const T& value) : data(value), next(nullptr) {}
  };

  Node* head;
  std::size_t size;

  Node* get_last_node() {
    Node* node = head;
    if (node) {
      while (node->next) 
        node = node->next;
    } 
    return node;
  }

  public:
    void push_front(const T& value) {
      Node* node = new Node(value);
      node->next = head;
      head = node;
      size++;
    }

    void push_back(const T& value) {
      Node* node = new Node(value);
      Node* last = get_last_node();
      if (last) {
        last->next = node;
      } else {
        head = node;
      }
      size++;
    }

    T pop_front() {
      if (size == 0) 
        throw std::out_of_range("pop_front() on linked list when size is 0");
      T value = head->data;
      Node* next = head->next;
      delete head;
      head = next;
      size--;
      return value;
    }

    std::size_t get_size() const {
      return size;
    }

    LinkedList() : size(0), head(nullptr) {}
    LinkedList(std::initializer_list<T> init) : size(init.size()), head(nullptr) {
      for (auto value = init.end(); value != init.begin(); --value) {
        Node* node = new Node(*(value - 1));
        node->next = head;
        head = node;
      }
    }
    LinkedList(const LinkedList<T>& ll) : size(ll.size), head(nullptr) {
      if (ll.head) {
        Node* node = ll.head;
        Node* node_cp = new Node(node->data);
        head = node_cp;
        while (node->next) {
          Node* next_node_cp = new Node(node->next->data);
          node_cp->next = next_node_cp;
          node_cp = next_node_cp;
          node = node->next;
        }
      }
    }

    ~LinkedList() {
      Node* node = head;
      while (node) {
        Node* next_node = node->next;
        delete node;
        node = next_node;
      }
    }

    template <class U>
    friend std::ostream& operator<< (std::ostream& os, const LinkedList<U>& ll);

    LinkedList& operator= (const LinkedList& ll);
};

template <class T>
std::ostream& operator<< (std::ostream& os, const LinkedList<T>& ll) {
  typename LinkedList<T>::Node* n = ll.head;
  os << "{";
  for (std::size_t i = 0; i < ll.size; i++) {
    os << " " << n->data;
    if ((i + 1) != ll.size) 
      os << " <-";
    n = n->next;
  }
  os << " }";
  return os;
}

template <class T>
LinkedList<T>& LinkedList<T>::operator= (const LinkedList<T>& ll) {
  if (head != ll.head) {
    if (head) {
      Node* node = head;
      while (node) {
        Node* next_node = node->next;
        delete node;
        node = next_node;
      }
    }
    size = ll.size;
    head = nullptr;
    if (ll.head) {
      Node* node = ll.head;
      Node* node_cp = new Node(node->data);
      head = node_cp;
      while (node->next) {
        Node* next_node_cp = new Node(node->next->data);
        node_cp->next = next_node_cp;
        node_cp = next_node_cp;
        node = node->next;
      }
    }
  }
  return *this;
}

int main() {
  LinkedList<int> a{1, 2, 3};

  LinkedList<int> b = a;   // copy constructor

  LinkedList<int> c;
  c = a;                   // copy assignment
  b.push_back(4);
  c.pop_front();

  std::cout << "a: " << a << '\n';
  std::cout << "b: " << b << '\n';
  std::cout << "c: " << c << '\n';

  a = a;

  std::cout << "a: " << a << '\n';

  return 0;
}
