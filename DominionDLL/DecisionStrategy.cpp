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


double DecisionStrategy::getDecisionWeight(const State &s, DecisionResponse &response, Decisions d) const{

	double w = 0.0;

	//for (FeatureWeight f : _decisionWeights[d]){
	for (unsigned int i = 0; i < _decisionWeights->at(d)->Length(); i++){
		FeatureWeight f = _decisionWeights->at(d)->at(i);

		const DecisionState &d = s.decision;
		Card &a = *d.activeCard;
		int player = s.decision.controllingPlayer;
		const PlayerState &p = s.players[player];


		double featureValue = 0.0;



		////////////////////////////////
		int totalMoneyLeft = 0;
				for (Card *c : p.deck){
					if (c->isTreasure){
				totalMoneyLeft += c->money;
					}
				}

		int totalActionsLeft = 0;
		for (Card *c : p.deck){
			if (c->isAction){
				totalActionsLeft++;
			}
		}

		int totalVictoryCardsAndCursesLeft = 0;
		for (Card *c : p.deck){
			if (c->IsPureVictory() || c->name == "curse"){
				totalVictoryCardsAndCursesLeft++;
			}
		}
		////////////////////////////////


		switch (f.type){
			case MONEY_DENSITY_OF_DECK:
				// Value of the money density of the remaining deck
				featureValue = (double)totalMoneyLeft / (double)p.deck.Length();
				break;
			case MONEY_DENSITY_OF_DECK_GREATER_THAN_1:
				// get value of OPPONENT_HAS_ATTACK_CARDS from state s and multiphy in 
				//TODO: ACTUALLY EXTRACT INFO FROM S

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

			case BUYING_POWER:

				featureValue = p.MoneyTotal();
				break;
				//TODO: FILL IN MORE FEATURES HERE
		}

		w +=  f.weight * featureValue;
	}


	return w;
}

DecisionStrategy* DecisionStrategy::Mutate(const CardDatabase &cards, const GameOptions &options) const{
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

	return new DecisionStrategy(m);
}