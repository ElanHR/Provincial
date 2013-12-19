//Build a vector of features
#include <random>
#include "Main.h"

static double mutateVariance = 3, initVariance = 20;
const static int initialNumFeaturesPerDecision = 3;
static std::random_device rd;
static std::mt19937 gen(rd());	
static std::normal_distribution<> initGauss(0, initVariance);

typedef enum Decisions{
	// playing each card in the base game:
	Cellar_Play,
	Chapel_Play,
	Moat_Play,
	Chancellor_Play,
	Village_Play,
	Woodcutter_Play,
	Workshop_Play,
	Bureaucrat_Play,
	Feast_Play,
	// gardens no action
	Militia_Play,
	Moneylender_Play,
	Remodel_Play,
	Smithy_Play,
	Spy_Play,
	Thief_Play,
	ThroneRoom_Play,
	CouncilRoom_Play,
	Festival_Play,
	Laboratory_Play,
	Library_Play,
	Market_Play,
	Mine_Play,
	Witch_Play,
	Adventurer_Play,

	Play_Nothing,

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
	NUM_DECISIONS,
	INVALID_DECISION
};

typedef enum Feature{
	DECISION_CONSTANT,
	// deck features
	MONEY_DENSITY_OF_DECK,
	MONEY_DENSITY_OF_DECK_GREATER_THAN_1,
	MONEY_DENSITY_OF_DECK_GREATER_THAN_1_25,
	MONEY_DENSITY_OF_DECK_GREATER_THAN_1_5,
	ACTION_DENSITY_OF_DECK,
	VICTORY_DENSITY_OF_DECK,
	// turn features
	NUM_AVAILABLE_ACTIONS,
	NUM_AVAILABLE_BUYS,
	// hand features
	NUM_CARDS_IN_HAND,
	NUM_ACTION_CARDS_IN_HAND,
	NUM_VICTORY_CARDS_IN_HAND,
	NUM_TREASURE_CARDS_IN_HAND,
	NUM_CURSE_CARDS_IN_HAND,
	NUM_COPPER_CARDS_IN_HAND,
	HAS_ONE_COPPER_IN_HAND,
	BUYING_POWER,
	// game ending conditions:
	ONE_KINGDOM_PILE_EMPTY, TWO_KINGDOM_PILES_EMPTY, NUM_PROVINCES_LEFT,
	// TODO supply features ?!
	// TODO opponent deck features?
	OPP_MOAT_DENSITY_OF_OPP_DECK,
	OPP_TREASURE_DENSITY_OF_OPP_DECK,
	OPP_MOAT_DENSITY_OF_OPP_DECK_IS_ZERO,
	OPP_HAND_SIZE_LARGER_THAN_THREE,

	NUM_FEATURES,
	INVALID_FEATURE


	//HAS_ONE_PLUS_AVAILABLE_ACTIONS, HAS_TWO_PLUS_AVAILABLE_ACTIONS, HAS_THREE_PLUS_AVAILABLE_ACTIONS,
	//HAS_FOUR_PLUS_AVAILABLE_ACTIONS,
	//HAS_ONE_ACTION_TO_PLAY, HAS_TWO_ACTIONS_TO_PLAY, HAS_THREE_ACTIONS_TO_PLAY, 
	//HAS_FOUR_PLUS_ACTIONS_TO_PLAY,
	//HAS_ONE_BUYING_POWER, HAS_TWO_PLUS_BUYING_POWER, HAS_THREE_PLUS_BUYING_POWER, 
	//HAS_FOUR_PLUS_BUYING_POWER, HAS_FIVE_PLUS_BUYING_POWER,
	//HAS_SIX_PLUS_BUYING_POWER, HAS_SEVEN_PLUS_BUYING_POWER, 
	//HAS_EIGHT_PLUS_BUYING_POWER, HAS_NINE_PLUS_BUYING_POWER, HAS_TEN_PLUS_BUYING_POWER,
	//HAS_ELEVEN_PLUS_BUYING_POWER, HAS_TWELVE_PLUS_BUYING_POWER, 
	//HAS_THIRTEEN_PLUS_BUYING_POWER, HAS_FOURTEEN_PLUS_BUYING_POWER,
	//HAS_FOURTEEN_PLUS_BUYING_POWER,
	//NUM_CARDS_LOWEST_KINGDOM_PILE, NUM_CARDS_SECOND_LOWEST_KINGDOM_PILE,
	//NUM_CARDS_THIRD_LOWEST_KINGDOM_PILE, NUM_CARDS_FOURTH_LOWEST_KINGDOM_PILE,
	//NUM_CARDS_FIFTH_LOWEST_KINGDOM_PILE, NUM_CARDS_SIXTH_LOWEST_KINGDOM_PILE,
	//NUM_CARDS_SEVENTH_LOWEST_KINGDOM_PILE, NUM_CARDS_EIGHTH_LOWEST_KINGDOM_PILE,
	//NUM_CARDS_NINTH_LOWEST_KINGDOM_PILE, NUM_CARDS_HIGHEST_LOWEST_KINGDOM_PILE,
	//OPPONENT_HAS_ATTACK_CARDS, OPPONENT_HAS_MORE_THAN_THREE_ATTACK_CARDS,
};

 struct FeatureWeight{
	Feature type;
	//bool enabled;
	double weight;


	FeatureWeight() : type(INVALID_FEATURE), weight(-1){}

	FeatureWeight(Feature t) : type(t), weight(0.0){}

	FeatureWeight(Feature t, double myWeight) : type(t),  weight(myWeight){}
	

};

 class DecisionStrategy : public BuyAgendaMenu
{
public:

	DecisionStrategy(const CardDatabase &cards, const String &s);
	DecisionStrategy(const CardDatabase &cards, const String &s, const String &decString);

	DecisionStrategy(BuyMenu &m);
	DecisionStrategy(BuyMenu &m, Vector<Vector<FeatureWeight>*>* fws);

	// for initial creation?
	DecisionStrategy(const CardDatabase &cards, const GameOptions &options);

	DecisionStrategy(const CardDatabase &cards, const GameOptions &options, Card *a, Card *b);

	Vector<Feature>* getPertinentFeatures(Decisions d) const;

	double getDecisionWeight(const State &s, Decisions d) const;

	DecisionStrategy* Mutate(const CardDatabase &cards, const GameOptions &options) const;

	DecisionStrategy* MutateOnlyBuys(const CardDatabase &cards, const GameOptions &options) const;
	DecisionStrategy* MutateOnlyDecisions(const CardDatabase &cards, const GameOptions &options) const;

	String getStringInfo() const;
	Decisions getActionPlayDecision(Card* c) const;

	String Name() const
	{
		String d = "";

		String s = "e" + String(_m.estateBuyThreshold) + "-d" + String(_m.duchyBuyThreshold) + "-p" + String(_m.provinceBuyThreshold) + ":";
		for (BuyMenuEntry entry : _m.entries)
		{
			s += entry.ToString() + "-";
		}
		s.PopEnd();
		return getStringInfo() + "__" + s;
	}

	void LoadDecisionWeightsFromFile(const String &s);
	void SaveDecisionWeightsToFile(const String &s) const;
private:

	void Init();
	void Init(const String &decString);
	

	Vector<Vector<FeatureWeight>*>* _decisionWeights;

};
