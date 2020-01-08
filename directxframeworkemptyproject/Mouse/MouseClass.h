#pragma once
#include "MouseEvent.h"
#include <queue>

class MouseClass
{
	public:
	MouseClass();

	void OnLeftPressed(int x,int y);
	void OnLeftRelesed(int x, int y);
	void OnRightPressed(int x, int y);
	void OnRightRelesed(int x, int y);
	void OnMiddlePressed(int x, int y);
	void OnMiddleRelesed(int x, int y);
	void OnWheelUp(int x, int y);
	void OnWheelDown(int x, int y);
	void OnMouseMove(int x, int y);
	void OnMouseMoveRaw(int x, int y);


	bool IsLeftDown();
	bool IsMiddleDown();
	bool IsRightDown();

	int GetPosX();
	int GetPosY();
	MousePoint GetPos();

	bool EventBufferIsEmpty();
	MousePoint RawInput;
	void ClearRawInput();
	MouseEvent ReadEvent();
	private:
	std::queue<MouseEvent> eventBuffer;
	bool IsLeftButtonDown = false;
	bool IsRightButtonDown = false;
	bool IsMiddleButtonDown = false;
	int x = 0;
	int y = 0;

};
