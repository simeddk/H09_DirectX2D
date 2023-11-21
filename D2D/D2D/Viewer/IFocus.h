#pragma once

#include "stdafx.h"

class IFocus
{
public:
	virtual void Focus(OUT Vector2* focusPosition) = 0;
};