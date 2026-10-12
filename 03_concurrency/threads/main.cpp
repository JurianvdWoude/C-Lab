#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <string>

void work(int id, std::string name) {
  std::string opening_text = "Thread " + std::to_string(id) + " started: " + name + "\n";
  std::cout << opening_text;

  std::this_thread::sleep_for(std::chrono::milliseconds(200));

  std::string ending_text = "Thread " + std::to_string(id) + " ended: " + name + "\n";
  std::cout << ending_text;
  std::cout << "Thread id: " << id << ", " << std::this_thread::get_id() << "\n";
}

void increment(int& value) {
  ++value;
}

void practice_join() {
  std::cout << "std::join: FUNCTION START\n";
  // Practice thread and join
  std::cout << "std::join - Main starting\n";
  std::thread t1(work, 1, "std::join - alpha");
  std::thread t2(work, 2, "std::join - beta");
  std::thread t3(work, 3, "std::join - gamma");

  std::cout << "std::join - Main threads launched\n";

  t1.join();
  t2.join();
  t3.join();

  std::cout << "std::join - Main finished\n";
}

void practice_ref() {
  std::cout << "std::ref: FUNCTION START\n";
  int x = 0;
  std::cout << "std::ref - x = " << x << "\n";
  std::thread tn(increment, std::ref(x));
  tn.join();
  std::cout << "std::ref - increment: " << x << "\n";
}

void practice_detach() {
  std::cout << "std::detach: FUNCTION START\n";
  std::thread t([] {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::cout << "std::detach - background work done\n";
  });
  t.detach();
  std::cout << "std::detach - main is done\n";
}

void practice_emplace_back() {
  std::cout << "std::emplace_back: FUNCTION START\n";
  std::vector<std::thread> threads;

  for (int i = 0; i < 4; ++i) {
    threads.emplace_back(work, i, "std::emplace_back - job");
  }
  for (auto& t : threads) {
    t.join();
  }
  std::cout << "std::emplace_back - main finished \n";
}

int main() {
  practice_join();
  practice_ref();
  practice_detach();
  practice_emplace_back();

  return 0;
}
