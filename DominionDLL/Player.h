class Player
{
public:
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
};

class PlayerRandom : public Player
{
public:
    void MakeDecision(const State &s, DecisionResponse &response);
    String ControllerName() { return "Random AI"; }
};

class PlayerHeuristic : public Player
{
public:
    PlayerHeuristic(const BuyAgenda *agenda);
    void MakeDecision(const State &s, DecisionResponse &response);
    String ControllerName() { return "Heuristic " + _agenda->Name(); }
    const BuyAgenda& Agenda() const
    {
        return *_agenda;
    }

    PlayerHeuristic* Mutate(const CardDatabase &cards, const GameOptions &options) const;

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

    const BuyAgenda *_agenda;
    int _remodelGoldThreshold;
};




class PlayerStateInformed : public Player
{
public:
    PlayerStateInformed(const BuyAgenda *agenda);
    void MakeDecision(const State &s, DecisionResponse &response);
    String ControllerName() { return "Heuristic " + _agenda->Name(); }
    const BuyAgenda& Agenda() const
    {
        return *_agenda;
    }

    PlayerStateInformed* Mutate(const CardDatabase &cards, const GameOptions &options) const;

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

    const BuyAgenda *_agenda;
    int _remodelGoldThreshold;


	//Build a vector of features
	typedef struct StateFeature{
		bool enabled;
		double weight;

		StateFeature() : enabled(false), weight(0.0){}

		StateFeature(double myWeight) : enabled(true), weight(myWeight){}
	};

	enum StateFeatures {
		OPPONENT_HAS_ATTACK_CARDS, OPPONENT_HAS_MORE_THAN_THREE_ATTACK_CARDS,
		HAS_AVAILABLE_ACTIONS, HAS_TWO_AVAILABLE_ACTIONS, HAS_THREE_AVAILABLE_ACTIONS,
		BUY_WITCH_IF_PRESENT, BUY_MOAT_IF_PRESENT,
		BUY_PLUSONE_ACTION_CARD,
		BUY_DRAWCARDS_ACTION_CARD,
		BUY_PROVINCES, BUY_DUCHIES, BUY_ESTATES,
		BUY_GOLD, BUY_SILVER, BUY_COPPER,
		BUY_ACTION_CARD_WITH_EFFECT,
		BUY_ACTION_CARD_MORE_ACTIONS,
		BUY_ACTION_CARD_MORE_CARDS,
		BUY_ACTION_CARD_MORE_BUYING_POWER,
		BUY_ACTION_CARD_BUYING_POWER_BELOW_5,
		BUY_ACTION_CARD_BUYING_POWER_ABOVE_5,
		BUY_TREASURE_CARD_BUYING_POWER_BELOW_5,
		BUY_TREASURE_CARD_BUYING_POWER_ABOVE_5,
		BUY_VICTORY_CARD_BUYING_POWER_BELOW_5,
		BUY_VICTORY_CARD_BUYING_POWER_ABOVE_5,
		BUY_CARD_CHAPEL,
		BUY_CARD_CELLAR,
		BUY_CARD_MONEYLENDER,
		BUY_CARD_WORKSHOP,
		BUY_CARD_CHANCELLOR,
		BUY_CARD_MILITIA,
		BUY_CARD_COUNCILROOM,
		BUY_CARD_FEAST,
		BUY_CARD_REMODEL,
		BUY_CARD_LIBRARY,
		BUY_CARD_WITCH,
		BUY_CARD_MINE,
		BUY_CARD_GARDENS,
		BUY_CARD_ADVENTURER,
		BUY_CARD_BUREAUCRAT,
		BUY_CARD_SPY,
		BUY_CARD_THRONEROOM
	};


	Vector<StateFeature>* GetStateFeatureVector(const State &s);
};

class PlayerSmart : public Player
{
public:
    void MakeDecision(const State &s, DecisionResponse &response);
    String ControllerName() { return "Smart AI"; }
};