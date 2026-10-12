// race condition
// deadlock
// starvation

#include <thread>
#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>

int counter = 0;
std::mutex mutex_a;
std::mutex mutex_b;
std::mutex mutex;
std::atomic<bool> running = true;

void increment() {
  for (int i = 0; i < 1'000'000; ++i) {
    ++counter;
  }
}

void thread_one() {
  std::lock_guard lock_a(mutex_a);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  std::lock_guard lock_b(mutex_b);
  std::cout << "deadlock: thread one finished\n";
}

void thread_two() {
  std::lock_guard lock_b(mutex_b);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  std::lock_guard lock_a(mutex_a);
  std::cout << "deadlock: thread two finished\n";
}

void greedy_worker()
{
  while (running) {
    std::lock_guard lock(mutex);

    std::this_thread::sleep_for(std::chrono::milliseconds(2));
  }
}

void starving_worker()
{
  int attempts = 0;
  int acquired = 0;

  while (running) {
    ++attempts;

    if (mutex.try_lock()) {
      ++acquired;

      std::this_thread::sleep_for(std::chrono::microseconds(50));
      mutex.unlock();
    }
    std::this_thread::yield();
  }

  std::cout << "starvation - "
            << "attempts: " << attempts
            << ", acquired: " << acquired
            << "\n";
}

// ---------------

void practice_race_condition() {
  std::cout << "race condition: FUNCTION START\n";
  std::thread t1(increment);
  std::thread t2(increment);

  t1.join();
  t2.join();

  std::cout << "race condition: counter = " << counter << '\n';
}

void practice_deadlock() {
  std::cout << "deadlock: FUNCTION START\n";
  std::thread t1(thread_one);
  std::thread t2(thread_two);

  t1.join();
  t2.join();

  std::cout << "deadlock: finished\n";
}

void practice_starvation() {
  std::cout << "starvation: FUNCTION START\n";
  std::thread greedy1(greedy_worker);
  std::thread greedy2(greedy_worker);
  std::thread greedy3(greedy_worker);
  std::thread starving(starving_worker);

  std::this_thread::sleep_for(std::chrono::seconds(2));

  running = false;

  greedy1.join();
  greedy2.join();
  greedy3.join();
  starving.join();
}

int main() {
  practice_race_condition();
  practice_starvation();
  // practice_deadlock();

  return 0;
}
