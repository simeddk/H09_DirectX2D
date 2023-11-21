#pragma once

#include "IFocus.h"

class Follow : public Camera
{
public:
	Follow(IFocus* focusObject = nullptr);
	~Follow();

	void Update() override;

	void SetFocusObject(IFocus* newFocusObject);

private:
	IFocus* focusObject;
};
