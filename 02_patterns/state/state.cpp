#include <iostream>
#include "state.h"
#include "entity.h"

EnterMineAndDigForNugget* EnterMineAndDigForNugget::Instance() {
  static EnterMineAndDigForNugget instance;
  return &instance;
}

void EnterMineAndDigForNugget::Enter(Miner* miner) {
  if (miner->Location() != goldmine) {
    std::cout << '\n' 
              << GetNameOfEntity(miner->ID())
              << ": "
              << "Walkin' to the gold mine";
    miner->ChangeLocation(goldmine);
  }
}

void EnterMineAndDigForNugget::Execute(Miner* miner) {
  miner->AddToGoldCarried(1);
  miner->IncreaseFatigue();
  std::cout << '\n'
            << GetNameOfEntity(miner->ID())
            << ": "
            << "Pickin' up a nugget";

  if (miner->PocketsFull()) {
    miner->ChangeState(VisitBankAndDepositGold::Instance());
    return;
  }

  if (miner->Thirsty()) {
    miner->ChangeState(QuenchThirst::Instance());
    return;
  }
}

void EnterMineAndDigForNugget::Exit(Miner* miner) {
  std::cout << '\n'
            << GetNameOfEntity(miner->ID())
            << ": "
            << "Ah'm leavin' the gold mine with mah pockets full o' gold";
}

// 000000000000000000000000000000000000000000000000000000000000000000000000000

VisitBankAndDepositGold* VisitBankAndDepositGold::Instance() {
  static VisitBankAndDepositGold instance;
  return &instance;
}

void VisitBankAndDepositGold::Enter(Miner* miner) {
  if (miner->Location() != bank) {
    std::cout << '\n'
              << GetNameOfEntity(miner->ID())
              << ": "
              << "Goin' to the bank. Yes siree";
    miner->ChangeLocation(bank);
  }
}

void VisitBankAndDepositGold::Execute(Miner* miner) {
  miner->AddToWealth(miner->GoldCarried());
  miner->SetGoldCarried(0);
  std::cout << '\n'
            << GetNameOfEntity(miner->ID())
            << ": "
            << "Depositin' gold. Total savings now: "
            << miner->Wealth();
  if (miner->Wealth() >= ComfortLevel) {
    std::cout << '\n'
              << GetNameOfEntity(miner->ID())
              << ": "
              << "WooHoo! Rich enough for now-> Back home to mah lil' lady";
    miner->ChangeState(GoHomeAndSleepTillRested::Instance());
  } else {
    miner->ChangeState(EnterMineAndDigForNugget::Instance());
  }
}

void VisitBankAndDepositGold::Exit(Miner* miner) {
  std::cout << '\n'
            << GetNameOfEntity(miner->ID())
            << ": "
            << "Leavin' the bank";
}


// 000000000000000000000000000000000000000000000000000000000000000000000000000

GoHomeAndSleepTillRested* GoHomeAndSleepTillRested::Instance() {
  static GoHomeAndSleepTillRested instance;
  return &instance;
}

void GoHomeAndSleepTillRested::Enter(Miner* miner) {
  if (miner->Location() != shack) {
    std::cout << '\n'
              << GetNameOfEntity(miner->ID())
              << ": "
              << "Walkin' home";
    miner->ChangeLocation(shack);
  }
}

void GoHomeAndSleepTillRested::Execute(Miner* miner) {
  if (!miner->Fatigued()) {
    std::cout << '\n'
              << GetNameOfEntity(miner->ID())
              << ": "
              << "What a God darn fantastic nap! Time to find more gold";
    miner->ChangeState(EnterMineAndDigForNugget::Instance());
  } else {
    miner->DecreaseFatigue();
    std::cout << '\n'
              << GetNameOfEntity(miner->ID())
              << ": "
              << "ZZZZ...";
  }
}

void GoHomeAndSleepTillRested::Exit(Miner* miner) {
  std::cout << '\n'
            << GetNameOfEntity(miner->ID())
            << ": "
            << "Leaving the house";
}

// 000000000000000000000000000000000000000000000000000000000000000000000000000

QuenchThirst* QuenchThirst::Instance() {
  static QuenchThirst instance;
  return &instance;
}

void QuenchThirst::Enter(Miner* miner) {
  if (miner->Location() != saloon) {
    std::cout << '\n'
              << GetNameOfEntity(miner->ID())
              << ": "
              << "Boy, ah sure is thusty! Walkin' to the saloon";
    miner->ChangeLocation(saloon);
  }
}

void QuenchThirst::Execute(Miner* miner) {
    if (miner->Thirsty()) {
      miner->BuyAndDrinkWhiskey();
      std::cout << '\n'
                << GetNameOfEntity(miner->ID())
                << ": "
                << "That's mighty fine sippin' liquer";

      miner->ChangeState(EnterMineAndDigForNugget::Instance());
    } else {
      std::cout << "\nERROR\nERROR\nERROR";
    }
}

void QuenchThirst::Exit(Miner* miner) {
  std::cout << '\n'
            << GetNameOfEntity(miner->ID())
            << ": "
            << "Leavin' the saloon, feelin' good!";
}
