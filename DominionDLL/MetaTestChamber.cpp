#include "Main.h"

void MetaTestChamber::StrategizeStart(const CardDatabase &cards, const GameOptions &options, const String &directory, int chamberCount, TrainingType trainingType, PlayerType playerType, String buyMenu)
{
    Console::WriteLine("BLAAAH!!! Strategizing for kingdom piles:");
    Console::WriteLine(options.ToString());


	Console::WriteLine("Player Type = " + String(playerType));


    Console::WriteLine("Using " + String(chamberCount) + " testing chambers");

    _chambers.Allocate(chamberCount);
    for(UINT chamberIndex = 0; chamberIndex < _chambers.Length(); chamberIndex++)
    {
        String chamberSuffix = "A";
        chamberSuffix[0] += chamberIndex;
        if(_chambers.Length() == 1) chamberSuffix = "";
        else chamberSuffix = "_" + chamberSuffix;

		if(trainingType == TRAINING_BUYS)
			_chambers[chamberIndex].StrategizeStartBuys(cards, options, directory, chamberSuffix, playerType, buyMenu);
		else if (trainingType == TRAINING_DECISIONS)
			_chambers[chamberIndex].StrategizeStartDecisions(cards, options, directory, chamberSuffix, playerType, buyMenu);
    }
}

void MetaTestChamber::StrategizeStep(const CardDatabase &cards, TrainingType trainingType, PlayerType playerType)
{
    for(UINT chamberIndex = 0; chamberIndex < _chambers.Length(); chamberIndex++)
    {
		if (trainingType == TRAINING_BUYS)
			_chambers[chamberIndex].StrategizeStepBuys(cards, playerType);
		else if (trainingType == TRAINING_DECISIONS)
			_chambers[chamberIndex].StrategizeStepDecisions(cards, playerType);
    }

    TestChamber &chamber = _chambers[0];
    if((chamber._generation - 1) % 4 == 0 && _chambers.Length() > 1)
    {
        Console::WriteLine("Generating inter-chamber leaderboard comparison for generation " + String(chamber._generation - 1));
        Vector<TestPlayer*> metaLeaders;
		for (TestChamber &c : _chambers) metaLeaders.PushEnd(c._leaders[0]);
		
		if (trainingType == TRAINING_BUYS)
			chamber.ComputeLeaderboard(cards, metaLeaders, chamber._directory + "leaderboard/" + String::ZeroPad(chamber._generation - 1, 3) + ".txt", 20000);
		else if (trainingType == TRAINING_DECISIONS)
			chamber.ComputeLeaderboard(cards, metaLeaders, chamber._directory + "decision-leaderboard/" + String::ZeroPad(chamber._generation - 1, 3) + ".txt", 20000);

    }
}
