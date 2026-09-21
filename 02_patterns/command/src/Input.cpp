
class InputHandler {
  void handleInput() {
    if (isPressed(BUTTON_W)) jump();
    else if (isPressed(BUTTON_A)) go_left();
    else if (isPressed(BUTTON_D)) go right();
    else if (isPressed(BUTTON_S)) crouch();
  }
}
