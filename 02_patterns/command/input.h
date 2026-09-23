class Command;

class InputHandler {
public:
  InputHandler(Command* w, Command* a, Command* s, Command* d);
  void bind(char button, Command* command);
  void printBindings();
  Command* handleInput();

private:
  std::unordered_map<char, Command*> bindings_;
};
