#include <iostream>

class Troll;

class State {
  virtual ~State() = default;
  virtual void execute(Troll& troll) = 0;
}

class State_RunAway : public State {
public:
  void execute(Troll& troll) override {
    if (troll.isSafe()) {
      troll.changeState(std::make_unique<State_Sleep>());
    } else {
      troll.moveAwayFromEnemy();
    }
  }
}

class State_Sleep : public State {
public:
  void execute(Troll& troll) override {
    if (troll.isThreatened()) {
      troll.changeState(std::make_unique<State_RunAway>())
    } else {
      troll.snore();
    }
  }
}

class Troll {
  std::unique_pointer<State> m_pCurrentState;
public:
  void update() {
    b_pCurrentState->execute(*this);
  }
  void changeState(const std::unique_pointer<State> pNewState) {
    m_pCurrentState = std::move(pNewState);
  }
}

int main() {
  std::cout << "hello world" << std::endl;
  return 0;
}
