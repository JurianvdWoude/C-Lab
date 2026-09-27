#include <algorithm>
#include <cstdlib>
#include <initializer_list>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <vector>

class HashTable {
  std::size_t bucketCount;
  std::size_t size;
  std::hash<int> hasher;
  std::vector<std::vector<int>> table;
  std::size_t hashFunction(int key) const {
    return hasher(key) % bucketCount;
  }
  double loadFactor() const { return static_cast<double>(size) / bucketCount; }
  void rehash(std::size_t new_bucket_count) {
    std::vector<std::vector<int>> new_table(new_bucket_count);

    for (const auto &bucket : table) {
      for (int key : bucket) {
        std::size_t index = hasher(key) % new_bucket_count;
        new_table[index].push_back(key);
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
  HashTable(std::initializer_list<int> init, int b)
      : bucketCount(b), size(0), table(b) {
    if (b < 1) {
      throw std::invalid_argument("bucket size must be larger than 0");
    }
    for (const auto &key : init) {
      insert(key);
    }
  }
  ~HashTable() {}
  void insert(int key) {
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
  bool contains(int key) const {
    std::size_t index = hashFunction(key);
    for (const auto &table_key : table[index]) {
      if (key == table_key) {
        return true;
      }
    }
    return false;
  }
  bool remove(int key) {
    if (!contains(key))
      return false;

    std::size_t index = hashFunction(key);
    auto &inner = table[index];
    inner.erase(std::remove(inner.begin(), inner.end(), key), inner.end());

    size--;
    return true;
  }
  std::size_t count() const { return size; };
  std::size_t buckets() const { return bucketCount; };

  friend std::ostream &operator<<(std::ostream &os, const HashTable &ht);

  HashTable &operator=(const HashTable &ht);
};

HashTable &HashTable::operator=(const HashTable &ht) {
  if (this == &ht) {
    return *this;
  }
  bucketCount = ht.bucketCount;
  size = ht.size;
  table = ht.table;

  return *this;
}

std::ostream &operator<<(std::ostream &os, const HashTable &ht) {
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
  HashTable a;
  a.insert(1);
  a.insert(4);
  a.insert(15);
  std::cout << a << '\n';
  HashTable b({1, 2, 3}, 8);
  std::cout << b << '\n';
  HashTable c = b;
  std::cout << "size:" << c.count() << ",buckets:" << c.buckets() << ",c:" << c << '\n';
  c.remove(3);
  std::cout << "size:" << c.count() << ",buckets:" << c.buckets() << ",c:" << c << '\n';
  c.insert(8);
  c.insert(9);
  c.insert(10);
  c.insert(11);
  c.insert(12);
  c.insert(13);
  std::cout << "size:" << c.count() << ",buckets:" << c.buckets() << ",c:" << c << '\n';
  return 0;
}
