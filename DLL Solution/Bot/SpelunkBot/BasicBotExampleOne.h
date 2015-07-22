#pragma once

#include "IBot.h"
#include <vector>

class BasicBotExampleOne : public IBot
{
public:
	BasicBotExampleOne() { }
	virtual ~BasicBotExampleOne() { }
	struct Tuple;
	void NewLevel();
	std::vector<Tuple> Reversibles(int hx, int hy);
	void scanForGroups(int x, int y);
	void Update() override;
};