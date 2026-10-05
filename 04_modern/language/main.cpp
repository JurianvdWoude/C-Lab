#include <array>
#include <concepts>
#include <iostream>
#include <memory>
#include <optional>
#include <ranges>
#include <span>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

template<std::integral T>
T add_with_integral(T a, T b) {
  return a + b;
}

template<typename T>
concept Addable = requires(T a, T b) {
  a + b;
};
template<Addable T>
T add_with_concept(T a, T b) {
  return a + b;
}

class Enemy {
public:
  int size;
  constexpr Enemy(int size) : size(size) {}
  virtual ~Enemy() = default;
  virtual bool attack() = 0;
  virtual int getSize() const = 0;
};

class Slime : public Enemy {
public:
  constexpr Slime(int size = 0) : Enemy(size) {}
  bool attack() override {
    return true;
  }
  constexpr int getSize() const override { return size; }
};

constexpr int square(int x) {
  return x * x;
}

std::optional<int> find_player(const std::vector<std::string>& players, std::string_view name) {
  for (int i = 0; i < static_cast<int>(players.size()); ++i) {
    if (players[i] == name) 
      return i;
  }
  return std::nullopt;
}

void print_numbers(std::span<const int> numbers) {
  std::cout << "[ ";
  for (const int n : numbers)
    std::cout << n << ' ';
  std::cout << "]";
}

struct DamageEvent {
  int amount;
};

struct HealEvent {
  int heal;
};

struct MessageEvent {
  std::string text;
};

// -------------------------

void practice_unique_pointers() {
  std::cout << "unique_ptr: ";
  auto slime = std::make_unique<Slime>();
  if(slime->attack()) 
    std::cout << "arrow, ";
  if((*slime).attack())
    std::cout << "deref, ";
  std::unique_ptr<Slime> other = std::move(slime);
  if (other) {
    std::cout << "move, ";
    Slime* raw = other.get();
    if((*raw).attack()) 
      std::cout << "get, ";
    other.reset();
    if(!other) 
      std::cout << "reset";

    raw = nullptr;
  }
  std::cout << '\n';
}


void practice_shared_pointers() {
  std::cout << "shared_ptr: ";
  auto slime = std::make_shared<Slime>();
  if(slime->attack()) 
    std::cout << "arrow, ";
  if((*slime).attack())
    std::cout << "deref, ";
  auto slime2 = slime;
  if(slime.use_count() == 2) 
    std::cout << "count 2, ";
  auto other = std::move(slime);
  if (other) {
    std::cout << "move, ";
    if (other.use_count() == 2)
      std::cout << "count 2, ";
    Slime* raw = other.get();
    if((*raw).attack())
      std::cout << "get, ";
    other.reset();
    if (slime2.use_count() == 1)
      std::cout << "reset, count 1";

    raw = nullptr;
  }
  std::cout << '\n';
}

void practice_weak_pointers() {
  std::cout << "weak_ptr: ";
  auto slime = std::make_shared<Slime>();
  std::weak_ptr<Slime> weak = slime;
  std::cout << "count " << slime.use_count() << ", ";
  if (auto alive = weak.lock()) { 
    if (alive->attack()) {
      std::cout << "lock, ";
      std::cout << "count " << slime.use_count() << ", ";
    }
  }
  slime.reset();
  std::cout << "reset shared, ";
  if (weak.expired()) 
    std::cout << "expired? true, ";
  if (auto alive = weak.lock()) {
    std::cout << "lock? weak is alive";
  } else {
    std::cout << "lock? weak is dead";
  }
  std::cout << '\n';
}

void practice_constexpr() {
  std::cout << '\n' << "ENTER A NUMBER: ";
  int input;
  std::cin >> input;
  std::cout << "constexpr: ";
  // Compile-time evaluation
  constexpr int result = square(5);
  static_assert(result == 25);
  std::cout << "compile-time functions, ";
  // Runtime evaluation
  if(square(input)) {
    std::cout << "runtime-time functions " 
              << input 
              << "^2 = " 
              << square(input) 
              << ", ";
  }
  constexpr Slime slime{10};
  static_assert(slime.getSize() == 10);
  static_assert(slime.getSize() * 2 == 20);
  std::cout << "members, ";

  constexpr auto add = [](int a, int b) {
    return a + b;
  };
  constexpr int sum = add(1, 2);
  static_assert(sum == 3);
  std::cout << "lambda";

  std::cout << '\n';
}

void practice_concepts() {
  std::cout << "template constraints: ";
  int integral_add = add_with_integral(1, 2);
  std::cout << "used std::integral "
            << "1 + 2 = " 
            << integral_add
            << ", ";
  // THE FOLLOWING WOULD CAUSE A COMPILE ERROR:
#if 0
  int double_add = add_with_integral(1.5, 2.5);
#endif
  int concept_add_i = add_with_concept(1, 2);
  std::cout << "used concept "
            << "1 + 2 = " 
            << concept_add_i
            << ", ";
  double concept_add_d = add_with_concept(1.2, 2.4);
  std::cout << "1.2 + 2.4 = "
            << concept_add_d
            << ", ";
  // THE FOLLOWING WOULD CAUSE A COMPILE ERROR:
#if 0
  std::string concept_add_s = add_with_concept("hello", "world");
#endif
  std::cout << '\n';
}

void practice_ranges() {
  std::cout << "ranges: ";
  std::vector<int> numbers{1,2,3,4,5,6,7,8};
  auto result = 
    numbers
    | std::views::filter([](int n) { return n % 2 == 0; })
    | std::views::filter([](int n) { return n > 4; });

  std::cout << "filter [ ";
  for (auto n : numbers)
    std::cout << n << ' ';
  std::cout << "]";
  std::cout << " -(n % 2 == 0)-(n > 4)-> ";
  std::cout << "[ ";
  for (auto n : result) 
    std::cout << n << ' ';
  std::cout << "]\n";

  numbers[2] = 10;
  std::cout << " cached begin, [ ";
  for (auto n : numbers)
    std::cout << n << ' ';
  std::cout << "]";
  std::cout << " -(n % 2 == 0)-(n > 4)-> ";
  std::cout << "[ ";
  for (auto n : result) 
    std::cout << n << ' ';
  std::cout << "]\n";

  auto result2 =
    numbers
    | std::views::transform([](int n) { return n * n; });
  std::cout << " transform     [ ";
  for (auto n : numbers)
    std::cout << n << ' ';
  std::cout << "]";
  std::cout << " -(n * n)-> ";
  std::cout << "[ ";
  for (auto n : result2) 
    std::cout << n << ' ';
  std::cout << "]";

  std::cout << '\n';
}

void practice_structured_bindings() {
  std::cout << "structured bindings: ";
  struct Player {
    std::string name;
    int health;
  };
  Player player{"Bob", 100};
  auto& [name, health] = player;
  name = "Jim";
  health -= 20;
  if (player.name == "Jim" && player.health == 80) 
    std::cout << " reassignment worked";
  std::cout << '\n';
}

void practice_optional() {
  std::cout << "std::optional: ";
  std::vector<std::string> players = {"Bob", "John", "Alice"};
  std::cout << "[ ";
  for (auto& player : players) {
    std::cout << "\"" << player << "\" ";
  }
  std::cout << "]";

  auto idx = find_player(players, "Alice");
  if (idx) {
    std::cout << ", Alice found (idx =" << *idx << ")";
  } else {
    std::cout << ", Alice not found (idx = std::nullopt)";
  }

  auto idx2 = find_player(players, "Jackson");
  if (idx2) {
    std::cout << ", Jackson found (idx =" << *idx2 << ")";
  } else {
    std::cout << ", Jackson not found (idx = std::nullopt)";
  }
  std::cout << '\n';
}

void practice_span() {
  std::cout << "std::span: ";
  std::vector<int> vec{1, 2, 3};
  std::array<int, 4> arr{4, 5, 6, 7};
  int raw[]{8, 9};
  
  std::cout << "printing vec(1,2,3),arr(4,5,6,7),raw[](8,9) = ";
  print_numbers(vec);
  print_numbers(arr);
  print_numbers(raw);
  std::cout << '\n';
}

void practice_variant() {
  using Event = std::variant<
    DamageEvent,
    HealEvent,
    MessageEvent
  >;

  std::cout << "std::variant: ";

  std::vector<Event> events{
    DamageEvent{20}, HealEvent{10}, MessageEvent{"ouch!"}, DamageEvent{5}
  };

  for (const auto& event : events) {
    std::visit([](const auto& value) {
      // strips away ref and const and stuff
      using T = std::decay_t<decltype(value)>;

      if constexpr (std::is_same_v<T, DamageEvent>) {
        std::cout << "damage " << value.amount;
      } else if constexpr (std::is_same_v<T, HealEvent>) {
        std::cout << "heal " << value.heal;
      } else if constexpr(std::is_same_v<T, MessageEvent>) {
        std::cout << "message \"" << value.text << "\"";
      }
      std::cout << ", ";
    }, event);
  }
}

int main() {
  practice_unique_pointers();
  practice_shared_pointers();
  practice_weak_pointers();
  practice_concepts();
  practice_ranges();
  practice_structured_bindings();
  practice_optional();
  practice_span();
  practice_variant();
  practice_constexpr();
  return 0;
}
