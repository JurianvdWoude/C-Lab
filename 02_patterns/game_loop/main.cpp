#include <iostream>
#include <chrono>

using Clock = std::chrono::steady_clock;
using Duration = std::chrono::duration<double>;

const auto TIME_PER_UPDATE = Duration{1.0 / 60.0};

void processInput() {
  std::cout << "i";
}

void update(Duration deltaTime) {
  std::cout << "\n\n-=-=-=-\n\n  updating\n\n-=-=-=-\n\n";
}

void render(double alpha) {
  std::cout << "r";
}

int main() {
  auto previousTime = Clock::now();
  auto lag = Duration::zero();

  while (true) {
    auto currentTime = Clock::now();
    Duration deltaTime = currentTime - previousTime;

    previousTime = currentTime;
    lag += deltaTime;

    processInput();

    while (lag >= TIME_PER_UPDATE) {
      update(TIME_PER_UPDATE);
      lag -= TIME_PER_UPDATE;
    }
    
    double alpha = lag / TIME_PER_UPDATE;
    render(alpha);
  }
  return 0;
}
