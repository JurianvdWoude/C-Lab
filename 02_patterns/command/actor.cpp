#include <iostream>
#include "actor.h"

void GameActor::jump() {
  std::cout << "the character jumps into the air" << '\n';
}
void GameActor::moveLeft() {
  std::cout << "the character takes one step to the left" << '\n';
}
void GameActor::moveRight() {
  std::cout << "the character takes on step to the right" << '\n';
}
void GameActor::crouch() {
  std::cout << "the character is crouching down" << '\n';
}
