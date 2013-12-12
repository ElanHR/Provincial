#include "Main.h"


// copying the other one, should copy 
DecisionStrategy::DecisionStrategy(BuyMenu &m) : BuyAgendaMenu(m)
{
	Init(); // this should be copying the old 
}

DecisionStrategy::DecisionStrategy(const CardDatabase &cards, const GameOptions &options) : BuyAgendaMenu(cards, options)
{
	Init();
}

void DecisionStrategy::Init(){
	_decisionWeights = new Vector<Vector<FeatureWeight>*>();
	for (int d = 0; d < NUM_DECISIONS; d++){

		_decisionWeights->PushEnd(new Vector<FeatureWeight>());

	}
}


double DecisionStrategy::getDecisionWeight(const State &s, DecisionResponse &response, Decisions d){

	double w = 0.0;

	//for (FeatureWeight f : _decisionWeights[d]){
	for (unsigned int i = 0; i < _decisionWeights->at(d)->Length(); i++){
		FeatureWeight f = _decisionWeights->at(d)->at(i);

		const DecisionState &d = s.decision;
		Card &a = *d.activeCard;
		int player = s.decision.controllingPlayer;
		const PlayerState &p = s.players[player];

		double curW ;

		//  MONEY_DENSITY_OF_DECK,
		//  MONEY_DENSITY_OF_DECK_GREATER_THAN_1,
		//  MONEY_DENSITY_OF_DECK_GREATER_THAN_1_5,
		//  MONEY_DENSITY_OF_DECK_GREATER_THAN_2,
		//  ACTION_DENSITY_OF_DECK,
		//  VICTORY_DENSITY_OF_DECK,
		double featureValue = 0.0;
		switch (f.type){
			case MONEY_DENSITY_OF_DECK:
				int totalMoney = 0;
				//for (int i = 0; i < p.deck.Length; i++){
				//	Card *c = p.deck.at(i);
				//for (Card *c : d.cardChoices){
				for (Card *c : p.deck){
					if (c->isTreasure){
						totalMoney += c->money;
					}
				}
				featureValue = totalMoney / p.deck.Length();
				break;
			case MONEY_DENSITY_OF_DECK_GREATER_THAN_1:

				int totalMoney = 0;
				
				for (auto *c : p.deck){
					if (c->isTreasure){
						totalMoney += c->money;
					}
				}
				featureValue = totalMoney / p.deck.Length();
				
				break;

				//TODO: FILL IN MORE FEATURES HERE
		}

		w +=  f.weight * featureValue;
	}


	return w;
}

DecisionStrategy* DecisionStrategy::Mutate() const{
	return 0;
}