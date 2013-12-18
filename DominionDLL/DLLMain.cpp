// dllmain.cpp : Defines the entry point for the DLL application.
#include "Main.h"

#ifdef _DLL
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
#endif

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
    LPSTR lpszCmdLine, int nCmdShow)
{
    Utility::MessageBox("App Start");
    return 0;
}

int main(int argc, char *argv[])
{
    App *app = new App;
    app->Init();

    srand(timeGetTime());

    bool useTournamentFile = true;
    String directory;
    int generationCount = 64;
    int chamberCount = 3;
	String buyMenu = "";

    BYTE bytes[16];
    for(UINT byteIndex = 0; byteIndex < 16; byteIndex++) bytes[byteIndex] = rand() % 255;

    String kingdomPileHash = String::ByteStreamToHexString(bytes, 16);

	String trainingType = "";

    if(useTournamentFile)
    {

		
        Vector<String> lines = Utility::GetFileLines("TournamentParameters.txt");
        
		

		if (lines[0].Contains("trainingType")){
			if (lines[0].Contains("Train_BuyOrder")){
				trainingType = "Train_BuyOrder";
			}else if (lines[0].Contains("Train_Decisions")){
				trainingType = "Train_Decisions";
			}
		}else{
			Console::WriteLine("ERROR! No training type given");
		}

		if(lines[1] != "random") app->ProcessCommand("newKingdomCards@" + lines[1]);

        if(lines[2] == "random"){
            Utility::MakeDirectory("kingdomsIntermediate");

            directory = "kingdomsIntermediate/auto_" + kingdomPileHash + "/";

            Utility::MakeDirectory(directory);
            //Utility::MakeDirectory(directory + "counters/");
            Utility::MakeDirectory(directory + "generations/");
            Utility::MakeDirectory(directory + "leaderboard/");
            Utility::MakeDirectory(directory + "progression/");
        }
        else{
            directory = lines[2];
        }

        generationCount = lines[3].RemovePrefix("generations=").ConvertToInteger();
        chamberCount = lines[4].RemovePrefix("chambers=").ConvertToInteger();
		buyMenu = lines[5].RemovePrefix("buyMenu=");
    }
    else
    {
        directory = "kingdomsIntermediate/auto_" + kingdomPileHash + "/";

        Utility::MakeDirectory(directory);
        //Utility::MakeDirectory(directory + "counters/");
        Utility::MakeDirectory(directory + "generations/");
        Utility::MakeDirectory(directory + "leaderboard/");
        Utility::MakeDirectory(directory + "progression/");
    }



	app->ProcessCommand("trainAIStart@" + directory + "@" + String(chamberCount) + "@" + trainingType + "@" + buyMenu);

    for (int generationIndex = 0; generationIndex < generationCount; generationIndex++)
    {
        app->ProcessCommand("trainAIStep@" + directory);
    }

    return 0;
}
