#include "stdafx.h"
#include "BasicBotExampleOne.h"
#include <vector>

/*
	BasicBotExampleOne is a very simple bot. If the exit is within sight when the bot enters a
	room it will move towards it. However, if the exit is not within sight, the bot does nothing.
*/

bool hangingOnLedge = false;
bool facingLeft = true;


struct BasicBotExampleOne::Tuple 
{
	int x;
	int y;
};

void createTuple(int x, int y, std::vector<BasicBotExampleOne::Tuple> &foo) 
{
	BasicBotExampleOne::Tuple result;
	result.x = x;
	result.y = y;
	foo.push_back(result);
}

bool pssableNode(int arg) {
	return (arg == 4 || arg == 0 || arg == 3 || arg == 2 || arg == 10);
}

std::vector<BasicBotExampleOne::Tuple> BasicBotExampleOne::Reversibles(int hx, int hy) 
{
	std::vector<BasicBotExampleOne::Tuple> foo;

	if(hangingOnLedge)
	{
		if(facingLeft)
		{
			//can jump up to x-1,y-1
			createTuple(hx-1, hy-1, foo);

			
		} else {
			//can jump up to x+1,y-1
			createTuple(1,-1, foo);
		}

		if(pssableNode(GetNodeState(hx, hy + 1, 0)))
		{
			if (pssableNode(GetNodeState(hx, hy + 2, 0)) && !pssableNode(GetNodeState(hx, hy + 3, 0)))
				createTuple(0, 2, foo);
		} else {
			//can drop down to x,y+1
			createTuple(0, 1, foo);
		}


	} else {
		
		int temp = GetNodeState(hx + 1, hy, 0);

		if ( pssableNode(GetNodeState(hx + 1, hy, 0)) == true )
		{
			if(pssableNode(GetNodeState(hx + 1, hy - 1, 0)))
			{
				if (!pssableNode(GetNodeState(hx + 2, hy - 1, 0)) && pssableNode(GetNodeState(hx + 2, hy - 2, 0)))
					//can jump and grab x+2,y+1
					createTuple(2, 1, foo);
			}

			if(pssableNode(GetNodeState(hx + 1, hy + 1, 0)))
			{
				//can fliphang onto right side of x,y+1
				createTuple(1, 1, foo);
				if(!pssableNode(GetNodeState(hx + 2, hy + 1, 0)))
				{
					//can sprint right onto x+2,y
					createTuple(2, 0, foo);
				}

				if(pssableNode(GetNodeState(hx + 2, hy + 1, 0)))
				{
					if (!pssableNode(GetNodeState(hx + 1, hy + 3, 0)))
						//can stand on x+1,y+3
						createTuple(1, 3, foo);
				} else {
					//can hang on right side of x,y+1 (?)
					createTuple(1, 1, foo);
				}

				if (!pssableNode(GetNodeState(hx, hy - 1, 0)) && pssableNode(GetNodeState(hx, hy - 2, 0)))
					//can jump and hang on right side of x,y-1
					createTuple(1, -1, foo);
				
			} else {
				//can simply walk to stand on x + 1, y + 1
					createTuple(1, 1, foo);
			}
		} 
	
		if(pssableNode(GetNodeState(hx - 1, hy, 0)))
		{
			if(pssableNode(GetNodeState(hx - 1, hy - 1, 0)))
			{
				if (!pssableNode(GetNodeState(hx - 2, hy - 1, 0)) && pssableNode(GetNodeState(hx - 2, hy - 2, 0))) 
					createTuple(-2, 1, foo);
				//can jump and grab x-2,y+1
			}

			if(pssableNode(GetNodeState(hx - 1, hy + 1, 0)))
			{
				if(!pssableNode(GetNodeState(hx - 2, hy + 1, 0)))
				{
					//can sprint left onto x-2,y
					createTuple(-2, 0, foo);
				}

				if(pssableNode(GetNodeState(hx - 1, hy + 2, 0)))
				{
					if (!pssableNode(GetNodeState(hx - 1, hy + 3, 0)))
						//can stand on x+1,y+3
						createTuple(1, 3, foo);
				} else {
					//can hang on left side of x,y+1 (?)
					createTuple(-1, 1, foo);
				}

				if (!pssableNode(GetNodeState(hx, hy - 1, 0)) && pssableNode(GetNodeState(hx, hy - 2, 0)))
					//can jump and hang on left side of x,y-1
					createTuple(-1, -1, foo);

			} else {
				//can simply walk to stand on x-1,y+1
				createTuple(-1, 1, foo);
			}
		} 

		if(pssableNode(GetNodeState(hx, hy - 1, 0)))
		{
			if(pssableNode(GetNodeState(hx + 1, hy - 1, 0)))
			{
				if (!pssableNode(GetNodeState(hx + 1, hy, 0)))
					createTuple(1, 0, foo);
			} else {
				createTuple(1, -1, foo);
			}

			if(pssableNode(GetNodeState(hx - 1, hy - 1, 0)))
			{
				if (!pssableNode(GetNodeState(hx - 1, hy, 0)))
					createTuple(-1, 0, foo);
			} else {
				createTuple(-1, -1, foo);
			}
		}
	}
	return foo;
}

bool processed[42][34]; //remember to reinitialize per-level
struct Node 
{
	int x;
	int y;
	int* group;
};

void BasicBotExampleOne::scanForGroups(int x, int y) //called recursively
{
	std::vector<Tuple> tmp;
	if (pssableNode(GetNodeState(x, y, 0))) 
	{
		tmp = Reversibles(x, y);
		for (int i = 0; i < tmp.size; i++) 
		{
			if (!processed[tmp[i].x][tmp[i].y]) 
			{
				scanForGroups(tmp[i].x, tmp[i].y);
			}
		}
	}
}



void BasicBotExampleOne::NewLevel()
{
	//figure out if this is called before or after ClearFogFromSquare
	/*

	int hx = 0;
	int hy = 0;

	if(hangingOnLedge)
	{
		if(facingLeft)
		{
			//can jump up to x-1,y-1
		} else {
			//can jump up to x+1,y-1
		}

		if(pssableNode(GetNodeState(hx, hy + 1, 0)))
		{
			if(pssableNode(GetNodeState(hx, hy + 2, 0)) && !pssableNode(GetNodeState(hx, hy + 3, 0)))
				//can drop down to x,y+2
		} else {
			//can drop down to x,y+1
		}


	} else {
		
		if(pssableNode(GetNodeState(hx + 1, hy, 0)))
		{
			if(pssableNode(GetNodeState(hx + 1, hy - 1, 0)))
			{
				if(!pssableNode(GetNodeState(hx + 2, hy - 1, 0)) && pssableNode(GetNodeState(hx + 2, hy - 2, 0)))
					//can jump and grab x+2,y+1
			}

			if(pssableNode(GetNodeState(hx + 1, hy + 1, 0)))
			{
				can fliphang onto right side of x,y+1
				if(!pssableNode(GetNodeState(hx + 2, hy + 1, 0)))
				{
					can sprint right onto x+2,y
				}

				if(pssableNode(GetNodeState(hx + 2, hy + 1, 0)))
				{
					if(!pssableNode(GetNodeState(hx + 1, hy + 3, 0)))
						can stand on x+1,y+3
				} else {
					can hang on right side of x,y+1 (?)
				}

				if(x,y-1 is NOT empty && x,y-2 is empty)
					can jump and hang on right side of x,y-1
				
			} else {
				can simply walk to stand on x+1,y+1
			}
		} 
	
		if((x-1,y) is empty)
		{
			if(x-1,y-1 is empty)
			{
				if(x-2,y-1 is NOT empty && x-2,y-2 is empty)
				can jump and grab x-2,y+1
			}

			if(x-1,y+1 is empty)
			{
				can fliphang onto left side of x,y+1
				if(x-2,y+1 is NOT empty)
				{
					can sprint left onto x-2,y
				}

				if(x-1,y+2 is empty)
				{
					if(x-1,y+3 is NOT empty)
						can stand on x+1,y+3
				} else {
					can hang on left side of x,y+1 (?)
				}

				if(x,y-1 is NOT empty && x,y-2 is empty)
					can jump and hang on left side of x,y-1

			} else {
				can simply walk to stand on x-1,y+1
			}
		} 

		if(x,y-1 is empty) 
		{
			if((x+1,y-1) is empty)
			{
				if(x+1,y is NOT empty)
					can jump and stand on x+1,y
			} else {
				can jump and grab left side of x+1,y-1
			}

			if((x-1,y-1) is empty)
			{
				if(x-1,y is NOT empty)
					can jump and stand on x-1,y
			} else {
				can jump and grab right side of x-1,y-1
			}
		}
	}

	*/

}

bool tenthFrame = false;
int frameC = 0;



void BasicBotExampleOne::Update()	
{

	if (tenthFrame)
	{
		
		int nodState = 0, flors = 0, walls = 0;
		for (int yPos = 0; yPos <= 33; yPos++)
		{
			for (int xPos = 0; xPos <= 41; xPos++)
			{
				nodState = GetNodeState(xPos, yPos, 0);
				if (pssableNode(nodState))
				{
					
					if (xPos == 41 || !pssableNode(GetNodeState(xPos + 1, yPos, 0))) 
						walls += 1;
					if (xPos == 0 || !pssableNode(GetNodeState(xPos - 1, yPos, 0))) 
						walls += 1;
					if (yPos == 33 || !pssableNode(GetNodeState(xPos, yPos + 1, 0)))
					{
						std::cout << "X: " << xPos << " Y: " << yPos << std::endl;
						flors += 1;
					}
				}
				//std::cout << "X: " << xPos <<  " Y: " << yPos << std::endl;
			}
		}
	
		std::cout << "FLOORS: " << flors << std::endl;
		std::cout << "WALLS: " << walls << std::endl;
	
		tenthFrame = false;
		
	}
	
	if (frameC < 12)
	{
		if (frameC == 10)
		{
			tenthFrame = true;
		}
		frameC++;
	}


	/*
	if (firstFrame)
	{
		int floors = 0;
		int walls = 0;

		std::cout << "UPPER LEFT: " << GetNodeState(0,0,0) << std::endl;

		for (int yPos = 1; yPos <= 33; yPos++)
		{

			bool recentEmpty = false;

			for (int xPos = 1; xPos <= 41; xPos++)
			{
				if ((GetNodeState(xPos, yPos + 1, 0) == 0 || GetNodeState(xPos, yPos + 1, 0) == 3 || GetNodeState(xPos, yPos + 1, 0) == 4)
					&& GetNodeState(xPos, yPos, 0) != 0 && GetNodeState(xPos, yPos, 0) != 3 && GetNodeState(xPos, yPos, 0) != 4)
				{
					floors += 1;
					//create new floor surface on xPos, yPos
				}

				if (yPos == 1 && (GetNodeState(xPos, yPos, 0) == 0 || GetNodeState(xPos, yPos, 0) == 3 || GetNodeState(xPos, yPos, 0) == 4))
					floors += 1;

				if (recentEmpty != (GetNodeState(xPos, yPos, 0) == 0 || GetNodeState(xPos, yPos, 0) == 3 || GetNodeState(xPos, yPos, 0) == 4))
				{
					walls += 1;
					//create surface between x-1,y and x,y
				}

				recentEmpty = (GetNodeState(xPos, yPos, 0) == 0 || GetNodeState(xPos, yPos, 0) == 3 || GetNodeState(xPos, yPos, 0) == 4);
			}

			if (GetNodeState(41, yPos, 0) == 0 || GetNodeState(41, yPos, 0) == 3 || GetNodeState(41, yPos, 0) == 4)
			{
				walls += 1;
				//create surface between 40,yPos and 41,yPos
			}

		}

		std::cout << "FLOORS: " << floors << std::endl;
		std::cout << "WALLS: " << walls << std::endl;

		firstFrame = false;

	}
	*/

	if (!_hasGoal)
	{
		// If no goal, search for the exit
		for (int y = 0; y < Y_NODES; y++)
		{
			for (int x = 0; x < X_NODES; x++)
			{
				if (GetNodeState(x, y, NODE_COORDS) == spExit)
				{
					_hasGoal = true;
					_itemGoal = true;
					_targetX = x * PIXELS_IN_NODES;
					_targetY = y * PIXELS_IN_NODES;
					CalculatePathFromXYtoXY(_playerPositionX, _playerPositionY, _targetX, _targetY, PIXEL_COORDS);
				}
			}
		}
	}
	else
	{
		// If goal, move towards it
		if (_playerPositionXNode < (GetNextPathXPos(_playerPositionXNode, _playerPositionYNode, NODE_COORDS)))
		{
			_goLeft = true;
		}
		else
		{
			_goLeft = true;
		}
	}
}