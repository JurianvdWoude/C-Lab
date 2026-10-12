// mutex
// atomic
// condition variable
// semaphore
// barrier

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <semaphore>
#include <barrier>

int counter = 0;
std::atomic<int> a_counter{0};
std::mutex mtx;
std::condition_variable cv;
std::counting_semaphore<3> sem(3);
std::barrier sync_point(3);

void increment(int thread_id) {
  std::cout << "std::mutex: locking thread " << thread_id << '\n';
  mtx.lock();
  for (int i = 0; i < 1'000'000; i++) {
    counter++;
    if (counter % 500'000 == 0) {
      std::cout << "std::mutex: thread " << thread_id << " is at counter " << counter << '\n';
    }
  }
  std::cout << "std::mutex: unlocking thread " << thread_id << '\n';
  mtx.unlock();
}

void increment_safer(int thread_id) {
  std::lock_guard<std::mutex> lock(mtx);
  for (int i = 0; i < 1'000'000; i++) {
    counter++;
    if (counter % 500'000 == 0) {
      std::cout << "std::lock_guard: thread " << thread_id << " is at counter " << counter << '\n';
    }
  }
}

void worker() {
  std::unique_lock<std::mutex> lock(mtx);
  std::cout << "std::condition_variable: worker has to wait, counter is " << counter << '\n';
  cv.wait(lock, [] { return counter == 15; });
  std::cout << "std::condition_variable: worker is freed from waiting, counter is " << counter << '\n';
}

void signal() {
  {
    std::lock_guard<std::mutex> lock(mtx);
    counter = 15;
  }
  cv.notify_one();
}

void increment_atomic() {
  for (int i = 0; i < 1'000'000; i++) 
    a_counter++;
}

void task(int id) {
  sem.acquire();
  std::cout << "std::semaphore: thread " << id << " entered\n";
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::cout << "std::semaphore: thread " << id << " leaving\n";
  sem.release();
}

void barrier_worker(int id) {
  std::cout << "std::barrier: thread " << id << " before barrier\n";
  std::this_thread::sleep_for(std::chrono::milliseconds(500 * id));

  sync_point.arrive_and_wait();

  std::cout << "std::barrier: thread " << id << " after barrier\n";
}

// ----------------------


void practice_mutex() {
  std::thread t1(increment, 1);
  std::thread t2(increment, 2);

  t1.join();
  t2.join();

  std::cout << "std::mutex: counter is " << counter << '\n';

  std::thread t3(increment_safer, 3); 

  t3.join();

  std::cout << "std::lock_guard: counter is " << counter << '\n';

  std::thread t4(worker);
  std::thread t5([] {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    signal();
  });

  t4.join();
  t5.join();
}

void practice_atomic() {
  std::cout << "std::atomic: starting count " << a_counter << '\n';
  std::cout << "std::atomic: 4 threads\n";

  std::vector<std::thread> threads;
  for (int i = 0; i < 4; ++i) {
    threads.emplace_back(increment_atomic);
  }
  for (auto& t : threads) {
    t.join();
  }
  std::cout << "std::atomic: final count " << a_counter << '\n';
}

void practice_semaphore() {
  std::vector<std::thread> threads;
  for (int i = 0; i < 6; i++) {
    threads.emplace_back(task, i);
  }
  for (auto& t : threads) {
    t.join();
  }
}

void practice_barrier() {
  std::thread t6(barrier_worker, 6);
  std::thread t7(barrier_worker, 7);
  std::thread t8(barrier_worker, 8);

  t6.join();
  t7.join();
  t8.join();
}


int main() {
  practice_mutex();
  practice_atomic();
  practice_semaphore();
  practice_barrier();
  return 0;
}
