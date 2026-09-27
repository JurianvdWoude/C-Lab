#include <algorithm>
#include <cstdlib>
#include <initializer_list>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <vector>

template <class K, class V>
struct Entry {
  K key;
  V value;
};

template <class K, class V>
class HashTable {
  std::size_t bucketCount;
  std::size_t size;
  std::hash<K> hasher;
  std::vector<std::vector<Entry<K, V>>> table;
  std::size_t hashFunction(K &key) const {
    return hasher(key) % bucketCount;
  }
  double loadFactor() const { return static_cast<double>(size) / bucketCount; }
  void rehash(std::size_t new_bucket_count) {
    std::vector<std::vector<Entry<K, V>>> new_table(new_bucket_count);

    for (const auto &bucket : table) {
      for (const Entry<K, V> &entry : bucket) {
        std::size_t index = hasher(entry.key) % new_bucket_count;
        new_table[index].push_back(entry);
      }
    }
    table = std::move(new_table);
    bucketCount = new_bucket_count;
  }

public:
  HashTable() : bucketCount(8), size(0), table(8) {};
  HashTable(int b) : bucketCount(b), size(0), table(b) {
    if (b < 1) {
      throw std::invalid_argument("bucket size must be larger than 0");
    }
  };
  HashTable(std::initializer_list<T> init)
      : bucketCount(init.size()), size(0), table(init.size()) {
    if (init.size() < 1) {
      throw std::invalid_argument("bucket size must be larger than 0");
    }
    for (const auto &key : init) {
      insert(key);
    }
  }
  void insert(const T &key) {
    if (contains(key)) {
      return;
    }
    std::size_t index = hashFunction(key);
    table[index].push_back(key);
    size++;
    if (loadFactor() > 0.75) {
      rehash(bucketCount * 2);
    }
  };
  bool contains(T &key) const {
    std::size_t index = hashFunction(key);
    for (const auto &table_key : table[index]) {
      if (key == table_key) {
        return true;
      }
    }
    return false;
  }
  bool remove(const T &key) {
    std::size_t index = hashFunction(key);
    auto &inner = table[index];

    auto it = std::find(inner.begin(), inner.end(), key);

    if (it == inner.end()) {
      return false;
    }

    inner.erase(it);
    size--;
    return true;
  }
  std::size_t count() const { return size; };
  std::size_t buckets() const { return bucketCount; };

  template <class U>
  friend std::ostream &operator<<(std::ostream &os, const HashTable<U> &ht);

  HashTable<T> &operator=(const HashTable<T> &ht);
};

template <class T>
HashTable<T> &HashTable<T>::operator=(const HashTable<T> &ht) {
  if (this == &ht) {
    return *this;
  }
  bucketCount = ht.bucketCount;
  size = ht.size;
  table = ht.table;

  return *this;
}

template <class T>
std::ostream &operator<<(std::ostream &os, const HashTable<T> &ht) {
  os << "{";
  std::size_t table_index = 0;
  for (const auto &bucket : ht.table) {
    if (table_index > 0) {
      os << ", ";
    }
    os << table_index << ": [";
    for (std::size_t i = 0; i < bucket.size(); i++) {
      if (i > 0) {
        os << ", ";
      }
      os << bucket[i];
    }
    os << "]";
    table_index++;
  }
  os << "}";
  return os;
}

int main() {
  HashTable<int> a({1, 2, 3});
  std::cout << "size:" << a.count() << ",buckets:" << a.buckets() << ",a:" << a << '\n';

  HashTable<std::string> b({"a", "A", "b", "c", "hello world"}); 
  std::cout << "size:" << b.count() << ",buckets:" << b.buckets() << ",b:" << b << '\n';

  std::cout << b.contains("hello") << ", " << b.contains("hello world") << '\n';
  b.remove("c");
  std::cout << "size:" << b.count() << ",buckets:" << b.buckets() << ",b:" << b << '\n';

  return 0;
}
