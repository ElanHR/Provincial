#include "Main.h"

DecisionStrategy::DecisionStrategy(){
	for (int d = 0; d < NUM_DECISIONS; d++){
		//Vector<FeatureWeight> curDecicion;
		//_decisionWeights.PushEnd(curDecicion);
		
	}
}

DecisionStrategy::DecisionStrategy(DecisionStrategy &m){

}

double DecisionStrategy::getDecisionWeight(const State &s, Decisions d){

	double w = 0.0;
	for (FeatureWeight f : _decisionWeights[d]){
		double curW;

		switch (f.type){
			case OPPONENT_HAS_ATTACK_CARDS:
				// get value of OPPONENT_HAS_ATTACK_CARDS from state s and multiphy in 
				//TODO: ACTUALLY EXTRACT INFO FROM S
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