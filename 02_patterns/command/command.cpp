#include "actor.h"
#include "command.h"

void JumpCommand::execute(GameActor& actor) {
  actor.jump(); 
};

void MoveLeftCommand::execute(GameActor& actor) {
  actor.moveLeft();
};

void MoveRightCommand::execute(GameActor& actor) {
  actor.moveRight();
};

void CrouchCommand::execute(GameActor& actor) {
  actor.crouch();
};


