#pragma once

class Miner;
 
class State {
public:
  virtual ~State() = default;
  virtual void Enter(Miner*) = 0;
  virtual void Execute(Miner*) = 0;
  virtual void Exit(Miner*) = 0;
};

class EnterMineAndDigForNugget : public State {
  EnterMineAndDigForNugget() = default;

public:
  // singleton
  static EnterMineAndDigForNugget* Instance();
  void Enter(Miner* miner) override;
  void Execute(Miner* miner) override;
  void Exit(Miner* miner) override;
};


class VisitBankAndDepositGold : public State {
  VisitBankAndDepositGold() = default;

public:
  // singleton
  static VisitBankAndDepositGold* Instance();
  void Enter(Miner* miner) override;
  void Execute(Miner* miner) override;
  void Exit(Miner* miner) override;
};

class GoHomeAndSleepTillRested : public State {
  GoHomeAndSleepTillRested() = default;

public:
  // singleton
  static GoHomeAndSleepTillRested* Instance();
  void Enter(Miner* miner) override;
  void Execute(Miner* miner) override;
  void Exit(Miner* miner) override;
};

class QuenchThirst : public State {
  QuenchThirst() = default;

public:
  // singleton
  static QuenchThirst* Instance();
  void Enter(Miner* miner) override;
  void Execute(Miner* miner) override;
  void Exit(Miner* miner) override;
};
