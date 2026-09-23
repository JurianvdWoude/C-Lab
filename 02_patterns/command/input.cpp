#include <iostream>
#include "input.h"
#include "command.h"

InputHandler::InputHandler(
    Command* w,
    Command* a,
    Command* s,
    Command* d
) {
  bind('w', w);
  bind('a', a);
  bind('s', s);
  bind('d', d);
}

Command* InputHandler::handleInput() {
  char input;
  std::cin >> input;

  if (input == 'q') {
    askToRebind();
    return nullptr;
  }

  auto it = bindings_.find(input);

  if (it != bindings_.end())
    return it->second;

  return nullptr;
}

void InputHandler::askToRebind() {
  std::cout << "  Which button do you want to rebind?\n";
  printBindings();

  char letter;
  std::cout << "\nFirst, type the key with the command that you want to change: ";
  std::cin >> letter;

  auto old_it = bindings_.find(letter);
  if (old_it == bindings_.end()) {
    std::cout << "Cannot find key in list of bindings\n\n";
    return;
  }

  std::cout << "Next, type what the new key should be for that command: ";

  char new_letter;
  std::cin >> new_letter;

  if (new_letter == 'q') {
    std::cout << "Unable to recognize key\n\n";
    return;
  }

  Command* command = old_it->second;
  auto new_it = bindings_.find(new_letter);

  if (new_it == bindings_.end()) {
    bind(new_letter, command);
    bindings_.erase(letter);
  } else {
    Command* other_command = new_it->second;
    bind(letter, other_command);
    bind(new_letter, command);
  }

  std::cout << "\nList of new commands\n";
  printBindings();
}

void InputHandler::bind(char button, Command* command) {
  if(button == 'q') return;
  bindings_[button] = command;
}

void InputHandler::printBindings() {
  std::cout << "\nCommands:\n";
  for (const auto& [key, value] : bindings_) {
    std::cout << "  " << key << " maps to: " << value->name() << '\n';
  }
  std::cout << "  q maps to: Rebind (cannot be changed)\n" << std::endl;
}


