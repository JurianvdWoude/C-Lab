#using "<string>"

class GameActor;

class Command {
public:
  virtual ~Command() {};
  virtual void execute(GameActor& actor) = 0;
  virtual std::string name() const = 0;
};


class JumpCommand : public Command {
public:
  void execute(GameActor& actor) override;
  std::string name() const override { return "Jump"; }
};

class MoveLeftCommand : public Command {
public:
  void execute(GameActor& actor) override;
  std::string name() const override { return "Move Left"; }
};

class MoveRightCommand : public Command {
public:
  void execute(GameActor& actor) override;
  std::string name() const override { return "Move Right"; }
};

class CrouchCommand : public Command {
public:
  void execute(GameActor& actor) override;
  std::string name() const override { return "Crouch"; }
};
