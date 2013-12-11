//Build a vector of features


typedef enum Decisions{
	Chancellor_DiscardDeck,
	Chancellor_DoNothing,


	//// higher level, broad decision
	//PREFER_BUY_ACTION_CARD_WITH_EFFECT,
	//PREFER_BUY_ACTION_CARD_ATTACK,
	//PREFER_BUY_ACTION_CARD_REACTION,
	//PREFER_BUY_ACTION_CARD_MORE_ACTIONS,
	//PREFER_BUY_ACTION_CARD_MORE_CARDS,
	//PREFER_BUY_ACTION_CARD_MORE_PREFER_BUYING_POWER,
	//PREFER_BUY_ACTION_CARD_PREFER_BUYING_POWER_BELOW_5,
	//PREFER_BUY_ACTION_CARD_PREFER_BUYING_POWER_ABOVE_5,
	//PREFER_BUY_TREASURE_CARD_PREFER_BUYING_POWER_BELOW_5,
	//PREFER_BUY_TREASURE_CARD_PREFER_BUYING_POWER_ABOVE_5,
	//PREFER_BUY_VICTORY_CARD_PREFER_BUYING_POWER_BELOW_5,
	//PREFER_BUY_VICTORY_CARD_PREFER_BUYING_POWER_ABOVE_5,
	//// PREFER_playing cards
	//PREFER_PLAY_ACTION_CARD_WITH_EFFECT,
	//PREFER_PLAY_ACTION_CARD_TRASH_CARD,
	//PREFER_PLAY_ACTION_CARD_DISCARD_CARDS,
	//PREFER_PLAY_ACTION_CARD_ATTACK,
	//PREFER_PLAY_ACTION_CARD_REACTION,
	//PREFER_PLAY_ACTION_CARD_MORE_ACTIONS,
	//PREFER_PLAY_ACTION_CARD_MORE_CARDS,
	//PREFER_PLAY_ACTION_CARD_MORE_PREFER_PLAYING_POWER,
	//// lower level - per card
	//PREFER_BUY_CARD_CHAPEL,
	//PREFER_BUY_CARD_CELLAR,
	//PREFER_BUY_CARD_MONEYLENDER,
	//PREFER_BUY_CARD_WORKSHOP,
	//PREFER_BUY_CARD_CHANCELLOR,
	//PREFER_BUY_CARD_MILITIA,
	//PREFER_BUY_CARD_COUNCILROOM,
	//PREFER_BUY_CARD_FEAST,
	//PREFER_BUY_CARD_REMODEL,
	//PREFER_BUY_CARD_LIBRARY,
	//PREFER_BUY_CARD_WITCH,
	//PREFER_BUY_CARD_MINE,
	//PREFER_BUY_CARD_GARDENS,
	//PREFER_BUY_CARD_ADVENTURER,
	//PREFER_BUY_CARD_BUREAUCRAT,
	//PREFER_BUY_CARD_SPY,
	//PREFER_BUY_CARD_THRONEROOM,
	//PREFER_BUY_FIRST_WITCH, PREFER_BUY_FIRST_MOAT,
	//PREFER_BUY_PROVINCES, PREFER_BUY_DUCHIES, PREFER_BUY_ESTATES,
	//PREFER_BUY_GOLD, PREFER_BUY_SILVER, PREFER_BUY_COPPER
	NUM_DECISIONS
};

typedef enum Feature{
	OPPONENT_HAS_ATTACK_CARDS, OPPONENT_HAS_MORE_THAN_THREE_ATTACK_CARDS,
	HAS_AVAILABLE_ACTIONS, HAS_TWO_AVAILABLE_ACTIONS, HAS_THREE_AVAILABLE_ACTIONS, //...
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


	NUM_FEATURES,
	//
};

 struct FeatureWeight{
	Feature type;
	bool enabled;
	double weight;

	FeatureWeight(Feature t) : type(t), enabled(false), weight(0.0){}

	FeatureWeight(Feature t, double myWeight) : type(t), enabled(true), weight(myWeight){}

};

class DecisionStrategy
{
public:
	DecisionStrategy();
	DecisionStrategy(DecisionStrategy &m);

	double getDecisionWeight(const State &s, Decisions d);

	DecisionStrategy* Mutate() const;
	//DecisionStrategy* Mutate(const CardDatabase &cards, const GameOptions &options) const;
private:
	Vector<Vector<FeatureWeight>> _decisionWeights;

};