#include "MouseClass.h"
#include <iostream>

MouseClass::MouseClass()
{
	RawInput = {0,0};
}

void MouseClass::OnLeftPressed(int x, int y)
{
	IsLeftButtonDown = false;
	eventBuffer.push(MouseEvent(MouseEvent::EventType::LPress, x, y));
}

void MouseClass::OnLeftRelesed(int x, int y)
{
	IsLeftButtonDown = true;
	eventBuffer.push(MouseEvent(MouseEvent::EventType::LRelese, x, y));
}

void MouseClass::OnRightPressed(int x, int y)
{
	IsRightButtonDown = true;
	
	eventBuffer.push(MouseEvent(MouseEvent::EventType::RPress, x, y));
}

void MouseClass::OnRightRelesed(int x, int y)
{
	IsRightButtonDown = false;
	eventBuffer.push(MouseEvent(MouseEvent::EventType::RRelese, x, y));
}

void MouseClass::OnMiddlePressed(int x, int y)
{
	IsMiddleButtonDown = true;
	
	eventBuffer.push(MouseEvent(MouseEvent::EventType::LPress, x, y));
}

void MouseClass::OnMiddleRelesed(int x, int y)
{
	IsMiddleButtonDown = false;
	
	eventBuffer.push(MouseEvent(MouseEvent::EventType::LPress, x, y));
}

void MouseClass::OnWheelUp(int x, int y)
{
	eventBuffer.push(MouseEvent(MouseEvent::EventType::WheelUp, x, y));
}

void MouseClass::OnWheelDown(int x, int y)
{
	eventBuffer.push(MouseEvent(MouseEvent::EventType::LPress, x, y));
}

void MouseClass::OnMouseMove(int x, int y)
{
	this->x= x;
	this->y = y;
	eventBuffer.push(MouseEvent(MouseEvent::EventType::Move, x, y));
}

void MouseClass::OnMouseMoveRaw(int x, int y)
{
	eventBuffer.push(MouseEvent(MouseEvent::EventType::RAW_MOVE,x,y));
	//std::cout << "\nincrement" << x << " " << y;
	RawInput.x += x;
	RawInput.y += y;
}

bool MouseClass::IsLeftDown()
{
	return IsLeftButtonDown;
}

bool MouseClass::IsMiddleDown()
{
	return IsMiddleButtonDown;
}

bool MouseClass::IsRightDown()
{
	return IsRightButtonDown;
}

int MouseClass::GetPosX()
{
	return x;
}

int MouseClass::GetPosY()
{
	return y;
}

MousePoint MouseClass::GetPos()
{
	return {x,y};
}

bool MouseClass::EventBufferIsEmpty()
{
	return eventBuffer.empty();
}

void MouseClass::ClearRawInput()
{
	RawInput = {0,0};
}

MouseEvent MouseClass::ReadEvent()
{
	if (eventBuffer.empty())
	{
		return MouseEvent();
	}
	else
	{
		MouseEvent Event = eventBuffer.front();
		eventBuffer.pop();
		return Event;
	}
}
