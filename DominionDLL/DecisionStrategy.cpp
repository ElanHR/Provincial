#include "Main.h"

DecisionStrategy::DecisionStrategy(){
	_decisionWeights = new Vector<Vector<StateFeature>>();
	for (int d = 0; d < NUM_DECISIONS; d++){

		//_decisionWeights
		for (int f = 0; f < NUM_FEATURES; f++){

		}
	}
}

DecisionStrategy::DecisionStrategy(DecisionStrategy &m){

}

DecisionStrategy* DecisionStrategy::Mutate() const{
	return 0;
}