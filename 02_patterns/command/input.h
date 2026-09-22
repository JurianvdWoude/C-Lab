class Command;

class InputHandler {
public:
  InputHandler(Command* w, Command* a, Command* s, Command* d);
  void bind(char button, Command* command);
  void printBindings();
  Command* handleInput();

private:
  Command* buttonW_;
  Command* buttonA_;
  Command* buttonS_;
  Command* buttonD_;
};
