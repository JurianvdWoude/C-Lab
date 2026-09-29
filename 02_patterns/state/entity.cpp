#include <cassert>
#include "entity.h"
#include "state.h"

int BaseGameEntity::m_iNextValidID = 0;

void BaseGameEntity::SetID(int val) {
  assert((val >= m_iNextValidID) && "<BaseGameEntity::SetID>: invalid ID");
  m_ID = val;
  m_iNextValidID = m_ID + 1;
}

Miner::Miner(int id) :
  BaseGameEntity(id), 
  m_Location(shack),
  m_iGoldCarried(0),
  m_iMoneyInBank(0),
  m_iThirst(0),
  m_iFatigue(0),
  m_pCurrentState(GoHomeAndSleepTillRested::Instance())
{}

void Miner::Update() {
  m_iThirst += 1;

  if (m_pCurrentState) {
    m_pCurrentState->Execute(this);
  }
}

void Miner::ChangeState(State* pNewState) {
  assert(m_pCurrentState && pNewState);
  m_pCurrentState->Exit(this);
  m_pCurrentState = pNewState;
  m_pCurrentState->Enter(this);
}

void Miner::AddToGoldCarried(const int val) {
  m_iGoldCarried += val;
  if (m_iGoldCarried < 0) m_iGoldCarried = 0;
}

void Miner::AddToWealth(const int val) {
  m_iMoneyInBank += val;
  if (m_iMoneyInBank < 0) 
    m_iMoneyInBank = 0;
}

bool Miner::Thirsty() const {
  return m_iThirst >= ThirstLevel;
}

bool Miner::Fatigued() const {
  return m_iFatigue > TirednessThreshold;
}

std::string GetNameOfEntity(int n) {
  switch(n) {
    case ent_Miner_Bob:
      return "Miner Bob";
    case ent_Elsa:
      return "Elsa";
    default:
      return "UNKNOWN";
  }
}
