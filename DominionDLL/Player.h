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

	enum Decisions{
		Chancellor_DiscardDeck,
		Chancellor_DoNothing,

	};

	enum StateFeatures {
		OPPONENT_HAS_ATTACK_CARDS, OPPONENT_HAS_MORE_THAN_THREE_ATTACK_CARDS,
		HAS_AVAILABLE_ACTIONS, HAS_TWO_AVAILABLE_ACTIONS, HAS_THREE_AVAILABLE_ACTIONS,
		HAS_ONE_BUYING_POWER,
		HAS_TWO_BUYING_POWER,
		HAS_THREE_BUYING_POWER,
		HAS_FOUR_BUYING_POWER,
		HAS_FIVE_BUYING_POWER,
		HAS_SIX_BUYING_POWER,
		HAS_SEVEN_BUYING_POWER,
		HAS_EIGHT_BUYING_POWER,
		HAS_NINE_BUYING_POWER,
		HAS_TEN_BUYING_POWER,
		HAS_ELEVEN_BUYING_POWER,
		HAS_TWELVE_BUYING_POWER,
		HAS_THIRTEEN_BUYING_POWER,
		HAS_FOURTEEN_BUYING_POWER,
		HAS_MORE_THAN_FOURTEEN_BUYING_POWER,
		// higher level, broad decision
		PREFER_BUY_ACTION_CARD_WITH_EFFECT,
		PREFER_BUY_ACTION_CARD_ATTACK,
		PREFER_BUY_ACTION_CARD_REACTION,
		PREFER_BUY_ACTION_CARD_MORE_ACTIONS,
		PREFER_BUY_ACTION_CARD_MORE_CARDS,
		PREFER_BUY_ACTION_CARD_MORE_PREFER_BUYING_POWER,
		PREFER_BUY_ACTION_CARD_PREFER_BUYING_POWER_BELOW_5,
		PREFER_BUY_ACTION_CARD_PREFER_BUYING_POWER_ABOVE_5,
		PREFER_BUY_TREASURE_CARD_PREFER_BUYING_POWER_BELOW_5,
		PREFER_BUY_TREASURE_CARD_PREFER_BUYING_POWER_ABOVE_5,
		PREFER_BUY_VICTORY_CARD_PREFER_BUYING_POWER_BELOW_5,
		PREFER_BUY_VICTORY_CARD_PREFER_BUYING_POWER_ABOVE_5,
		// PREFER_playing cards
		PREFER_PLAY_ACTION_CARD_WITH_EFFECT,
		PREFER_PLAY_ACTION_CARD_TRASH_CARD,
		PREFER_PLAY_ACTION_CARD_DISCARD_CARDS,
		PREFER_PLAY_ACTION_CARD_ATTACK,
		PREFER_PLAY_ACTION_CARD_REACTION,
		PREFER_PLAY_ACTION_CARD_MORE_ACTIONS,
		PREFER_PLAY_ACTION_CARD_MORE_CARDS,
		PREFER_PLAY_ACTION_CARD_MORE_PREFER_PLAYING_POWER,
		// lower level - per card
		PREFER_BUY_CARD_CHAPEL,
		PREFER_BUY_CARD_CELLAR,
		PREFER_BUY_CARD_MONEYLENDER,
		PREFER_BUY_CARD_WORKSHOP,
		PREFER_BUY_CARD_CHANCELLOR,
		PREFER_BUY_CARD_MILITIA,
		PREFER_BUY_CARD_COUNCILROOM,
		PREFER_BUY_CARD_FEAST,
		PREFER_BUY_CARD_REMODEL,
		PREFER_BUY_CARD_LIBRARY,
		PREFER_BUY_CARD_WITCH,
		PREFER_BUY_CARD_MINE,
		PREFER_BUY_CARD_GARDENS,
		PREFER_BUY_CARD_ADVENTURER,
		PREFER_BUY_CARD_BUREAUCRAT,
		PREFER_BUY_CARD_SPY,
		PREFER_BUY_CARD_THRONEROOM,
		PREFER_BUY_FIRST_WITCH, PREFER_BUY_FIRST_MOAT,
		PREFER_BUY_PROVINCES, PREFER_BUY_DUCHIES, PREFER_BUY_ESTATES,
		PREFER_BUY_GOLD, PREFER_BUY_SILVER, PREFER_BUY_COPPER,
	};


	Vector<StateFeature>* GetStateFeatureVector(const State &s);
};

class PlayerSmart : public Player
{
public:
    void MakeDecision(const State &s, DecisionResponse &response);
    String ControllerName() { return "Smart AI"; }
};