#include <iostream>

#include "command.h"
#include "actor.h"
#include "input.h"

void processInput(GameActor& actor, InputHandler& inputHandler) {
  std::cout << "Enter command:\n";
  Command* command = inputHandler.handleInput();

  if (command) {
    command->execute(actor);
  }
}

int main() {
  GameActor actor{};

  JumpCommand jumpCommand{};
  MoveLeftCommand moveLeftCommand{};
  MoveRightCommand moveRightCommand{};
  CrouchCommand crouchCommand{};

  InputHandler inputHandler{
    &jumpCommand,
    &moveLeftCommand,
    &crouchCommand,
    &moveRightCommand,
  };

  inputHandler.printBindings();
  while (true) {
    processInput(actor, inputHandler);
  }
  return 0;
}
