#include "Main.h"

DecisionStrategy::DecisionStrategy(const CardDatabase &cards, const String &s) : BuyAgendaMenu(cards, s){
	//Console::WriteLine("ConA - " + s);
	Init();
}

DecisionStrategy::DecisionStrategy(const CardDatabase &cards, const String &s, const String &decString) : BuyAgendaMenu(cards, s){
	//Console::WriteLine("ConB - " + s);
	Init(decString);
}

// copying the other one, should copy 
DecisionStrategy::DecisionStrategy(BuyMenu &m) : BuyAgendaMenu(m)
{
	//Console::WriteLine("ConC - " );
	Init(); // this should be copying the old
	_decisionWeights->FreeMemory();
}


void DecisionStrategy::LoadDecisionWeightsFromFile(const String &s)
{
	ifstream file(("decisions"+s).CString());

	for (int d = 0; d < NUM_DECISIONS; d++){
		char* curLine = new char[1024];
		file.getline(curLine,1024);

		String cur(curLine);
		Vector<String> partioned = cur.Partition(' ');
		if (partioned.Length() != NUM_FEATURES)
			Console::WriteLine("UHOH!!! File does not have proper number of features.");

		for (int f = 0; f < NUM_FEATURES; f++){
			_decisionWeights->at(d)->at(f).weight = partioned.at(f).ConvertToDouble();
		}

		free(curLine);
	}
	

}

void DecisionStrategy::SaveDecisionWeightsToFile(const String &s) const
{
	Console::WriteLine("Printing out decision file for " + s);
	ofstream file(("C:\\Users\\Elan\\Source\\Repos\\Provincial\\bin\\data\\decisions" + s).CString());

	if (_decisionWeights == NULL){
		file << " DECISION_WEIGHTS IS NULL FOR " << s;
	}else{
	for (int d = 0; d < NUM_DECISIONS; d++){
		for (int f = 0; f < NUM_FEATURES; f++){
			file << _decisionWeights->at(d)->at(f).weight << " ";
		}
		file << endl;
	}
	}


}


DecisionStrategy::DecisionStrategy(BuyMenu &m, Vector<Vector<FeatureWeight>*>* fws) : BuyAgendaMenu(m)
{
	Init(); // this should be copying the old
	_decisionWeights->FreeMemory();
	_decisionWeights->Append(*fws);
}

DecisionStrategy::DecisionStrategy(const CardDatabase &cards, const GameOptions &options) : BuyAgendaMenu(cards, options){
	Init();
}

DecisionStrategy::DecisionStrategy(const CardDatabase &cards, const GameOptions &options, Card *a, Card *b) : BuyAgendaMenu(cards, options, a, b){
	Init();
}

DecisionStrategy::~DecisionStrategy(){

	for (unsigned int i = 0; i < _decisionWeights->Length(); i++)
		delete _decisionWeights->at(i);

	delete _decisionWeights;
}

void DecisionStrategy::Init(){
	_decisionWeights = new Vector<Vector<FeatureWeight>*>();
	for (int d = 0; d < NUM_DECISIONS; d++){
		_decisionWeights->PushEnd(new Vector<FeatureWeight>());
		// only add pertinent features
		Vector<Feature>* pertinentFeatures = getPertinentFeatures(static_cast<Decisions>(d));
		for (UINT f = 0; f < pertinentFeatures->Length(); f++) {
			Feature feature = pertinentFeatures->at(f);
			FeatureWeight fw(feature, initGauss(gen));
			_decisionWeights->at(d)->PushEnd(fw);
		}
	}
}

void DecisionStrategy::Init(const String &decisions){
	_decisionWeights = new Vector<Vector<FeatureWeight>*>();

	// decision:
	regex decRegex("\\{d:(\\d+)(.*?)\\}");
	// feature and weight
	regex fwtRegex("\\(f:(\\d+) wt:((?:-|\\.|\\d)+)\\)");
	Console::WriteLine("before convert:\n" + decisions);
	std::string d(decisions.CString(), decisions.Length());
	Console::WriteString("after convert:\n");
	Console::WriteString(d.c_str());
	Console::WriteLine("");
	auto decBegin = sregex_iterator(d.begin(), d.end(), decRegex);
	auto decEnd = sregex_iterator();
	Console::WriteLine("Found num decisions:" + String(std::distance(decBegin, decEnd)));
	// for each decision:
	UINT dIndex = 0;
	for (std::sregex_iterator it = decBegin; it != decEnd; ++it) {
		std::smatch match = *it;
		std::string match_str = match.str();
		Console::WriteString("found decision:");
		Console::WriteLine(match_str.c_str());
		// add a vector to decision weights
		_decisionWeights->PushEnd(new Vector<FeatureWeight>());
		// for each feature weight:
		auto featureBegin = sregex_iterator(match_str.begin(), match_str.end(), fwtRegex);
		auto featureEnd = sregex_iterator();
		Console::WriteLine("Found num fwts:" + String(std::distance(featureBegin, featureEnd)));
		for (std::sregex_iterator itf = featureBegin; itf != featureEnd; ++itf) {
			// get the matches
			std::smatch fmatch = *itf;
			std::string match_f = fmatch[0];
			std::string match_wt = fmatch[1];
			Console::WriteString("f:");
			Console::WriteString(match_f.c_str());
			Console::WriteString(" wt:");
			Console::WriteString(match_wt.c_str());
			Console::WriteString("\n");
			// and add them!
			Feature f = static_cast<Feature>(std::stoi(match_f));
			double weight = std::stod(match_wt);
			FeatureWeight fw(f, weight);
			_decisionWeights->at(dIndex)->PushEnd(fw);
		}
		dIndex++;
	}
}

String DecisionStrategy::getStringInfo() const {
	String result = "";
	int d = 0;
	for (Vector<FeatureWeight>* v : *_decisionWeights) {
		result += "{";
		result += "d:" + String(d);
		for (FeatureWeight f : *v) {
			result += ("(f:" + String(f.type)) + (" wt:" + String(f.weight) + ")"); 
		}
		result += "}";
		d++;
	}
	return result;
}

Decisions DecisionStrategy::getActionPlayDecision(Card* c) const {
	String name = c->name;
	//Console::WriteLine("get play decision for card:" + name);
	if (name == "cellar") {
		return Cellar_Play;
	}
	else if (name == "chapel") {
		return Chapel_Play;
	}
	else if (name == "moat") {
		return Moat_Play;
	}
	else if (name == "chancellor") {
		return Chancellor_Play;
	}
	else if (name == "village") {
		return Village_Play;
	}
	else if (name == "woodcutter") {
		return Woodcutter_Play;
	}
	else if (name == "workshop") {
		return Workshop_Play;
	}
	else if (name == "bureaucrat") {
		return Bureaucrat_Play;
	}
	else if (name == "feast") {
		return Feast_Play;
	}
	else if (name == "militia") {
		return Militia_Play;
	}
	else if (name == "moneylender") {
		return Moneylender_Play;
	}
	else if (name == "remodel") {
		return Remodel_Play;
	}
	else if (name == "smithy") {
		return Smithy_Play;
	}
	else if (name == "spy") {
		return Spy_Play;
	}
	else if (name == "thief") {
		return Thief_Play;
	}
	else if (name == "throne room") {
		return ThroneRoom_Play;
	}
	else if (name == "council room") {
		return CouncilRoom_Play;
	}
	else if (name == "festival") {
		return Festival_Play;
	}
	else if (name == "laboratory") {
		return Laboratory_Play;
	}
	else if (name == "library") {
		return Library_Play;
	}
	else if (name == "market") {
		return Market_Play;
	}
	else if (name == "mine") {
		return Mine_Play;
	}
	else if (name == "witch") {
		return Witch_Play;
	}
	else if (name == "adventurer") {
		return Adventurer_Play;
	}
	else{ // default
		Console::WriteLine("NO DECISION FOR: " + name);
		return INVALID_DECISION;
	}
}

Vector<Feature>* DecisionStrategy::getPertinentFeatures(Decisions d) const {
	Vector<Feature>* v = new Vector<Feature>();
	switch (d) {
	case Cellar_Play:
		// play features
		v->PushEnd(DECISION_CONSTANT);
		// turn features
		v->PushEnd(NUM_AVAILABLE_BUYS);
		v->PushEnd(NUM_AVAILABLE_ACTIONS);
		// some hand features - worth it to discard?
		v->PushEnd(NUM_VICTORY_CARDS_IN_HAND);
		v->PushEnd(NUM_ACTION_CARDS_IN_HAND);
		v->PushEnd(NUM_COPPER_CARDS_IN_HAND);
		v->PushEnd(BUYING_POWER);
		// deck features - worth it to draw?
		v->PushEnd(MONEY_DENSITY_OF_DECK);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1_25);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1_5);
		v->PushEnd(ACTION_DENSITY_OF_DECK);
		v->PushEnd(VICTORY_DENSITY_OF_DECK);
		break;
	case Chapel_Play:
		v->PushEnd(DECISION_CONSTANT);
		// turn features
		v->PushEnd(NUM_AVAILABLE_BUYS);
		v->PushEnd(NUM_AVAILABLE_ACTIONS);
		// hand features - worth it to trash?
		v->PushEnd(NUM_ACTION_CARDS_IN_HAND);
		v->PushEnd(NUM_CURSE_CARDS_IN_HAND);
		v->PushEnd(NUM_COPPER_CARDS_IN_HAND);
		break;
	case Moat_Play:
		v->PushEnd(DECISION_CONSTANT);
		// turn features
		v->PushEnd(NUM_AVAILABLE_BUYS);
		v->PushEnd(NUM_AVAILABLE_ACTIONS);
		// some hand features - worth it to play and draw?
		v->PushEnd(NUM_ACTION_CARDS_IN_HAND);
		v->PushEnd(BUYING_POWER);
		// deck features
		v->PushEnd(MONEY_DENSITY_OF_DECK);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1_25);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1_5);
		v->PushEnd(ACTION_DENSITY_OF_DECK);
		v->PushEnd(VICTORY_DENSITY_OF_DECK);
		break;
	case Chancellor_Play:
		v->PushEnd(DECISION_CONSTANT);
		// turn features
		v->PushEnd(NUM_AVAILABLE_BUYS);
		v->PushEnd(NUM_AVAILABLE_ACTIONS);
		// some hand features
		v->PushEnd(NUM_ACTION_CARDS_IN_HAND);
		// deck features
		v->PushEnd(MONEY_DENSITY_OF_DECK);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1_25);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1_5);
		v->PushEnd(ACTION_DENSITY_OF_DECK);
		v->PushEnd(VICTORY_DENSITY_OF_DECK);
		break;
	case Village_Play:
		v->PushEnd(DECISION_CONSTANT);
		// turn features
		v->PushEnd(NUM_AVAILABLE_BUYS);
		v->PushEnd(NUM_AVAILABLE_ACTIONS);
		// some hand features
		v->PushEnd(NUM_ACTION_CARDS_IN_HAND);
		v->PushEnd(BUYING_POWER);
		// deck features
		v->PushEnd(MONEY_DENSITY_OF_DECK);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1_25);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1_5);
		v->PushEnd(ACTION_DENSITY_OF_DECK);
		v->PushEnd(VICTORY_DENSITY_OF_DECK);
		break;
	case Woodcutter_Play:
		v->PushEnd(DECISION_CONSTANT);
		// turn features
		v->PushEnd(NUM_AVAILABLE_BUYS);
		v->PushEnd(NUM_AVAILABLE_ACTIONS);
		// some hand features
		v->PushEnd(NUM_ACTION_CARDS_IN_HAND);
		v->PushEnd(BUYING_POWER);
		break;
	case Workshop_Play:
		v->PushEnd(DECISION_CONSTANT);
		// turn features
		v->PushEnd(NUM_AVAILABLE_ACTIONS);
		// some hand features
		v->PushEnd(NUM_ACTION_CARDS_IN_HAND);
		// is it worth it to gain a card?
		// TODO add supply features?
		break;
	case Bureaucrat_Play:
		v->PushEnd(DECISION_CONSTANT);
		// turn features
		v->PushEnd(NUM_AVAILABLE_ACTIONS);
		// some hand features
		v->PushEnd(NUM_ACTION_CARDS_IN_HAND);
		// opp features
		v->PushEnd(OPP_MOAT_DENSITY_OF_OPP_DECK);
		v->PushEnd(OPP_MOAT_DENSITY_OF_OPP_DECK_IS_ZERO);
		break;
	case Feast_Play:
		v->PushEnd(DECISION_CONSTANT);
		// turn features
		v->PushEnd(NUM_AVAILABLE_ACTIONS);
		// some hand features
		v->PushEnd(NUM_ACTION_CARDS_IN_HAND);
		// is it worth it to gain a card?
		// TODO add supply features?
		break;
	case Militia_Play:
		v->PushEnd(DECISION_CONSTANT);
		// turn features
		v->PushEnd(NUM_AVAILABLE_ACTIONS);
		// some hand features
		v->PushEnd(NUM_ACTION_CARDS_IN_HAND);
		v->PushEnd(BUYING_POWER);
		// opp features
		v->PushEnd(OPP_MOAT_DENSITY_OF_OPP_DECK);
		v->PushEnd(OPP_MOAT_DENSITY_OF_OPP_DECK_IS_ZERO);
		v->PushEnd(OPP_HAND_SIZE_LARGER_THAN_THREE);
		break;
	case Moneylender_Play:
		v->PushEnd(DECISION_CONSTANT);
		// turn features
		v->PushEnd(NUM_AVAILABLE_BUYS);
		v->PushEnd(NUM_AVAILABLE_ACTIONS);
		// some hand features
		v->PushEnd(NUM_ACTION_CARDS_IN_HAND);
		v->PushEnd(HAS_ONE_COPPER_IN_HAND);
		break;
	case Remodel_Play:
		v->PushEnd(DECISION_CONSTANT);
		// turn features
		v->PushEnd(NUM_AVAILABLE_ACTIONS);
		// some hand features
		v->PushEnd(NUM_ACTION_CARDS_IN_HAND);
		// TODO add supply features?
		break;
	case Smithy_Play:
		v->PushEnd(DECISION_CONSTANT);
		// turn features
		v->PushEnd(NUM_AVAILABLE_BUYS);
		v->PushEnd(NUM_AVAILABLE_ACTIONS);
		// some hand features
		v->PushEnd(NUM_ACTION_CARDS_IN_HAND);
		v->PushEnd(BUYING_POWER);
		// deck features
		v->PushEnd(MONEY_DENSITY_OF_DECK);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1_25);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1_5);
		v->PushEnd(ACTION_DENSITY_OF_DECK);
		v->PushEnd(VICTORY_DENSITY_OF_DECK);
		break;
	case Spy_Play:
		v->PushEnd(DECISION_CONSTANT);
		// turn features
		v->PushEnd(NUM_AVAILABLE_BUYS);
		v->PushEnd(NUM_AVAILABLE_ACTIONS);
		// some hand features
		v->PushEnd(NUM_ACTION_CARDS_IN_HAND);
		v->PushEnd(BUYING_POWER);
		// opp features
		v->PushEnd(OPP_MOAT_DENSITY_OF_OPP_DECK);
		v->PushEnd(OPP_MOAT_DENSITY_OF_OPP_DECK_IS_ZERO);
		// deck features
		v->PushEnd(MONEY_DENSITY_OF_DECK);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1_25);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1_5);
		v->PushEnd(ACTION_DENSITY_OF_DECK);
		v->PushEnd(VICTORY_DENSITY_OF_DECK);
		break;
	case Thief_Play:
		v->PushEnd(DECISION_CONSTANT);
		// turn features
		v->PushEnd(NUM_AVAILABLE_ACTIONS);
		// some hand features
		v->PushEnd(NUM_ACTION_CARDS_IN_HAND);
		// opp features
		v->PushEnd(OPP_MOAT_DENSITY_OF_OPP_DECK);
		v->PushEnd(OPP_TREASURE_DENSITY_OF_OPP_DECK);
		v->PushEnd(OPP_MOAT_DENSITY_OF_OPP_DECK_IS_ZERO);
		break;
	case ThroneRoom_Play:
		v->PushEnd(DECISION_CONSTANT);
		// turn features
		v->PushEnd(NUM_AVAILABLE_ACTIONS);
		// some hand features
		v->PushEnd(NUM_ACTION_CARDS_IN_HAND);
		break;
	case CouncilRoom_Play:
		v->PushEnd(DECISION_CONSTANT);
		// turn features
		v->PushEnd(NUM_AVAILABLE_BUYS);
		v->PushEnd(NUM_AVAILABLE_ACTIONS);
		// some hand features
		v->PushEnd(BUYING_POWER);
		v->PushEnd(NUM_ACTION_CARDS_IN_HAND);
		// deck features
		v->PushEnd(MONEY_DENSITY_OF_DECK);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1_25);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1_5);
		v->PushEnd(ACTION_DENSITY_OF_DECK);
		v->PushEnd(VICTORY_DENSITY_OF_DECK);
		//TODO consider opponent deck
		break;
	case Festival_Play:
		v->PushEnd(DECISION_CONSTANT);
		// turn features
		v->PushEnd(NUM_AVAILABLE_BUYS);
		v->PushEnd(NUM_AVAILABLE_ACTIONS);
		// some hand features
		v->PushEnd(BUYING_POWER);
		v->PushEnd(NUM_ACTION_CARDS_IN_HAND);
		break;
	case Laboratory_Play:
		v->PushEnd(DECISION_CONSTANT);
		// turn features
		v->PushEnd(NUM_AVAILABLE_BUYS);
		v->PushEnd(NUM_AVAILABLE_ACTIONS);
		// some hand features
		v->PushEnd(NUM_ACTION_CARDS_IN_HAND);
		v->PushEnd(BUYING_POWER);
		// deck features
		v->PushEnd(MONEY_DENSITY_OF_DECK);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1_25);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1_5);
		v->PushEnd(ACTION_DENSITY_OF_DECK);
		v->PushEnd(VICTORY_DENSITY_OF_DECK);
		break;
	case Library_Play:
		v->PushEnd(DECISION_CONSTANT);
		// turn features
		v->PushEnd(NUM_AVAILABLE_ACTIONS);
		// some hand features
		v->PushEnd(NUM_ACTION_CARDS_IN_HAND);
		// deck features
		v->PushEnd(MONEY_DENSITY_OF_DECK);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1_25);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1_5);
		v->PushEnd(ACTION_DENSITY_OF_DECK);
		v->PushEnd(VICTORY_DENSITY_OF_DECK);
		break;
	case Market_Play:
		v->PushEnd(DECISION_CONSTANT);
		// turn features
		v->PushEnd(NUM_AVAILABLE_BUYS);
		v->PushEnd(NUM_AVAILABLE_ACTIONS);
		// some hand features
		v->PushEnd(NUM_ACTION_CARDS_IN_HAND);
		v->PushEnd(BUYING_POWER);
		// deck features
		v->PushEnd(MONEY_DENSITY_OF_DECK);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1_25);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1_5);
		v->PushEnd(ACTION_DENSITY_OF_DECK);
		v->PushEnd(VICTORY_DENSITY_OF_DECK);
		break;
	case Mine_Play:
		v->PushEnd(DECISION_CONSTANT);
		// turn features
		v->PushEnd(NUM_AVAILABLE_ACTIONS);
		v->PushEnd(NUM_AVAILABLE_BUYS);
		// some hand features
		v->PushEnd(NUM_ACTION_CARDS_IN_HAND);
		v->PushEnd(NUM_TREASURE_CARDS_IN_HAND);
		break;
	case Witch_Play:
		v->PushEnd(DECISION_CONSTANT);
		// turn features
		v->PushEnd(NUM_AVAILABLE_ACTIONS);
		// some hand features
		v->PushEnd(NUM_ACTION_CARDS_IN_HAND);
		// opp features
		v->PushEnd(OPP_MOAT_DENSITY_OF_OPP_DECK);
		v->PushEnd(OPP_MOAT_DENSITY_OF_OPP_DECK_IS_ZERO);
		// deck features
		v->PushEnd(MONEY_DENSITY_OF_DECK);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1_25);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1_5);
		v->PushEnd(ACTION_DENSITY_OF_DECK);
		v->PushEnd(VICTORY_DENSITY_OF_DECK);
		break;
	case Adventurer_Play:
		v->PushEnd(DECISION_CONSTANT);
		// turn features
		v->PushEnd(NUM_AVAILABLE_ACTIONS);
		v->PushEnd(NUM_AVAILABLE_BUYS);
		// some hand features
		v->PushEnd(NUM_ACTION_CARDS_IN_HAND);
		v->PushEnd(NUM_TREASURE_CARDS_IN_HAND);
		// deck features
		v->PushEnd(MONEY_DENSITY_OF_DECK);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1_25);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1_5);
		break;
	case Play_Nothing:
		v->PushEnd(DECISION_CONSTANT);
		break;
	// card decisioms
	case Chancellor_DiscardDeck:
		v->PushEnd(DECISION_CONSTANT);
		// deck features
		v->PushEnd(MONEY_DENSITY_OF_DECK);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1_25);
		v->PushEnd(MONEY_DENSITY_OF_DECK_GREATER_THAN_1_5);
		v->PushEnd(ACTION_DENSITY_OF_DECK);
		v->PushEnd(VICTORY_DENSITY_OF_DECK);
		break;
	case Chancellor_DoNothing:
		v->PushEnd(DECISION_CONSTANT);
		break;
	}
	return v;
}

double DecisionStrategy::getDecisionWeight(const State &s, Decisions d) const{

	double w = 0.0;

	//for (FeatureWeight f : _decisionWeights[d]){
	for (unsigned int i = 0; i < _decisionWeights->at(d)->Length(); i++){
		FeatureWeight f = _decisionWeights->at(d)->at(i);

		const DecisionState &d = s.decision;
		//Card &a = *d.activeCard;
		int player = s.decision.controllingPlayer;
		const PlayerState &p = s.players[player];

		double featureValue = 0.0;

		////////////////////////////////
		// calculate deck counts
		int totalMoneyLeft = 0;
		int totalActionsLeft = 0;
		int totalVictoryCardsAndCursesLeft = 0;
		for (Card *c : p.deck){
			if (c->isTreasure){
				totalMoneyLeft += c->money;
			}
			if (c->isAction){
				totalActionsLeft++;
			}
			if (c->IsPureVictory() || c->name == "curse"){
				totalVictoryCardsAndCursesLeft++;
			}
		}
		// calculate hand counts
		int numCursesInHand = 0, numCoppersInHand = 0;
		for (Card *c : p.hand) {
			if (c->name == "curse") {
				numCursesInHand++;
			}
			else if (c->name == "copper") {
				numCoppersInHand++;
			}
		}
		// calculate game state counts
		UINT numEmptyPiles = s.EmptySupplyPiles();
		// calculate opponents' deck densities
		int numMoatsOppDeck = 0;
		int numCardsOppDeck = 0;
		int numCardsOppHand = 0;
		int numTreasuresOppDeck = 0;
		// get other player and get counts
		//TODO change this if increase number players
		for (int pIndex = 0; pIndex < s.playerMaximum; pIndex++) {
			if (pIndex != player) {
				const PlayerState &curp = s.players[pIndex];
				for (Card* c : curp.deck) {
					if (c->name == "moat") {
						numMoatsOppDeck++;
					}
					if (c->isTreasure) {
						numTreasuresOppDeck++;
					}
				}
				numCardsOppDeck = curp.deck.Length();
				numCardsOppHand = curp.hand.Length();
			}
		}

		////////////////////////////////
		switch (f.type){
			// deck features
			case MONEY_DENSITY_OF_DECK:
				// Value of the money density of the remaining deck
				featureValue = (double)totalMoneyLeft / (double)p.deck.Length();
				break;
			case MONEY_DENSITY_OF_DECK_GREATER_THAN_1:
				// get value of OPPONENT_HAS_ATTACK_CARDS from state s and multiphy in 
				featureValue = ((double)totalMoneyLeft / (double)p.deck.Length()>1) ? 1.0 : 0;
				break;
			case MONEY_DENSITY_OF_DECK_GREATER_THAN_1_25:
				// Boolean value if money density is greater than 1.25
				featureValue = ((double)totalMoneyLeft / (double)p.deck.Length()>1.25) ? 1.0 : 0;
				break;
			case MONEY_DENSITY_OF_DECK_GREATER_THAN_1_5:
				// Boolean value if money density is greater than 1.5
				featureValue = ((double)totalMoneyLeft / (double)p.deck.Length()>1.5) ? 1.0 : 0;
				break;
			case ACTION_DENSITY_OF_DECK:
				featureValue = (double)totalActionsLeft / (double)p.deck.Length();
				break;
			case VICTORY_DENSITY_OF_DECK:
				featureValue = (double)totalVictoryCardsAndCursesLeft / (double)p.deck.Length();
				break;
			// turn features
			case NUM_AVAILABLE_ACTIONS:
				featureValue = p.actions;
				break;
			// hand features
			case NUM_CARDS_IN_HAND:
				featureValue = p.hand.Length();
				break;
			case BUYING_POWER:
				featureValue = p.MoneyTotal();
				break;
			case NUM_ACTION_CARDS_IN_HAND:
				featureValue = p.ActionCount();
				break;
			case NUM_VICTORY_CARDS_IN_HAND:
				featureValue = p.VictoryCount();
				break;
			case NUM_TREASURE_CARDS_IN_HAND:
				featureValue = p.TreasureCount();
				break;
			case NUM_CURSE_CARDS_IN_HAND:
				featureValue = numCursesInHand;
				break;
			case NUM_COPPER_CARDS_IN_HAND:
				featureValue = numCoppersInHand;
				break;
			case HAS_ONE_COPPER_IN_HAND:
				featureValue = numCoppersInHand > 0 ? 1 : 0;
				break;
			// game ending conditions
			case ONE_KINGDOM_PILE_EMPTY:
				if (numEmptyPiles >= 1) featureValue = 1;
				break;
			case TWO_KINGDOM_PILES_EMPTY:
				if (numEmptyPiles >= 2) featureValue = 1;
				break;
			case NUM_PROVINCES_LEFT:
				featureValue = (double) s.SupplyCount(s.data->baseCards.province);
				break;
			// opp deck features
			case OPP_MOAT_DENSITY_OF_OPP_DECK_IS_ZERO:
				featureValue = ((double)numMoatsOppDeck / (double)numCardsOppDeck) == 0 ? 1 : 0;
				break;
			case OPP_MOAT_DENSITY_OF_OPP_DECK:
				featureValue = (double)numMoatsOppDeck / (double)numCardsOppDeck;
				break;
			case OPP_TREASURE_DENSITY_OF_OPP_DECK:
				featureValue = (double)numTreasuresOppDeck / (double)numCardsOppDeck;
				break;
			case OPP_HAND_SIZE_LARGER_THAN_THREE:
				featureValue = numCardsOppHand > 3 ? 1 : 0;
				break;
				//TODO: FILL IN MORE FEATURES HERE
		}

		w +=  f.weight * featureValue;
	}


	return w;
}

DecisionStrategy* DecisionStrategy::Mutate(const CardDatabase &cards, const GameOptions &options) const{
	DecisionStrategy* buysChanged = MutateOnlyBuys(cards, options);
	return buysChanged->MutateOnlyDecisions(cards,options);
}

DecisionStrategy* DecisionStrategy::MutateOnlyBuys(const CardDatabase &cards, const GameOptions &options) const{
	Console::WriteLine("MUTATING BUYS");
	
	PersistentAssert(_m.entries.Length() > 0, "Empty menu");
	BuyMenu m = _m;

	if (rnd() <= 0.1) m.estateBuyThreshold = Utility::Bound(m.estateBuyThreshold + AIUtility::Delta(), 0, 8);
	if (rnd() <= 0.1) m.duchyBuyThreshold = Utility::Bound(m.duchyBuyThreshold + AIUtility::Delta(), 0, 8);
	if (options.prosperity && rnd() <= 0.1) m.provinceBuyThreshold = Utility::Bound(m.provinceBuyThreshold + AIUtility::Delta(), 0, 8);

	if (rnd() <= 0.7)
	{
		//
		// Mutate existing entry
		//
		BuyMenuEntry &curEntry = m.entries.RandomElement();
		if (curEntry.count != 99)
		{
			double r = rnd();
			if (r <= 0.2)
			{
				curEntry.count = 0;
			}
			else if (r <= 0.7)
			{
				curEntry.count = Utility::Bound(curEntry.count + AIUtility::Delta(2), 0, 12);
			}
			else
			{
				curEntry.count = Utility::Bound(curEntry.count + AIUtility::Delta(12), 0, 12);
			}
		}
	}

	for (UINT mutationIndex = 0; mutationIndex < 2; mutationIndex++)
	{
		if (rnd() <= 0.6)
		{
			BuyMenuEntry &curEntry = m.entries.RandomElement();
			if (curEntry.count != 99)
			{
				Card *newCard = options.RandomSupplyCard(cards);
				for (int retryIndex = 0; retryIndex < 20; retryIndex++) if (!curEntry.CardValidInSlot(newCard)) newCard = options.RandomSupplyCard(cards);

				if (curEntry.CardValidInSlot(newCard) && newCard->name != "curse" && newCard->name != "copper" && newCard->name != "estate" && newCard->name != "duchy" && newCard->name != "province" && newCard->name != "colony")
				{
					curEntry.c = newCard;
					if (rnd() <= 0.5) curEntry.count = 1;
				}
			}
		}
	}

	if (rnd() <= 0.5)
	{
		BuyMenuEntry &entry1 = m.entries.RandomElement();
		BuyMenuEntry &entry2 = m.entries.RandomElement();
		if (entry1.minCost == entry2.minCost && entry1.maxCost == entry2.maxCost && entry1.count != 99 && entry2.count != 99) Utility::Swap(entry1, entry2);
	}

	return new DecisionStrategy(m, _decisionWeights);
}


DecisionStrategy* DecisionStrategy::MutateOnlyDecisions(const CardDatabase &cards, const GameOptions &options) const{
	PersistentAssert(_m.entries.Length() > 0, "Empty menu");
	BuyMenu m = _m;

	// mutate the decision strategy
	if (rnd() <= 0.9) {
		Vector<Vector<FeatureWeight>*>* dwts = new Vector<Vector<FeatureWeight>*>();
		for (Vector<FeatureWeight>* v : *_decisionWeights) {
			Vector<FeatureWeight>* vf = new Vector<FeatureWeight>();
			for (FeatureWeight f : *v) {
				normal_distribution<> gauss(f.weight, mutateVariance);
				FeatureWeight fw(f.type, gauss(gen));
				vf->PushEnd(fw);
			}
			dwts->PushEnd(vf);
		}
		return new DecisionStrategy(m, dwts);
	}

	return new DecisionStrategy(m, _decisionWeights);
}
