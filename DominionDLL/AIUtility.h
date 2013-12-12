
class DominionGame;

class AIUtility
{
public:
    static void SelectCards(const State &s, DecisionResponse &response, const function<double(Card*)> &scoringFunction);
    static Card* BestCard(const Vector<Card*> &choices, const function<double(Card*)> &scoringFunction, double minScore);
    
	static Player* MakeTwoCardPlayer(const CardDatabase &cards, const String &cardA, const String &cardB, PlayerType type);
	static PlayerLearning* MakeTwoCardPlayer(const CardDatabase &cards, Card *a, Card *b, PlayerType type);
    static Player* MakeRandomPlayer(const CardDatabase &cards);
    
    static void AdvanceAIs(DominionGame &game);

    static int Delta()
    {
        if(rand() % 2 == 0) return -1;
        return 1;
    }
    static int Delta(int value)
    {
        int v = rand() % value + 1;
        if(rand() % 2 == 0) return -v;
        return v;
    }
};

struct CardCounter
{
    CardCounter(const PlayerState &p);
    CardCounter(const Vector<Card*> &cardList);

    int Count(Card *c)
    {
        int length = counts.Length();
        if(length == 0) return 0;
        auto *CPtr = counts.CArray();
        for(int i = 0; i < length; i++)
        {
            if(CPtr[i].first == c)
            {
                return CPtr[i].second;
            }
        }
        return 0;
    }

    Vector< pair<Card*, int> > counts;

private:
    void AddCard(Card *c)
    {
        int length = counts.Length();
        if(length > 0)
        {
            auto *CPtr = counts.CArray();
            for(int i = 0; i < length; i++)
            {
                if(CPtr[i].first == c)
                {
                    CPtr[i].second++;
                    return;
                }
            }
        }
        counts.PushEnd(pair<Card*, int>(c, 1));
    }
};