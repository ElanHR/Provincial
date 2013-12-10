#include "Main.h"

const int debugResourceBonus = 0;
const int debugRandomCardCount = 0;
const char *debugCardName = "gambler";
const int debugCardCount = 0;

/**
This function returns true if the decision is trivial, ie, 
if it involves selecting one card from a pool of one card.
*/
bool DecisionState::IsTrivial() const
{
    if(type == DecisionSelectCards && cardChoices.Length() == 1 && minimumCards == 1 && maximumCards == 1)
    {
        return true;
    }
    return false;
}

/**
This function contructs and returns a trivial response,
a response which is choosing one card from a set of one card.
*/
DecisionResponse DecisionState::TrivialResponse() const
{
    DecisionResponse response;
    if(cardChoices.Length() == 1 && minimumCards == 1 && maximumCards == 1)
    {
        response.cards.Allocate(1);
        response.cards[0] = cardChoices[0];
    }
    return response;
}

/**
This function attempts to gain a specific card from the supply.
If it is found acceptable to gain this card, the card is 
added to the decision state's list of candidate cards from which
the player will choose.
@param s state
@param c card
@param minCost minimum cost 
@param maxCost maximum cost
@param filter card filter
*/
void DecisionState::GainCardFromSupply(State &s, Card *c, int minCost, int maxCost, CardFilter filter)
{
	// sets the decision type to gaining only one specific card.
    SelectCards(c, 1, 1);
	// prints debug info
    if(decisionText) text = "Select a card to gain:";
	// loops over all the supply cards
    for(UINT supplyIndex = 0; supplyIndex < s.data->supplyCards.Length(); supplyIndex++)
    {
		// for each card, gets cost, remaining count, and a candidate card
        int cost = s.SupplyCost(supplyIndex);
        int count = s.supply[supplyIndex].count;
        Card *candidate = s.data->supplyCards[supplyIndex];

		// a card is acceptable if there is one to be gained,
		// and its cost is acceptable
        bool cardAcceptable = (count > 0 && cost >= minCost && cost <= maxCost);
		// if we only are filtering on treasure, check if it's a 
		// treasure
        if(filter == FilterTreasure) cardAcceptable &= candidate->isTreasure;

		// add the card to the decision state's list
		// of choices, if it's acceptable.
        if(cardAcceptable)
        {
            AddUniqueCard(candidate);
        }
    }
}
/**
This function attempts to gain a specific treasure card from the supply.
If it is found acceptable to gain this card, the card is
added to the decision state's list of candidate cards from which
the player will choose.
@param s
@param c
@param minCost
@param maxCost
*/
void DecisionState::GainTreasureFromSupply(State &s, Card *c, int minCost, int maxCost)
{
	// sets the decision type to gaining only one specific card.
    SelectCards(c, 1, 1);
    if(decisionText) text = "Select a treasure to gain:";
	// loop over all cards, find acceptable, add to list 
	// of card choices
    for(UINT supplyIndex = 0; supplyIndex < s.data->supplyCards.Length(); supplyIndex++)
    {
        int cost = s.SupplyCost(supplyIndex);
        int count = s.supply[supplyIndex].count;
        if(count > 0 && cost >= minCost && cost <= maxCost && s.data->supplyCards[supplyIndex]->isTreasure)
        {
            AddUniqueCard(s.data->supplyCards[supplyIndex]);
        }
    }
}
/**
This function attempts to gain a specific victory card from the supply.
If it is found acceptable to gain this card, the card is
added to the decision state's list of candidate cards from which
the player will choose.
@param s
@param c
@param minCost
@param maxCost
*/
void DecisionState::GainVictoryFromSupply(State &s, Card *c, int minCost, int maxCost)
{
	// sets the decision type to gaining only one specific card.
    SelectCards(c, 1, 1);
    if(decisionText) text = "Select a treasure to gain:";
    for(UINT supplyIndex = 0; supplyIndex < s.data->supplyCards.Length(); supplyIndex++)
    {
		// loop through all card types, add the card to list of
		// choices if it's acceptable (if it's a victory card with 
		// our given min and max cost)
        int cost = s.SupplyCost(supplyIndex);
        int count = s.supply[supplyIndex].count;
        if(count > 0 && cost >= minCost && cost <= maxCost && s.data->supplyCards[supplyIndex]->isVictory)
        {
            AddUniqueCard(s.data->supplyCards[supplyIndex]);
        }
    }
}

/**
Constructs a new game.
Frees memory, sets up the deck object.
Sets the turn variable to 0, the buy variable to 1, and action to 1,
unless debug options specified.
*/
void PlayerState::NewGame(const GameData &data)
{
    playArea.FreeMemory();
    discard.FreeMemory();
    hand.FreeMemory();
    deck.FreeMemory();
    islandZone.FreeMemory();
	
	// sets up players decks
    if(data.options.startingCondition == StartingCondition34Split)
    {
        for(UINT copperIndex = 0; copperIndex < 3; copperIndex++) deck.PushEnd(data.cards->GetCard("copper"));
        for(UINT estateIndex = 0; estateIndex < 2; estateIndex++) deck.PushEnd(data.cards->GetCard("estate"));

        for(UINT copperIndex = 0; copperIndex < 4; copperIndex++) deck.PushEnd(data.cards->GetCard("copper"));
        for(UINT estateIndex = 0; estateIndex < 1; estateIndex++) deck.PushEnd(data.cards->GetCard("estate"));
    }
    else if(data.options.startingCondition == StartingCondition25Split)
    {
        for(UINT copperIndex = 0; copperIndex < 2; copperIndex++) deck.PushEnd(data.cards->GetCard("copper"));
        for(UINT estateIndex = 0; estateIndex < 3; estateIndex++) deck.PushEnd(data.cards->GetCard("estate"));

        for(UINT copperIndex = 0; copperIndex < 5; copperIndex++) deck.PushEnd(data.cards->GetCard("copper"));
    }
    else
    {
        for(UINT copperIndex = 0; copperIndex < 7; copperIndex++) deck.PushEnd(data.cards->GetCard("copper"));
        for(UINT estateIndex = 0; estateIndex < 3; estateIndex++) deck.PushEnd(data.cards->GetCard("estate"));
        deck.Randomize();
    }

    for(UINT debugCardIndex = 0; debugCardIndex < debugCardCount; debugCardIndex++) deck.PushEnd(data.cards->GetCard(debugCardName));

    //deck.PushEnd(data.cards->GetCard("platinum"));
    //deck.PushEnd(data.cards->GetCard("silver"));
    //deck.PushEnd(data.cards->GetCard("silver"));
    //deck.PushEnd(data.cards->GetCard("copper"));
    //deck.PushEnd(data.cards->GetCard("copper"));

	// if debug random card count is set, add random cards 
	// to the deck
    for(UINT randomCards = 0; randomCards < debugRandomCardCount; randomCards++) deck.PushEnd(data.cards->RandomSupplyCard());
    //deck.PushEnd(data.cards->GetCard("mine"));

    actions = 1 + debugResourceBonus;
    buys = 1 + debugResourceBonus;
    money = 0 + debugResourceBonus;
    VPTokens = 0;
    turns = 0;
}
/**
This function returns the total number of cards 
available to a player's state.
It includes the number of cards in his deck, his hand, his discard pile,
the play area, and the island zone (?)
@return the total number of cards in a player's state.
*/
int PlayerState::TotalCards() const
{
    return deck.Length() + hand.Length() + discard.Length() + playArea.Length() + islandZone.Length();
}
/**
@return the number of action cards in the player's hand.
*/
int PlayerState::ActionCount() const
{
    int result = 0;
    for(Card *c : hand) if(c->isAction) result++;
    return result;
}

/**
@return the number of treasure cards in the player's hand.
*/
int PlayerState::TreasureCount() const
{
    int result = 0;
    for(Card *c : hand) if(c->isTreasure) result++;
    return result;
}

/**
@return the number of victory cards in the player's hand.
*/
int PlayerState::VictoryCount() const
{
    int result = 0;
    for(Card *c : hand) if(c->isVictory) result++;
    return result;
}

/**
@return the buying power in the player's hand.
*/
int PlayerState::MoneyTotal() const
{
    int result = money;
    for(Card *c : hand) if(c->isTreasure) result += c->treasure;
    return result;
}

/*map<Card*,int> PlayerState::CardCounts() const
{
    map<Card*,int> result;
    for(Card *c : hand            ) if(result.find(c) == result.end()) result[c] = 1; else result[c]++;
    for(Card *c : deck            ) if(result.find(c) == result.end()) result[c] = 1; else result[c]++;
    for(Card *c : discard         ) if(result.find(c) == result.end()) result[c] = 1; else result[c]++;
    for(Card *c : islandZone      ) if(result.find(c) == result.end()) result[c] = 1; else result[c]++;
    for(CardPlayInfo c : playArea) if(result.find(c.card) == result.end()) result[c.card] = 1; else result[c.card]++; 
    return result;
}*/

/**
Returns the cost of the card at the specified supply index
@param supplyIndex the index of the card in the supply
@return the cost of the card
*/
UINT State::SupplyCost(int supplyIndex) const
{
    return SupplyCost(data->supplyCards[supplyIndex]);
}
/**
Returns the cost of the card at the specified supply index
@param c the type of the card in the supply
@return the cost of the card
*/
UINT State::SupplyCost(Card *c) const
{
    int cost = c->cost;

    if(c == data->baseCards.peddler && phase == PhaseBuy)
    {
        int actionCardCount = 0;
        for(const CardPlayInfo &c : players[player].playArea) if(c.card->isAction) actionCardCount++;
        cost = Math::Max(0, cost - actionCardCount * 2);
    }

    if(data->bridgeInSupply)
    {
        const Vector<CardPlayInfo> &playArea = players[player].playArea;
        for(UINT playIndex = 0; playIndex < playArea.Length(); playIndex++)
        {
            if(playArea[playIndex].card == data->baseCards.bridge) cost -= playArea[playIndex].copies;
        }
        if(cost < 0) cost = 0;
    }

    if(data->quarryInSupply)
    {
        const Vector<CardPlayInfo> &playArea = players[player].playArea;
        for(UINT playIndex = 0; playIndex < playArea.Length(); playIndex++)
        {
            if(c->isAction && playArea[playIndex].card == data->baseCards.quarry) cost -= 2;
        }
        if(cost < 0) cost = 0;
    }

    if(data->plunderInSupply)
    {
        const Vector<CardPlayInfo> &playArea = players[player].playArea;
        for(UINT playIndex = 0; playIndex < playArea.Length(); playIndex++)
        {
            if(c->isTreasure && playArea[playIndex].card == data->baseCards.plunder) cost -= 2;
        }
        if(cost < 0) cost = 0;
    }

    return cost;
}

/**
Returns the count of the card in the supply
@param c the type of the card in the supply
@return the cost of the card
*/
UINT State::SupplyCount(Card *c) const
{
    int supplyIndex = data->SupplyIndex(c);
    if(supplyIndex == -1) return 0;
    return supply[supplyIndex].count;
}
/**
This function initializes the State object for a new game.
@param _data the game data
*/
void State::NewGame(const GameData &_data)
{
    data = &_data;
    const UINT playerCount = data->players.Length();
	// initializes each player
    for(UINT playerIndex = 0; playerIndex < playerCount; playerIndex++)
    {
        players[playerIndex].NewGame(*data);
        DrawCards(playerIndex, 5);
    }
	// random starting player
    player = rand() % playerCount;
	// sets the first turn phase
    players[player].turns = 1;
    phase = PhaseAction;

    stack.FreeMemory();
    gainList.FreeMemory();
    prevGainList.FreeMemory();

    if(logging)
    {
        data->log.Reset();
        data->log("New Game");
        data->log("Players: " + data->players[0].name + ", " + data->players[1].name);
        //data->log(data->players[player].name + " goes first");
        data->log("< " + data->players[player].name + "'s turn -- Turn 1>");
    }

	// declares the number of provinces, etc
    int victoryCardCount = 8;
    if(playerCount >= 3)
    {
        victoryCardCount = 12;
    }

    tradeRouteValue = 0;
	// initializes supply.
    for(UINT supplyIndex = 0; supplyIndex < maxSupply; supplyIndex++) supply[supplyIndex].count = 0;
    for(UINT supplyIndex = 0; supplyIndex < data->supplyCards.Length(); supplyIndex++)
    {
        Card *c = data->supplyCards[supplyIndex];
        if(c->isVictory)
        {
            supply[supplyIndex].count = victoryCardCount;
            supply[supplyIndex].tradeRouteToken = 1;
        }
        else
        {
            supply[supplyIndex].count = data->supplyCards[supplyIndex]->supply;
            supply[supplyIndex].tradeRouteToken = 0;
        }
    }
    supply[data->SupplyIndex(data->baseCards.copper)].count = 60 - 7 * playerCount;
    supply[data->SupplyIndex(data->baseCards.curse)].count = 10 * (playerCount - 1);
	// inits decision to blank
    decision.type = DecisionNone;
    decision.text.FreeMemory();
	// and advances to the first decision of the game
	// to be made by the 'player' that was randomly chosen earlier
    AdvanceToNextDecision(0);
}
/**
Shuffles the given player's deck.
@param playerIndex the index of the player in the game
*/
void State::Shuffle(UINT playerIndex)
{
    if(logging) Log(playerIndex, "shuffles");

    PlayerState &curPlayer = players[playerIndex];
    curPlayer.deck.Append(curPlayer.discard);
    curPlayer.discard.FreeMemory();
    curPlayer.deck.Randomize();
}

/**
Draws the given number of cards into the given player's hand.
@param playerIndex the index of the player in the game
@param cardCount the number of cards
*/
void State::DrawCards(UINT playerIndex, UINT cardCount)
{
    for(UINT cardIndex = 0; cardIndex < cardCount; cardIndex++) DrawCard(playerIndex);
}
/**
Draws one card from the given player's deck into his hand.
Shuffles the deck if necessary beforehand.
@param playerIndex the index of the player in the game
*/
Card* State::DrawCard(UINT playerIndex)
{
    PlayerState &curPlayer = players[playerIndex];
    if(curPlayer.deck.Length() == 0)
    {
        Shuffle(playerIndex);
    }
    if(curPlayer.deck.Length() > 0)
    {
        Card *c = curPlayer.deck.Last();
        if(logging) LogIndent(1, playerIndex, "draws " + c->PrettyName());
        curPlayer.hand.PushEnd(c);
        curPlayer.deck.PopEnd();
        return c;
    }
    else
    {
        if(logging) Log(playerIndex, "tries to draw, but has no cards left");
        return NULL;
    }
}
/**
Discard the specific card from the given player's hand.
@param playerIndex the index of the player in the game
@param c the card type to discard
*/
void State::DiscardCard(UINT playerIndex, Card *c)
{
    PlayerState &p = players[playerIndex];
    if(logging) Log(playerIndex, "discards " + c->PrettyName());
    int index = p.hand.FindFirstIndex(c);
    p.hand.RemoveSwap(index);
    p.discard.PushEnd(c);
}
/**
Plays from the given player's hand.
@param playerIndex the index of the player in the game
@param c the card type to discard
*/
void State::PlayCard(UINT playerIndex, Card *c)
{
    PlayerState &p = players[playerIndex];
    if(logging) Log(playerIndex, "plays " + c->PrettyName());
    int index = p.hand.FindFirstIndex(c);
    p.hand.RemoveSwap(index);

    int turnsLeft = 0;
    if(c->isDuration) turnsLeft = 1;
    if(c->isPermanent) turnsLeft = 999;

    p.playArea.PushEnd(CardPlayInfo(c, turnsLeft));
}
/**
Pushes an event on the stack to reorder the deck of a given player,
ie, for an attack card or because of an action.
@param source card that triggered the action
@param playerIndex the index of the player in the game
@param cardCount the number of cards to reorder, <= 2
*/
void State::ReorderDeck(Card *source, UINT playerIndex, UINT cardCount)
{
    for(UINT reorderIndex = 2; reorderIndex <= cardCount; reorderIndex++)
    {
        stack.PushEnd(new EventReorderDeck(source, playerIndex, reorderIndex));
    }
}
/**
Processes an action of the card given
@param c the card type
*/
void State::ProcessAction(Card *c)
{
    PlayerState &p = players[player];
	// adds actions, buys, money, and VP tokens to the player's totals
    p.actions += c->actions;
    p.buys += c->buys;
    p.money += c->money;
    p.VPTokens += c->victoryTokens;
    if(logging)
    {
        if(c->victoryTokens > 0) LogIndent(1, "gains " + String(c->victoryTokens) + " VP");
        if(c->victoryTokens < 0) LogIndent(1, "loses " + String(c->victoryTokens) + " VP");
        if(c->money > 0) LogIndent(1, "gets $" + String(c->money));
        if(c->money < 0) LogIndent(1, "pays $" + String(-c->money));
    }
	// draws the appropriate number of cards
    for(int cardIndex = 0; cardIndex < c->cards; cardIndex++)
    {
        DrawCard(player);
    }
    if(c->isAttack && data->championInSupply)
    {
        for(const PlayerInfo &p : data->players)
        {
            if(p.index != player)
            {
                for(CardPlayInfo &c : players[p.index].playArea)
                {
                    if(c.card == data->baseCards.champion)
                    {
                        for(int copyIndex = 0; copyIndex < c.copies; copyIndex++)
                        {
                            if(logging) LogIndent(1, p.index, "gains 1 VP from " + data->baseCards.champion->PrettyName());
                            players[p.index].VPTokens++;
                        }
                    }
                }
            }
        }
    }
	// and actually play the action if it has another effect.
    if(c->effect != NULL)
    {
        c->effect->PlayAction(*this);
    }
}
/**
Process the given treasure card.
@param c the card type
*/
void State::ProcessTreasure(Card *c)
{
	// get the current player and treasure value from the card
    PlayerState &p = players[player];
    int treasureValue = c->treasure;

    if(data->coppersmithInSupply)
    {
		// if coppersmith is in the supply
		// check if coppersmith is in the player's play area
		// and add more value if so
        const Vector<CardPlayInfo> &playArea = players[player].playArea;
        for(UINT playIndex = 0; playIndex < playArea.Length(); playIndex++)
        {
            if(playArea[playIndex].card == data->baseCards.coppersmith) treasureValue += playArea[playIndex].copies;
        }
    }
	// process the bank card. add more value for each card in play
    if(c == data->baseCards.bank)
    {
        for(const CardPlayInfo &c : players[player].playArea) if(c.card->isTreasure) treasureValue++;
    }

	// add treasure value and buys
    if(logging) LogIndent(1, "gets $" + String(treasureValue));
    p.money += treasureValue;
    p.buys += c->buys;
	// and take any effect
    if(c->effect != NULL)
    {
        c->effect->PlayAction(*this);
    }
}
/**
The game state sets the decision, then asks the player,
then the player gives his response.

Processes the current decision set in the decision variable
using the given response from the player.

@param response the response from the player in question
*/
void State::ProcessDecision(const DecisionResponse &response)
{
	// return if the game's over, or assert that the decision is valid
    if(decision.type == DecisionGameOver) return;
    Assert(decision.type != DecisionNone, "No decision active");

    Card *singleCard = response.singleCard;
	// if the decision was to select one card
    if(decision.type == DecisionSelectCards && decision.maximumCards <= 1)
    {
		// if the response was correctly chosen, ie, the player
		// chose one card, then set it so that it can be played below
        if(response.cards.Length() == 1) singleCard = response.cards[0];
        Assert(response.cards.Length() <= 1, "Invalid number of cards in response");
        Assert(decision.minimumCards == 0 || singleCard != NULL, "No response chosen");
    }
    else
    { // else assert we're not selecting cards
		// or that we have enough cards to decide.
        Assert(decision.type != DecisionSelectCards || (response.cards.Length() >= decision.minimumCards && response.cards.Length() <= decision.maximumCards), "Invalid number of cards in response");
        Assert(decision.type != DecisionDiscreteChoice || (response.choice < decision.minimumCards), "Invalid choice");
    }

    //
    // Reset the current decision. Note that some cards may still spawn another decision
    // before this function returns.
    //
    decision.type = DecisionNone;

    PlayerState &p = players[player];
	// if there is no active card in the decision...
    if(decision.activeCard == NULL)
    {
		// play the single card chosen depending on game phase.
        if(phase == PhaseAction)
        {
            if(singleCard == NULL) // move to treasure phase
            {
                if(logging) Log("chooses not to play an action");
                phase = PhaseTreasure;
            } 
            else// play the one card chosen, ie, take an action.
            {
                PlayCard(player, singleCard);
                p.actions--;
                ProcessAction(singleCard);
            }
        }
        else if(phase == PhaseTreasure)
        {
            if(singleCard == NULL) // no card chosen, move to buy
            {
                if(logging) Log("chooses not to play a teasure");
                phase = PhaseBuy;
                BuyPhaseStart();
            }
            else// play a treasure card, add it to the player's total
            {
                PlayCard(player, singleCard);
                ProcessTreasure(singleCard);
            }
        }
        else if(phase == PhaseBuy)
        {
            if(singleCard == NULL) // move to clean up
            {
                if(logging) Log("chooses not to buy a card");
                phase = PhaseCleanup;
            }
            else // play the buy of the card chosen
            {
                data->players[player].ledger.RecordBuy(singleCard);
                // specific card effects...
                if(data->hoardInSupply && singleCard->isVictory)
                {
                    const Vector<CardPlayInfo> &playArea = players[player].playArea;
                    for(UINT playIndex = 0; playIndex < playArea.Length(); playIndex++)
                    {
                        if(playArea[playIndex].card == data->baseCards.hoard) stack.PushEnd(new EventGainCard(player, data->baseCards.gold, false, false, GainToDiscard));
                    }
                }

                if(data->gardenerInSupply && singleCard->isVictory)
                {
                    const Vector<CardPlayInfo> &playArea = players[player].playArea;
                    for(UINT playIndex = 0; playIndex < playArea.Length(); playIndex++)
                    {
                        if(playArea[playIndex].card == data->baseCards.gardener)
                        {
                            players[player].VPTokens++;
                            if(logging) LogIndent(1, "gains 1 VP from " + data->baseCards.gardener->PrettyName());
                        }
                    }
                }

                if(data->goonsInSupply)
                {
                    const Vector<CardPlayInfo> &playArea = players[player].playArea;
                    for(UINT playIndex = 0; playIndex < playArea.Length(); playIndex++)
                    {
                        if(playArea[playIndex].card == data->baseCards.goons)
                        {
                            players[player].VPTokens++;
                            if(logging) LogIndent(1, "gains 1 VP from " + data->baseCards.goons->PrettyName());
                        }
                    }
                }

                if(data->mintInSupply && singleCard == data->baseCards.mint)
                {
                    Vector<CardPlayInfo> &playArea = players[player].playArea;
                    Vector<Card*> treasuresToTrash;
                    for(UINT playIndex = 0; playIndex < playArea.Length(); playIndex++)
                    {
                        if(playArea[playIndex].card->isTreasure) treasuresToTrash.PushEnd(playArea[playIndex].card);
                    }
                    for(Card *c : treasuresToTrash) stack.PushEnd(new EventTrashCardFromPlay(player, c));
                }
				// create the gain card event, push on stack
                stack.PushEnd(new EventGainCard(player, singleCard, true, false, GainToDiscard));
            }
        }
    }
    else // if there is an active card in the decision
    {
		// ... the card must have an effect that can process
		// a decision, or the last event on the stack must
		// be able to process a decision.

        //
        // This order of processing card effects before the stack must be kept because of throne room and king's court
        //
		// if the card has an effect and the effect can process decisions
        if(decision.activeCard->effect != NULL &&
           decision.activeCard->effect->CanProcessDecisions())
        {	// process the decision.
            decision.activeCard->effect->ProcessDecision(*this, response);
        }
        else if(stack.Length() > 0 && stack.Last()->CanProcessDecisions())
        {	// if the last item on the stack can process decisions
			// process
            stack.Last()->ProcessDecision(*this, response);
        }
        else
        {
            SignalError("Decision cannot be processed");
        }
    }
	// set the player to make the decision
    if(decision.controllingPlayer == -1) decision.controllingPlayer = player;
	// and set the decision's max cards
    decision.maximumCards = Math::Min(decision.maximumCards, decision.cardChoices.Length());
}
/**
Advances to the next game phase.
Depending on the current phase, call on the current player
to do the appropriate thing.
*/
void State::AdvancePhase()
{
    PlayerState &p = players[player];
    if(phase == PhaseAction)
    {	// advance if no actions, or...
        if(p.ActionCount() == 0 || p.actions == 0) phase = PhaseTreasure;
        else
        {	// choose an action to play.
            if(decisionText) decision.text = "Choose an action to play:";
			// set current decision to being selecting 0 or 1 cards
            decision.SelectCards(NULL, 0, 1);
            for(Card *c : p.hand)
            {	// and add cards to be considered for that decision
                if(c->isAction) decision.AddUniqueCard(c);
            }
        }
    }
    if(phase == PhaseTreasure)
    {
        if(p.TreasureCount() == 0) // advance to buy if no money
        {
            phase = PhaseBuy;
            BuyPhaseStart();
        }
        else
        {	// or ask the player to select 0 or 1 treasure cards
            if(decisionText) decision.text = "Choose a treasure to play:";
            decision.SelectCards(NULL, 0, 1);
            
            //
            // Bank should always be played last so that it's easy to play treasures
            //

            bool bankPresent = false;
            for(Card *c : p.hand)
            {
                if(c->isTreasure)
                {
                    if(c == data->baseCards.bank) bankPresent = true;
                    else decision.AddUniqueCard(c);
                }
            }

            if(bankPresent) decision.AddUniqueCard(data->baseCards.bank);
        }
    }
    if(phase == PhaseBuy && stack.Length() == 0)
    {	// if no buys, move to cleanup
        if(p.buys == 0) phase = PhaseCleanup;
        else
        {	// prompt for cards to buy
            if(decisionText) decision.text = "Choose a card to buy:";
            decision.SelectCards(NULL, 0, 1);
            for(UINT supplyIndex = 0; supplyIndex < data->supplyCards.Length(); supplyIndex++)
            {	// if the player can afford it, and 
				// if there exists cards left to buy
                int cost = SupplyCost(supplyIndex);
                int count = supply[supplyIndex].count;
                if(cost <= p.money && count > 0)
                {
                    Card *c = data->supplyCards[supplyIndex];
                    bool canBuyCard = true;
                    if(c == data->baseCards.grandMarket && p.CardInPlay(data->baseCards.copper))
                    {
                        canBuyCard = false;
                    }
                    if(c == data->baseCards.grandCourt)
                    {
                        for(CardPlayInfo &c : p.playArea)
                        {
                            if(c.card->isAction) canBuyCard = false;
                        }
                    }
					// add to list of things to consider in decision.
                    if(canBuyCard) decision.cardChoices.PushEnd(c);
                }
            }
			// if after checking, the player can't
			// buy anything, move to cleanup.
            if(decision.cardChoices.Length() == 0)
            {
                decision.type = DecisionNone;
                phase = PhaseCleanup;
                if(logging) Log("cannot afford to buy any cards");
            }
        }
    }
    if(phase == PhaseCleanup)
    {	// clean up phase
		// discard hand
        while(p.hand.Length() > 0)
        {
            DiscardCard(player, p.hand.Last());
        }
        // construct the new play area
        Vector<CardPlayInfo> newPlayArea;
        for(CardPlayInfo &c : p.playArea)
        {
            if(c.turnsLeft >= 1)
            {	// if a card is still in play add it to the new one
				// and decrement its duration
                CardPlayInfo newInfo = c;
                newInfo.turnsLeft = c.turnsLeft - 1;
                newPlayArea.PushEnd(newInfo);
            }
            else
            {	// process in a special way if 'treasury' card present
                bool discardCard = true;
                if(data->treasuryInSupply && c.card == data->baseCards.treasury)
                {
                    bool victoryCardGained = false;
                    for(Card *gainedCard : gainList)
                    {
                        if(gainedCard->isVictory) victoryCardGained = true;
                    }
                    if(!victoryCardGained)
                    {
                        discardCard = false;
                        if(logging) Log("puts " + c.card->PrettyName() + " on top of their deck");
                        p.deck.PushEnd(c.card);
                    }
                }
				// and if we're discarding the card, actually discard it
                if(discardCard) p.discard.PushEnd(c.card);
            }
        }
		// if there are cards in play still (with multi-turn duration_
		// keep them around, else clear it.
        if(newPlayArea.Length() > 0) p.playArea = newPlayArea;
        else p.playArea.FreeMemory();

        if(logging) Log("draws a new hand");
		// draw a new hand.
        int cardsToDraw = 5;
		// process specially for 'promised land' card
        if(data->promisedLandInSupply)
        {
            for(CardPlayInfo &c : p.playArea)
            {
                if(c.card == data->baseCards.promisedLand)
                {
                    cardsToDraw -= c.copies;
                }
            }
            if(cardsToDraw < 0) cardsToDraw = 0;
        }
		// and now draw cards
        for(int cardIndex = 0; cardIndex < cardsToDraw; cardIndex++)
        {
            DrawCard(player);
        }
        if(logging) Log("ends their turn\n");
		// check if the game's over!
        CheckEndConditions();
        if(decision.type == DecisionGameOver) return;
		// advance to the next player's turn.
        player = (player + 1) % playerMaximum;
        phase = PhaseAction;
        PlayerState &newPlayer = players[player];
        newPlayer.actions = 1 + debugResourceBonus;
        newPlayer.buys = 1 + debugResourceBonus;
        newPlayer.money = 0 + debugResourceBonus;
        newPlayer.turns++;

        if(data->useGainList)
        {
            prevGainList.FreeMemory();
            prevGainList = move(gainList);
        }
		
        if(logging) data->log("< " + data->players[player].name + "'s turn - Turn " + String(newPlayer.turns) + " >");
		// take any effects of cards that have multi-turn duration 
        for(CardPlayInfo &c : players[player].playArea)
        {
            for(int copyIndex = 0; copyIndex < c.copies; copyIndex++)
            {
                if(c.card->effect) c.card->effect->ProcessDuration(*this);
            }
        }
    }
}
/**
Advance to the next decision in this turn.
We have just processed the previous decision.
The recursive depth is used to track the stack being stuck.
@param recursiveDepth  the depth of recursion.
*/
void State::AdvanceToNextDecision(UINT recursionDepth)
{
    if(recursionDepth >= 500)
    {
        SignalError("Excessive recusion depth -- top of stack is probably stuck");
    }
	// end game if it's over
    if(decision.type == DecisionGameOver) return;

    //
    // Sometimes, resolving a decision will automatically create a new decision.
    //
    if(decision.type != DecisionNone)
    {
        //
        // If the decision has only one possible response, respond to it and keep looking for
        // a non-trivial decision.


        // A non-trivial decision is a choice by the current player
		// i believe, in each phase - ie, which cards to play
        if(decision.IsTrivial())
        {
            ProcessDecision(decision.TrivialResponse());
            AdvanceToNextDecision(recursionDepth + 1);
        }
        return;
    }
	// if there are no events left to process, advance the phase
    if(stack.Length() == 0)
    {
        //
        // If the stack is empty, the basic rules of Dominion control what happens.
        //
        AdvancePhase();
    }
    else
    {
        //
        // If the stack is not empty, process the top element on the stack
        //
        UINT eventStackIndex = stack.Length() - 1;
        Event *curEvent = stack[eventStackIndex];

        //
        // If a reaction is played, we may need to skip processing this event until the reaction is resolved.
        //
        bool skipEventProcessing = false;
        bool lighthouseActive = false;
        if(curEvent->IsAttack()) // && data->reactionCards.Length() > 0) -> this optimization doesn't work when we're debugging because people can have cards not in the supply
        {
            Assert(curEvent->AttackedPlayer() != -1, "Invalid player");
            const PlayerState &p = players[curEvent->AttackedPlayer()];
            AttackAnnotations &annotations = *curEvent->Annotations();

            //
            // Moat
            //
            if(!annotations.moatProcessed && p.hand.Contains(data->baseCards.moat))
            {
                stack.PushEnd(new EventMoatReveal(data->baseCards.moat, curEvent->AttackedPlayer()));
                annotations.moatProcessed = true;
                skipEventProcessing = true;
            }

            //
            // Lighthouse
            //
            if(p.CardInPlay(data->baseCards.lighthouse))
            {
                if(logging) LogIndent(1, curEvent->AttackedPlayer(), " is protected from the attack by " + data->baseCards.lighthouse->PrettyName());
                Event *nextEvent = stack.Last();
                stack.PopEnd();
                delete nextEvent;
                skipEventProcessing = true;
            }
        }
		// if we don't skip this event processing because of a moat
		// or lighthouse, ie, if it isn't a reaction
        if(!skipEventProcessing)
        {
			// process event
            bool eventCompleted = curEvent->Advance(*this);
            if(eventCompleted)
            {
                bool destroyNextEvent = lighthouseActive || curEvent->DestroyNextEventOnStack();
                stack.RemoveSlow(eventStackIndex);
                delete curEvent;

                //
                // Cards such as Moat and Lighthouse will also remove the next event on the stack (typically, an attack event)
                //
                if(destroyNextEvent)
                {
                    Event *nextEvent = stack.Last();
                    stack.PopEnd();
                    delete nextEvent;
                }
            }
        }
    }

    //
    // If we failed to find a decision, keep advancing the game state until we do.
    // 
	// note that phase advancement will set this variable.
    if(decision.type == DecisionNone) AdvanceToNextDecision(recursionDepth + 1);

    //
    // By default, decisions are controlled by the current player.
    //
    if(decision.controllingPlayer == -1) decision.controllingPlayer = player;
    decision.maximumCards = Math::Min(decision.maximumCards, decision.cardChoices.Length());

    //
    // If the decision has only one possible response, respond to it and keep looking for
    // a non-trivial decision.
    //
    if(decision.IsTrivial())
    {
        ProcessDecision(decision.TrivialResponse());
        AdvanceToNextDecision(recursionDepth + 1);
    }

    Assert(decisionText || decision.text.Length() == 0, "Decision text is disabled!");

    //
    // Events and effects should never have no available options; they should never have
    // been put on the stack in the first place.
    //
    if(decision.type == DecisionSelectCards && decision.cardChoices.Length() == 0)
    {
        ofstream debugFile("debug.txt");
        debugFile << decision.activeCard->name << ' ' << decision.minimumCards << "," << decision.maximumCards << endl;
        debugFile.flush();
        debugFile.close();
        __asm int 3;
    }
}
/**
Log the given string as under the current player
@param s the string to log
*/
void State::Log(const String &s)
{
    Assert(logging, "Logging is disabled!");
    data->log(data->players[player].name + " " + s);
}

/**
Log the given string as under the given player
@param playerIndex the player to log
@param s the string to log
*/
void State::Log(UINT playerIndex, const String &s)
{
    Assert(logging, "Logging is disabled!");
    data->log(data->players[playerIndex].name + " " + s);
}

/**
Log the given string as under the current player
@param indentLevel level to which to indent
@param s the string to log
*/
void State::LogIndent(UINT indentLevel, const String &s)
{
    Assert(logging, "Logging is disabled!");
    String preamble;
    for(UINT i = 0; i < indentLevel; i++) preamble += "  ";
    data->log(preamble + data->players[player].name + " " + s);
}

/**
Log the given string as under the given player
@param indentLevel level to which to indent
@param playerIndex
@param s the string to log
*/
void State::LogIndent(UINT indentLevel, UINT playerIndex, const String &s)
{
    Assert(logging, "Logging is disabled!");
    String preamble;
    for(UINT i = 0; i < indentLevel; i++) preamble += "  ";
    data->log(preamble + data->players[playerIndex].name + " " + s);
}
/**
Gets the given player's score.
@param playerIndex
@return the player's score
*/
int State::PlayerScore(UINT playerIndex) const
{
    const PlayerState &p = players[playerIndex];
    int sum = p.VPTokens;
    for(Card *c : p.hand)
    {
        sum += c->victoryPoints;
        if(c->isVictory && c->effect != NULL) sum += c->effect->VictoryPoints(*this, playerIndex);
    }
    for(Card *c : p.deck)
    {
        sum += c->victoryPoints;
        if(c->isVictory && c->effect != NULL) sum += c->effect->VictoryPoints(*this, playerIndex);
    }
    for(Card *c : p.discard)
    {
        sum += c->victoryPoints;
        if(c->isVictory && c->effect != NULL) sum += c->effect->VictoryPoints(*this, playerIndex);
    }
    for(Card *c : p.islandZone)
    {
        sum += c->victoryPoints;
        if(c->isVictory && c->effect != NULL) sum += c->effect->VictoryPoints(*this, playerIndex);
    }
    for(const CardPlayInfo &c : p.playArea)
    {
        sum += c.card->victoryPoints;
        if(c.card->isVictory && c.card->effect != NULL) sum += c.card->effect->VictoryPoints(*this, playerIndex);
    }
    return sum;
}
/**
Gets the player(s) who won the game, 
ie, the player(s) who had the most points.
@return a vector of the winning players.
*/
Vector<int> State::WinningPlayers() const
{
    Vector<int> result;
    double bestScore = -100000.0;
    for(const PlayerInfo &player : data->players)
    {
        int score = PlayerScore(player.index);
        double trueScore = score + 1.0 - players[player.index].turns * 0.0001;
        if(trueScore > bestScore)
        {
            bestScore = trueScore;
            result.Allocate(1, player.index);
        }
        else if(trueScore == bestScore)
        {
            result.PushEnd(player.index);
        }
    }
    return result;
}
/**
Checks if the game is over.  If so, sets the decision type appropriately
*/
void State::CheckEndConditions()
{
    if(players[player].turns >= 100 || EmptySupplyPiles() >= 3 || SupplyCount(data->baseCards.province) == 0 ||
        (data->supplyCards.Contains(data->baseCards.colony) && SupplyCount(data->baseCards.colony) == 0))
    {
        decision.type = DecisionGameOver;
        if(logging)
        {
            data->log("\n\n\n");

            double bestScore = -100000.0;
            for(const PlayerInfo &player : data->players)
            {
                data->log(player.name + ": " + String(PlayerScore(player.index)) + " VP");
            }

            for(int playerIndex : WinningPlayers())
            {
                data->log(data->players[playerIndex].name + " is the winner!!");
            }

            data->log("\n\n");
        }
    }
}

/**
Count the number of empty supply piles
@return the number of empty piles
*/
UINT State::EmptySupplyPiles() const
{
    UINT result = 0;
    for(UINT supplyIndex = 0; supplyIndex < data->supplyCards.Length(); supplyIndex++)
    {
        if(supply[supplyIndex].count == 0) result++;
    }
    return result;
}
/**
@return the number of actions the current player has taken this turn
*/
UINT State::ActionsPlayedThisTurn() const
{
    int result = 0;
    const PlayerState &p = players[player];
    for(const CardPlayInfo &c : p.playArea)
    {
        if(c.card->isDuration)
        {
            if(c.turnsLeft == 1) result++;
        }
        if(c.card->isPermanent)
        {
            if(c.turnsLeft == 999) result++;
        }
        else result++;
    }
    return result;
}
/**
This function is run when the buy phase starts.
It accounts for the furnace being in play.
The current player must discard N cards, where there are N furnaces in play
*/
void State::BuyPhaseStart()
{
    if(data->furnaceInSupply)
    {
        for(CardPlayInfo &c : players[player].playArea)
        {
            if(c.card == data->baseCards.furnace)
            {
                for(int copyIndex = 0; copyIndex < c.copies; copyIndex++) stack.PushEnd(new EventDiscardNCards(c.card, player, 0, 1));
            }
        }
    }
}
