#pragma once

class Background
{
public:
	Background(Shader* shader);
	~Background();

	void Update();
	void Render();

private:
	Sprite* cloud[2];
	Sprite* bush[2];
	Sprite* tile;

	class Rect* rect;
	Shader* rectShader = nullptr;
};