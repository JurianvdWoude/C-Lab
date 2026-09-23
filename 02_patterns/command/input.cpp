#include <iostream>
#include "input.h"
#include "command.h"

InputHandler::InputHandler(
    Command* w,
    Command* a,
    Command* s,
    Command* d
)
  : buttonW_(w),
    buttonA_(a),
    buttonS_(s),
    buttonD_(d) {}

Command* InputHandler::handleInput() {
  char input;
  std::cin >> input;

  auto it = bindings_.find(input);

  if (it != bindings_.end())
    return it->second;

  return nulltptr;
}

void InputHandler::bind(char button, Command* command) {
  bindings_[button] = command;
}

void InputHandler::printBindings() {
  std::cout << "Commands:\n"
            << "├── W -> " << buttonW_.name() << '\n'
            << "├── A -> " << buttonA_.name() << '\n'
            << "├── S -> " << buttonS_.name() << '\n'
            << "└── D -> " << buttonD_.name() << std::end;
}


