#include "stdafx.h"
#include "Clip.h"

//-----------------------------------------------------------------------------
// Frame
//-----------------------------------------------------------------------------
Frame::Frame(Sprite* Image, float Time)
	: Image(Image)
	, Time(Time)
{
}

Frame::~Frame()
{
	SafeDelete(Image);
}

//-----------------------------------------------------------------------------
// Clip
//-----------------------------------------------------------------------------
Clip::Clip(EPlayType playType, float playRate)
	: playType(playType)
	, playRate(playRate)
{
}

Clip::~Clip()
{
	for (Frame* frame : frames)
		SafeDelete(frame);
}

void Clip::Update()
{
	Frame* frame = frames[currentFrame];

	if (bPlaying == true)
	{
		float keepFrameTime = frame->Time / playRate;
		playTime += Time::Delta();

		if (playTime >= keepFrameTime)
		{
			currentFrame++;

			switch (playType)
			{
				case EPlayType::OnceKeepFirstFrame:
				case EPlayType::OnceKeepLastFrame:
				{
					if (currentFrame >= frames.size())
						Stop();
				}
				break;

				case EPlayType::Loop:
				{
					currentFrame %= frames.size();
				}
				break;
			}//switch

			playTime = 0.f;

		} //if(nextFrame)
	} //if(bPlaying)

	frames[currentFrame]->Image->Scale(scale);
	frames[currentFrame]->Image->Position(position);

	frames[currentFrame]->Image->Update();
}

void Clip::Render()
{
	Frame* frame = frames[currentFrame];
	frame->Image->Render();
}

void Clip::AddFrame(Sprite* sprite, float time)
{
	frames.push_back(new Frame(sprite, time));
}

void Clip::Play()
{
	playTime = 0.f;
	currentFrame = 0;
	bPlaying = true;
}

void Clip::Play(UINT startFrame)
{
	playTime = 0.f;
	currentFrame = startFrame;
	bPlaying = true;
}

void Clip::Stop()
{
	bPlaying = false;

	switch (playType)
	{
		case EPlayType::OnceKeepFirstFrame:
		{
			currentFrame = 0.f;
		}
		break;

		case EPlayType::OnceKeepLastFrame:
		{
			currentFrame = frames.size() - 1;
		}
		break;
	}
}

void Clip::Position(float x, float y)
{
	Position(Vector2(x, y));
}

void Clip::Position(Vector2& vec)
{
	position = vec;
}

void Clip::Scale(float x, float y)
{
	Scale(Vector2(x, y));
}

void Clip::Scale(Vector2& vec)
{
	scale = vec;
}
