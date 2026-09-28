#include <algorithm>
#include <cstdlib>
#include <initializer_list>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <vector>


template <class K, class V>
class HashTable {
  struct Entry {
    K key;
    V value;
  };
  std::size_t bucketCount;
  std::size_t size;
  std::hash<K> hasher;
  std::vector<std::vector<Entry>> table;
  std::size_t hashFunction(const K &key) const {
    return hasher(key) % bucketCount;
  }
  double loadFactor() const { return static_cast<double>(size) / bucketCount; }
  void rehash(std::size_t new_bucket_count) {
    std::vector<std::vector<Entry>> new_table(new_bucket_count);

    for (const auto &bucket : table) {
      for (const Entry &entry : bucket) {
        std::size_t index = hasher(entry.key) % new_bucket_count;
        new_table[index].push_back(entry);
      }
    }
    table = std::move(new_table);
    bucketCount = new_bucket_count;
  }

public:
  HashTable() : bucketCount(8), size(0), table(8) {}
  HashTable(int b) : bucketCount(b), size(0), table(b) {
    if (b < 1) {
      throw std::invalid_argument("bucket size must be larger than 0");
    }
  }
  HashTable(std::initializer_list<std::pair<K, V>> init)
      : bucketCount(init.size()), size(0), table(init.size()) {
    if (init.size() < 1) {
      throw std::invalid_argument("bucket size must be larger than 0");
    }
    for (const auto &[key, value] : init) {
      insert(key, value);
    }
  }
  void insert(const K &key, const V &value) {
    if (contains(key)) {
      return;
    }
    std::size_t index = hashFunction(key);
    table[index].push_back({key, value});
    size++;
    if (loadFactor() > 0.75) {
      rehash(bucketCount * 2);
    }
  };
  bool contains(const K &key) const {
    std::size_t index = hashFunction(key);
    const auto &inner = table[index];
    for (const auto &entry : inner) {
      if (key == entry.key) {
        return true;
      }
    }
    return false;
  }
  V get(const K &key) const {
    std::size_t index = hashFunction(key);
    const auto &inner = table[index];

    for (const auto &entry : inner) {
      if (key == entry.key) {
        return entry.value;
      }
    }
    throw std::out_of_range("Key not found");
  }
  bool remove(const K &key) {
    std::size_t index = hashFunction(key);
    auto &inner = table[index];

    for (auto it = inner.begin(); it != inner.end(); ++it) {
      if (key == it->key) {
        inner.erase(it);
        size--;
        return true;
      }
    }
    return false;
  }
  std::size_t count() const { return size; };
  std::size_t buckets() const { return bucketCount; };

  template <class I, class J>
  friend std::ostream &operator<<(std::ostream &os, const HashTable<I, J> &ht);
};

template <class K, class V>
std::ostream &operator<<(std::ostream &os, const HashTable<K, V> &ht) {
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
      os << "(" << bucket[i].key << ": " << bucket[i].value << ")";
    }
    os << "]";
    table_index++;
  }
  os << "}";
  return os;
}

int main() {
  HashTable<int, std::string> a{{1, "one"}, {2, "two"}, {3, "three"}};
  std::cout << "size:" << a.count() << ",buckets:" << a.buckets() << ",a:" << a << '\n';

  HashTable<std::string, int> b{{"a", 1}, {"b", 10}}; 
  std::cout << "size:" << b.count() << ",buckets:" << b.buckets() << ",b:" << b << '\n';

  b.remove("b");
  std::cout << "size:" << b.count() << ",buckets:" << b.buckets() << ",b:" << b << '\n';

  HashTable<int, std::string> c = a;
  std::cout << "size:" << c.count() << ",buckets:" << c.buckets() << ",c:" << c << '\n';

  c.insert(7, "seven");
  c.insert(1, "new");
  std::cout << "size:" << a.count() << ",buckets:" << a.buckets() << ",a:" << a << '\n';
  std::cout << "size:" << c.count() << ",buckets:" << c.buckets() << ",c:" << c << '\n';
  return 0;
}
