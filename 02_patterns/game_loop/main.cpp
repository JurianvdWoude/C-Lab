#include <iostream>

void processInput() {
  std::cout << "processing input" << '\n';
}

void update() {
  std::cout << "updating game" << '\n';
}

void render() {
  std::cout << "rendering image" << '\n';
}

int main() {
  while (true) {
    processInput();
    update();
    render();
  }
  return 0;
}
