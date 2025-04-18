#include <iostream>
#include <stdio.h>

#include "Galaxy/Galaxy.h"
#include "Galaxy/NuclearDisk.h"
#include "Parameters/GlobalParameters.h"
#include "Parameters/InitialisedData.h"

GlobalParameters Params;

std::chrono::time_point<std::chrono::system_clock> startTime;

void Welcome()
{
	if (Params.Meta.Verbosity > 0)
	{
		forLineIn(Params.Resources.WelcomeFile.Value,
				  std::cout << FILE_LINE << "\n";);
		std::cout << "\nNew Simulation initiated on " << JSL::CurrentTime();
		std::cout << "\nFile output is being sent to " << Params.Output.Root.Value << "\n";
		std::cout << "\nBeginning initialisation of data structures...." << std::endl;
	}
}

void Exit()
{
	if (Params.Meta.Verbosity > 0)
	{
		auto endTime = std::chrono::system_clock::now();
		std::cout << "\n\nSimulation complete. Computation duration was " << JSL::FormatClock(startTime, endTime);
		std::cout << "\nHave a nice day!\n";
	}
}
int main(int argc, char **argv)
{
	for (int i = 0; i < argc; ++i)
	{
		std::cout << argv[i] << std::endl;
	}

	startTime = std::chrono::system_clock::now();
	Params.Initialise(argc, argv);

	std::cout << Params.Yield.SNIa_DelayTime << std::endl;

	Welcome();

	// initialise the precomputed portions of the data which will be available (nearly) globally

	InitialisedData Data(Params);


	//~ initialise main galaxy object
	if (Params.NuclearDisk.DoNuclearDisk)
	{
		NuclearDisk g = NuclearDisk(Data);

		int n = Params.Galaxy.RingCount;
		double fraction = 1e-3;
		int pol = 10;

		Data.UrgentLog("Beginning main computation loop...\n");
		g.Evolve();

		auto endTime = std::chrono::system_clock::now();
		Data.UrgentLog("\tMain Computation loop complete in " + JSL::FormatClock(startTime, endTime) + ".\n");

		if (Params.Catalogue.SynthesisActive)
		{
			Data.UrgentLog("Beginning Synthesis of mock catalogue\n");
			g.SynthesiseObservations();
		}

		Exit();
		return 0;
	}
	else
	{
		Galaxy g = Galaxy(Data);

		int n = Params.Galaxy.RingCount;
		double fraction = 1e-3;
		int pol = 10;

		Data.UrgentLog("Beginning main computation loop...\n");
		g.Evolve();

		auto endTime = std::chrono::system_clock::now();
		Data.UrgentLog("\tMain Computation loop complete in " + JSL::FormatClock(startTime, endTime) + ".\n");

		if (Params.Catalogue.SynthesisActive)
		{
			Data.UrgentLog("Beginning Synthesis of mock catalogue\n");
			g.SynthesiseObservations();
		}

		Exit();
		return 0;
	}
}
