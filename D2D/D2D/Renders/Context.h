#pragma once

class Context
{
private:
	Context();
	~Context();

public:
	static void Create();
	static void Delete();
	static Context* Get();

public:
	void Update();

	const Matrix& View();
	const Matrix& Projection();

	class Camera* GetCamera() { return camera; }
	void SetFollowCamera(class IFocus* focusObject = nullptr);

private:
	static Context* instance;

private:
	class Camera* camera = nullptr;
	Matrix projection;
};