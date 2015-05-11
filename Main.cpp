/* Main.cpp
 * Author:  Phil Smith
 * Date:    20150411
 * Version: inf
 *
 */

/**
Deployment methods
0 Basic Reflex drop with pre-set threshold
1 Reflex drop we evaluated threshold
2 Midpoint via halfway, adjust for wall
3 Midpoint via lowest force
4 Virtual force relay self positioning using V.
5  ""
*/

#define GRIDFILE "RelayTestGrid"
#define RELAYFILE "RelayTestRelay.txt"
#define LOGFILE "LogFile.txt"

#include "Simulation.h"

#include "Agent/ImplementAgent.h"
#include "FileRead.h"
#include <fstream>
#include "GUI/GridGUI.h"
#include <time.h>

using namespace Agentspace;
using namespace environment;
using namespace std;

const string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

int main(int argc, char* argv[]) {
	//make agent
#ifdef Simulation
	cout<<"Simulation"<<endl;
	//SimulationAgent* Robotino = new SimulationAgent();
	SimulationAgent Robotino;

	//read file about trueWorld and relay
	RelativeCoordinate relativeToGoal = readfileGrid(Robotino, (string)GRIDFILE+(string)argv[1]+".txt");

	Robotino.setGoal(relativeToGoal);/**using magic we find where the goal is */

	readfileRelay(Robotino, (string)RELAYFILE);

	GridGUI TrueGUI = GridGUI(&Robotino.trueWorld,5, argv[2]);
	GridGUI KnownGUI = GridGUI(&Robotino.getKnownGrid(),900,argv[2]);
	Robotino.lookAround();

#else
	//Agent* Robotino= new ImpementAgent();
	ImplementationAgent Robotino;
	//read file of just relay for agent
	readfileRelay(Robotino, RELAYFILE);

#endif
	Robotino.defineDeploymentMethod(atoi(argv[2]));



 clock_t start = clock();
	try{

	     if(Robotino.getDeploymentMethod()==1)
    {
        Robotino.evaluateRealayRange();
    }
	    //Agent run
		while(!Robotino.done()) //loop until robot job is done.
		{
			TrueGUI.paint();
			KnownGUI.updateSize();
			KnownGUI.paint(true);

			//char x;cin>>x;
			try{Robotino.findPath();} catch(int easyThrow){} //nothing serius, just keep trying


		}
	}
	catch(string s){cout<<s<<endl;} //yall done goofed, lets stop
     double timer = (clock()-start) / (double) CLOCKS_PER_SEC;
	TrueGUI.paint(true);
	KnownGUI.updateSize();
	KnownGUI.paint(true);
#ifdef Simulation
	cout<<"relay range used: "<<SimulationRelay::getRange()<<endl<<"Time Taken: "<<timer<<endl;;
#endif
char x;cin>>x;
    //print results to file

    ofstream Log(LOGFILE, fstream::app );
    if(Log.is_open())
    {
        #ifdef Simulation
        Log<<"Simulation;";
        Log<<SimulationRelay::getRange()<<";";
        #else
        Log<<"Implementation;";
        //put like ping quality or somehting here
        #endif
        Log<<currentDateTime();
        Log<<argv[2]<<";";//method
        Log<<timer<<";";
        for(int i=0; i<Robotino.getKnownGrid().getRelays().size(); i++)
        {
        Log<< Robotino.getKnownGrid().getRelays()[i]->getPos()<<" ";
        }
        Log<<";";

        ifstream problemFile(((string)GRIDFILE+(string)argv[1]+".txt").c_str());
        while(problemFile.good())
        {
            char temp;
            problemFile>>temp;
            if(temp!= '\n')
                Log<<temp;
        }
        problemFile.close();
        Log<<";";

        problemFile.open(RELAYFILE);
        while(problemFile.good())
        {
            char temp;
            problemFile>>temp;
            if(temp!= '\n')
                Log<<temp;
        }
        problemFile.close();
        Log<<";";


        Log<<endl;
        Log.close();
    }
}
