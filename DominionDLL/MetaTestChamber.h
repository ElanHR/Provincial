#include "Main.h"

class MetaTestChamber
{
public:
	void StrategizeStart(const CardDatabase &cards, const GameOptions &options, const String &directory, int chamberCount, PlayerType playerType);
	void StrategizeStep(const CardDatabase &cards, PlayerType playerType);

private:

    Vector<TestChamber> _chambers;
};
