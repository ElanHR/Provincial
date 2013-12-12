typedef enum PlayerType{
	RANDOM_PLAYER,
	HEURISTIC_PLAYER,
	STATEINFORMED_PLAYER
};

class Player
{
public:
	const PlayerType myPlayerType;
    virtual void MakeDecision(const State &s, DecisionResponse &response) = 0;
    virtual String ControllerName() = 0;
};

class PlayerHuman : public Player
{
public:
    void MakeDecision(const State &s, DecisionResponse &response)
    {
        SignalError("Human decisions are made from the host C# application!");
    }
    String ControllerName() { return "Human"; }

private:

};

class PlayerRandom : public Player
{
public:
    void MakeDecision(const State &s, DecisionResponse &response);
    String ControllerName() { return "Random AI"; }
};

class PlayerLearning: public Player
{
public:
	//LearningPlayer(const BuyAgenda *agenda);
	virtual ~PlayerLearning() {}
	
	virtual void MakeDecision(const State &s, DecisionResponse &response) = 0;
	virtual String ControllerName() = 0;
	virtual const BuyAgenda& Agenda() const = 0;
	virtual PlayerLearning* Mutate(const CardDatabase &cards, const GameOptions &options) const = 0;

};

class PlayerHeuristic : public PlayerLearning
{
public:
    PlayerHeuristic(const BuyAgenda *agenda);
    void MakeDecision(const State &s, DecisionResponse &response);
    String ControllerName() { return "Heuristic " + _buyAgenda->Name(); }
    const BuyAgenda& Agenda() const
    {
        return *_buyAgenda;
    }

	PlayerLearning* Mutate(const CardDatabase &cards, const GameOptions &options) const;

private:
    bool CardDesired(const State &s, int player, Card *c) const;
    void MakePhaseDecision(const State &s, DecisionResponse &response);
    void MakeBaseDecision(const State &s, DecisionResponse &response);
    void MakeIntrigueDecision(const State &s, DecisionResponse &response);
    void MakeSeasideDecision(const State &s, DecisionResponse &response);
    void MakeAlchemyDecision(const State &s, DecisionResponse &response);
    void MakeProsperityDecision(const State &s, DecisionResponse &response);
    void MakeCustomDecision(const State &s, DecisionResponse &response);

    void MakeCopyDecision(const State &s, DecisionResponse &response);
    void MakeDiscardDownDecision(const State &s, DecisionResponse &response);
    void MakeDeckReorderDecision(const State &s, DecisionResponse &response);
    void MakeTrashDecision(const State &s, DecisionResponse &response);
    
    Vector<Card*> TrashableCards(const State &s) const;
    UINT TrashableCardCount(const State &s) const;
    UINT DiscardableCardCount(const State &s) const;

    const BuyAgenda *_buyAgenda;
    int _remodelGoldThreshold;
};




class PlayerStateInformed : public PlayerLearning
{
public:
	//PlayerStateInformed(const BuyAgenda *agenda, const DecisionStrategy* strategy);
	PlayerStateInformed(const DecisionStrategy* strategy);

    void MakeDecision(const State &s, DecisionResponse &response);
	String ControllerName() { return "StateInformed " + _strategy->Name(); }
    const BuyAgenda& Agenda() const
    {
		return *_strategy;
    }

	PlayerLearning* Mutate(const CardDatabase &cards, const GameOptions &options) const;

protected:
    bool CardDesired(const State &s, int player, Card *c) const;
    void MakePhaseDecision(const State &s, DecisionResponse &response);
    void MakeBaseDecision(const State &s, DecisionResponse &response);
    void MakeIntrigueDecision(const State &s, DecisionResponse &response);
    void MakeSeasideDecision(const State &s, DecisionResponse &response);
    void MakeAlchemyDecision(const State &s, DecisionResponse &response);
    void MakeProsperityDecision(const State &s, DecisionResponse &response);
    void MakeCustomDecision(const State &s, DecisionResponse &response);

    void MakeCopyDecision(const State &s, DecisionResponse &response);
    void MakeDiscardDownDecision(const State &s, DecisionResponse &response);
    void MakeDeckReorderDecision(const State &s, DecisionResponse &response);
    void MakeTrashDecision(const State &s, DecisionResponse &response);
    
    Vector<Card*> TrashableCards(const State &s) const;
    UINT TrashableCardCount(const State &s) const;
    UINT DiscardableCardCount(const State &s) const;

	
	const DecisionStrategy* _strategy;
    //const BuyAgenda *_buyAgenda;

    int _remodelGoldThreshold;

	//Vector<Feature>* GetStateFeatureVector(const State &s);
};

class PlayerSmart : public Player
{
public:
    void MakeDecision(const State &s, DecisionResponse &response);
    String ControllerName() { return "Smart AI"; }
};