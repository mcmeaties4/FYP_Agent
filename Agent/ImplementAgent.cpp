#include "ImplementAgent.h"
#include "../Relay/ImplementRelay.h"
#include <math.h>

using namespace Agentspace;
using namespace Relayspace;

ImplementAgent::ImplementAgent(char* ControlPort):Agent(),iRobot_Control(ControlPort)
{
      currentAngle=90; //must set up robot so is facing up on board
}


void ImplementAgent::setRelayCount(int numberofRelays)
{
	//will do nothing if already has more then number told. but this should only be called on startups once.

	//has too few?
 	while(heldRelays.size()<numberofRelays)
	{
	    ImplementRelay* temp= new ImplementRelay();
		heldRelays.push_back(temp);
	}
}

bool ImplementAgent::move(Node::Direction toMove)
{
  Coordinate oldLocation=CurrentLocation;

    switch(toMove)
    {
        case(Node::Right):
            AdjustAngle(0);
            break;
        case(Node::Left):
            AdjustAngle(180);
            break;
        case(Node::Up):
            AdjustAngle(90);
            break;
        case(Node::Down):
            AdjustAngle(270);
            break;
        case(Node::UpRight):
            AdjustAngle(45);
            break;
        case(Node::UpLeft):
            AdjustAngle(135);
            break;
        case(Node::DownRight):
            AdjustAngle(315);
            break;
        case(Node::DownLeft):
            AdjustAngle(225);
            break;
    }
    lookAround();

    bool moveOK = Agent::move(toMove); //lets see if the grid thinks its ok

    if(!moveOK)
        return false;

    int driveReturn= iRobot_Control.driveForward();
    if(driveReturn==1)
    {
         cout<<"bump right"<<endl;
         knownWorld[CurrentLocation].addContent(ContentType::Wall);
        CurrentLocation=oldLocation;
    }
    else if(driveReturn==2)
    {
        cout<<"bump left"<<endl; /// should we treat these different bumpers differently?
        knownWorld[CurrentLocation].addContent(ContentType::Wall);
        CurrentLocation=oldLocation;
    }
    else if(driveReturn==3)
    {
        cout<<"bump front"<<endl;
        knownWorld[CurrentLocation].addContent(ContentType::Wall);
        CurrentLocation=oldLocation;
    }
     else if(driveReturn==4)
    {
        cout<<"IR found wall"<<endl;
        knownWorld[CurrentLocation].addContent(ContentType::Wall);
        CurrentLocation=oldLocation;
    }
    else
    {
        lookAround();
    }
    return moveOK;
}

void ImplementAgent::AdjustAngle(int newAngle)
{
    if(currentAngle==newAngle)
        return;

    int AngleDif= newAngle-currentAngle;

    if(AngleDif>180)
        AngleDif=AngleDif-360;

    if(AngleDif>0)
    {
        cout<<"turn anticlock "<<AngleDif<<endl;
        iRobot_Control.spinAntiClock(AngleDif);
    }
    else
    {
        cout<<"turn clock" <<abs(AngleDif)<<endl;
        iRobot_Control.spinClock(abs(AngleDif));
    }


    currentAngle=newAngle;
}

bool ImplementAgent::lowSignal(Coordinate CurrentLocationtemp){std::cout<<"no low signal"; return false;}
bool ImplementAgent::done(){std::cout<<"no done"<<endl; return false;}

void ImplementAgent::lookAround()
{
    bool IRs[5];
    iRobot_Control.lightWall(IRs);
    if(IRs[0])
    {
        ///wall on right
        switch(currentAngle)
        {
            case 0:
                knownWorld[Coordinate(CurrentLocation.getRow()+1, CurrentLocation.getColumn() )].addContent(ContentType::Wall);
            case 180:
                knownWorld[Coordinate(CurrentLocation.getRow()-1, CurrentLocation.getColumn())].addContent(ContentType::Wall);
            case 90:
                knownWorld[Coordinate(CurrentLocation.getRow(), CurrentLocation.getColumn()+1)].addContent(ContentType::Wall);
            case 270:
                knownWorld[Coordinate(CurrentLocation.getRow(), CurrentLocation.getColumn()-1)].addContent(ContentType::Wall);

            case 45:
                knownWorld[Coordinate(CurrentLocation.getRow()+1, CurrentLocation.getColumn() )].addContent(ContentType::Wall); ///?
            case 135:
                knownWorld[Coordinate(CurrentLocation.getRow()-1, CurrentLocation.getColumn())].addContent(ContentType::Wall);  ///?
            case 225:
                 knownWorld[Coordinate(CurrentLocation.getRow(), CurrentLocation.getColumn()+1)].addContent(ContentType::Wall);  ///?
            case 315:
                knownWorld[Coordinate(CurrentLocation.getRow(), CurrentLocation.getColumn()-1)].addContent(ContentType::Wall);  ///?
        }
    }

    if(IRS[1])
    {
        /// right front
        switch(currentAngle)
        {
            case 0:
                knownWorld[Coordinate(CurrentLocation.getRow(), CurrentLocation.getColumn()+1)].addContent(ContentType::Wall);
            case 180:
                knownWorld[Coordinate(CurrentLocation.getRow(), CurrentLocation.getColumn()-1)].addContent(ContentType::Wall);
            case 90:
                knownWorld[Coordinate(CurrentLocation.getRow()-1, CurrentLocation.getColumn())].addContent(ContentType::Wall);
            case 270:
                knownWorld[Coordinate(CurrentLocation.getRow()+1, CurrentLocation.getColumn())].addContent(ContentType::Wall);

            case 45:
                knownWorld[Coordinate(CurrentLocation.getRow(), CurrentLocation.getColumn()+1)].addContent(ContentType::Wall);
            case 135:
                knownWorld[Coordinate(CurrentLocation.getRow()-1, CurrentLocation.getColumn())].addContent(ContentType::Wall);
            case 225:
                knownWorld[Coordinate(CurrentLocation.getRow(), CurrentLocation.getColumn()-1)].addContent(ContentType::Wall);
            case 315:
                knownWorld[Coordinate(CurrentLocation.getRow()+1, CurrentLocation.getColumn())].addContent(ContentType::Wall);
        }
    }


    if(IRs[2])
    {
        ///Wall ahead
          switch(currentAngle)
        {
            case 0:
                knownWorld[Coordinate(CurrentLocation.getRow(), CurrentLocation.getColumn()+1)].addContent(ContentType::Wall);
            case 180:
                knownWorld[Coordinate(CurrentLocation.getRow(), CurrentLocation.getColumn()-1)].addContent(ContentType::Wall);
            case 90:
                knownWorld[Coordinate(CurrentLocation.getRow()-1, CurrentLocation.getColumn())].addContent(ContentType::Wall);
            case 270:
                knownWorld[Coordinate(CurrentLocation.getRow()+1, CurrentLocation.getColumn())].addContent(ContentType::Wall);

                ///maybe no diagnals for this one, as it could be picking up some corners of walls or somehting?
        }
    }

     if(IRS[3])
    {
        /// left front
        switch(currentAngle)
        {
            case 0:
                knownWorld[Coordinate(CurrentLocation.getRow(), CurrentLocation.getColumn()+1)].addContent(ContentType::Wall);
            case 180:
                knownWorld[Coordinate(CurrentLocation.getRow(), CurrentLocation.getColumn()-1)].addContent(ContentType::Wall);
            case 90:
                knownWorld[Coordinate(CurrentLocation.getRow()-1, CurrentLocation.getColumn())].addContent(ContentType::Wall);
            case 270:
                knownWorld[Coordinate(CurrentLocation.getRow()+1, CurrentLocation.getColumn())].addContent(ContentType::Wall);


            case 45:
                knownWorld[Coordinate(CurrentLocation.getRow()-1, CurrentLocation.getColumn())].addContent(ContentType::Wall);
            case 135:
                knownWorld[Coordinate(CurrentLocation.getRow(), CurrentLocation.getColumn()-1)].addContent(ContentType::Wall);
            case 225:
                knownWorld[Coordinate(CurrentLocation.getRow()+1, CurrentLocation.getColumn())].addContent(ContentType::Wall);
            case 315:
                knownWorld[Coordinate(CurrentLocation.getRow(), CurrentLocation.getColumn()+1)].addContent(ContentType::Wall);
        }
    }

    if(IRs[4])
    {
        ///Wall on left
          switch(currentAngle)
        {
            case 0:
                knownWorld[Coordinate(CurrentLocation.getRow()-1, CurrentLocation.getColumn())].addContent(ContentType::Wall);
            case 180:
                knownWorld[Coordinate(CurrentLocation.getRow()+1, CurrentLocation.getColumn())].addContent(ContentType::Wall);
            case 90:
                knownWorld[Coordinate(CurrentLocation.getRow(), CurrentLocation.getColumn()-1)].addContent(ContentType::Wall);
            case 270:
                knownWorld[Coordinate(CurrentLocation.getRow(), CurrentLocation.getColumn()+1)].addContent(ContentType::Wall);

            case 45:
                knownWorld[Coordinate(CurrentLocation.getRow()-1, CurrentLocation.getColumn())].addContent(ContentType::Wall);
            case 135:
                knownWorld[Coordinate(CurrentLocation.getRow()+1, CurrentLocation.getColumn())].addContent(ContentType::Wall);
            case 225:
                knownWorld[Coordinate(CurrentLocation.getRow(), CurrentLocation.getColumn()-1)].addContent(ContentType::Wall);
            case 315:
                knownWorld[Coordinate(CurrentLocation.getRow(), CurrentLocation.getColumn()+1)].addContent(ContentType::Wall);
        }
    }
    ///else no walls.
}

void ImplementAgent::evaluateRealayRange(){std::cout<<"no evaluation relay range";}
