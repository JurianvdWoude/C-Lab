
template <class T>
class StateMachine {
  T* m_pOwner;
  State<T>* m_pCurrentState;
  State<T>* m_pPreviousState;
  State<T>* m_pGlobalState;

public:
  StateMachine(T* owner): m_pOwner(owner),
                          m_pCurrentState(nullptr),
                          m_pPreviousState(nullptr),
                          m_pGlobalState(nullptr) {}

  void SetCurrentState(State<T>* state) m_pCurrentState = state;
  void SetGlobalState(State<T>* state) m_pGlobalState = state;
  void SetPreviousState(State<T>* state) m_pPreviousState = state;

  void Update() const {
    if(m_pGlobalState) m_pGlobalState->Execute(m_pOwner);
    if(m_pCurrentState) m_pCurrentState->Execute(m_pOwner);
  }

 /*  Change the state of the entity. 
  *
  *  1. store the current state
  *  2. trigger the exit function of the current state
  *  3. set the new state as the current state
  *  4. trigger the enter function of the new state
  *
  *  @param pNewState : The new state that the entity is in.
  */
  void ChangeState(State<T>* pNewState) {
    assert(pNewState && "<StateMachine::ChangeState: trying to change to a null state");

    m_pPreviousState = m_pCurrentState;
    m_pCurrentState->Exit(m_pOwner);
    m_pCurrentState = pNewState;
    m_pCurrentState->Enter(m_pOwner);
  }

  void RevertToPreviousState() {
    ChangeState(m_pPreviousState);
  }

  State<T>* CurrentState() const { return m_pCurrentState; }
  State<T>* GlobalState() const { return m_pGlobalState; }
  State<T>* PreviousState() const { return m_pPreviousState; }

  bool isInState(const State<T>& state) const;
}
