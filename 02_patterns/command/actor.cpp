#include <iostream>
#include "actor.h"

void GameActor::jump() {
  std::cout << "  The character jumps into the air\n" << std::endl;
}
void GameActor::moveLeft() {
  std::cout << "  The character takes one step to the left\n" << std::endl;
}
void GameActor::moveRight() {
  std::cout << "  The character takes one step to the right\n" << std::endl;
}
void GameActor::crouch() {
  std::cout << "  The character is crouching down\n" << std::endl;
}
