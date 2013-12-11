#include "Main.h"

DecisionStrategy::DecisionStrategy(){
	_decisionWeights = new Vector<Vector<FeatureWeight>*>();
	for (int d = 0; d < NUM_DECISIONS; d++){
		//Vector<FeatureWeight> curDecicion;
		//_decisionWeights.PushEnd(curDecicion);

		_decisionWeights->PushEnd(new Vector<FeatureWeight>());
		
	}
}

DecisionStrategy::DecisionStrategy(DecisionStrategy &m){

}

double DecisionStrategy::getDecisionWeight(const State &s, Decisions d){

	double w = 0.0;

	//for (FeatureWeight f : _decisionWeights[d]){
	for (unsigned int i = 0; i < _decisionWeights->at(d)->Length(); i++){
		FeatureWeight f = _decisionWeights->at(d)->at(i);

		double curW;

		//  MONEY_DENSITY_OF_DECK,
		//  MONEY_DENSITY_OF_DECK_GREATER_THAN_1,
		//  MONEY_DENSITY_OF_DECK_GREATER_THAN_1_5,
		//  MONEY_DENSITY_OF_DECK_GREATER_THAN_2,
		//  ACTION_DENSITY_OF_DECK,
		//  VICTORY_DENSITY_OF_DECK,

		switch (f.type){
			case OPPONENT_HAS_ATTACK_CARDS:
				// get value of OPPONENT_HAS_ATTACK_CARDS from state s and multiphy in 
				//TODO: ACTUALLY EXTRACT INFO FROM S
				s.data->players[0].
				curW = f.weight * 1.0;
				break;

				//TODO: FILL IN MORE FEATURES HERE


		}

		w += curW;
	}


	return w;
}

DecisionStrategy* DecisionStrategy::Mutate() const{
	return 0;
}