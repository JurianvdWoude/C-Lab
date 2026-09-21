#include <iostream>
#include <chrono>


decltype(std::chrono::steady_clock::now()) getCurrentTime() {
  return std::chrono::steady_clock::now();
}

void processInput() {
  std::cout << "processing input" << '\n';
}

void update(double elapsedTime) {
  std::cout << "updating game, elapsed time: " << elapsedTime << " s" << '\n';
}

void render() {
  std::cout << "rendering image" << '\n';
}

int main() {
  auto lastTime = getCurrentTime();
  while (true) {
    auto current = getCurrentTime();
    std::chrono::duration<double> elapsedTime = current - lastTime;
    processInput();
    update(elapsedTime.count());
    render();

    lastTime = current;
  }
  return 0;
}
