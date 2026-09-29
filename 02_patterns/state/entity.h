#pragma once

#include <string>
#include "locations.h"

class State;

inline constexpr int ComfortLevel = 5;
inline constexpr int MaxNuggets = 3;
inline constexpr int ThirstLevel = 5;
inline constexpr int TirednessThreshold = 5;

class BaseGameEntity {
  int m_ID;
  static int m_iNextValidID;
  void SetID(int val);

public:
  BaseGameEntity(int id) {
    SetID(id);
  }
  virtual ~BaseGameEntity() {};

  virtual void Update() = 0;
  int ID() const {
    return m_ID;
  }
};

class Miner : public BaseGameEntity {
  State* m_pCurrentState;
  location_type m_Location;
  int m_iGoldCarried;
  int m_iMoneyInBank;
  int m_iThirst;
  int m_iFatigue;

public:
  Miner(int ID);
  void Update() override;
  void ChangeState(State* pNewState);

  location_type Location() const {return m_Location;}
  void ChangeLocation(const location_type loc){m_Location = loc;}
  int GoldCarried()const{return m_iGoldCarried;}
  void SetGoldCarried(const int val){m_iGoldCarried = val;}
  void AddToGoldCarried(const int val);
  bool PocketsFull()const{return m_iGoldCarried >= MaxNuggets;}

  bool Fatigued()const;
  void DecreaseFatigue(){m_iFatigue -= 1;}
  void IncreaseFatigue(){m_iFatigue += 1;}

  int Wealth()const{return m_iMoneyInBank;}
  void SetWealth(const int val){m_iMoneyInBank = val;}
  void AddToWealth(const int val);

  bool Thirsty()const; 
  void BuyAndDrinkWhiskey(){m_iThirst = 0; m_iMoneyInBank-=2;}
};

enum {
  ent_Miner_Bob,
  ent_Elsa
};

std::string GetNameOfEntity(int n);
