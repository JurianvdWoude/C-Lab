#include <chrono>
#include <thread>
#include "locations.h"
#include "entity.h"

void Sleep(int ms) {
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

int main() {
  Miner miner(ent_Miner_Bob);
  for (int i=0; i < 20; ++i) {
    miner.Update();
    Sleep(800);
  }
  return 0;
}
