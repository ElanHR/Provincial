#include "Main.h"

class MetaTestChamber
{
public:
	void StrategizeStart(const CardDatabase &cards, const GameOptions &options, const String &directory, int chamberCount, TrainingType trainingType, PlayerType playerType, String buyMenu);
	void StrategizeStep(const CardDatabase &cards, TrainingType trainingType, PlayerType playerType);

private:

    Vector<TestChamber> _chambers;
	PlayerType _playerType;
	TrainingType  _TrainingType;

};
