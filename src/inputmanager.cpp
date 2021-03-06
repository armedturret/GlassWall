#include <inputmanager.h>
#include <SDL.h>
#include <iostream>
#include <glm/glm.hpp>

GW::InputManager::InputManager()
{
	m_quitRequested = false;
}

GW::InputManager::~InputManager()
{
}

void GW::InputManager::update()
{
	m_keyValuesLast = m_keyValues;
	m_mouseButtonsLast = m_mouseButtons;

	//reset mouse motion relative
	m_mouseMotion.xRel = 0;
	m_mouseMotion.yRel = 0;

	//reset mouse wheel movement
	m_wheelMovement = 0;

	//handle all events
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			//user requested quit
			m_quitRequested = true;
			break;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
		{
			//update our key to up if found
			auto it = m_keyValues.find(evnt.key.keysym.sym);
			if (it != m_keyValues.end()) {
				//key found, update
				it->second = evnt.key.state;
			}
			else {
				//create entry for key
				m_keyValues.insert(std::make_pair(evnt.key.keysym.sym, evnt.key.state));
			}
			break;
		}
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		{
			//update our key to up if found
			auto it = m_mouseButtons.find(evnt.button.button);
			if (it != m_mouseButtons.end()) {
				//key found, update
				it->second = evnt.button.state;
			}
			else {
				//create entry for key
				m_mouseButtons.insert(std::make_pair(evnt.button.button, evnt.button.state));
			}
			break;
		}
		case SDL_MOUSEWHEEL:
		{
			//handle mouse wheel movement
			m_wheelMovement = evnt.wheel.y;
			break;
		}
		case SDL_MOUSEMOTION:
		{
			//update mouse motion values
			//y rel and ypos are inverted
			m_mouseMotion.xPos = evnt.motion.x;
			m_mouseMotion.yPos = m_height - evnt.motion.y;
			m_mouseMotion.xRel = evnt.motion.xrel;
			m_mouseMotion.yRel = -evnt.motion.yrel;
			break;
		}
		default:
			
			//unrecognized event, event should be handled
			//std::cout << "Unrecognized event type: " << evnt.type << "!\nPlease contact a dev with this information." << std::endl;
			break;
		}
	}
}

bool GW::InputManager::isKeyDown(int keycode)
{
	//check if key stored in data and then return value if found or false if not
	auto it = m_keyValues.find(keycode);
	if (it != m_keyValues.end()) {
		//key found, return value
		return it->second;
	}
	return false;
}

bool GW::InputManager::isMouseButtonDown(int buttoncode)
{
	//check if button stored in data and then return value if found or false if not
	auto it = m_mouseButtons.find(buttoncode);
	if (it != m_mouseButtons.end()) {
		//key found, return value
		return it->second;
	}
	return false;
}

bool GW::InputManager::keyPressed(int keycode)
{
	auto it = m_keyValuesLast.find(keycode);
	bool pressedLastFrame = it != m_keyValuesLast.end() && it->second;
	return !pressedLastFrame && isKeyDown(keycode);
}

bool GW::InputManager::mousePressed(int buttoncode)
{
	auto it = m_mouseButtonsLast.find(buttoncode);
	bool pressedLastFrame = it != m_mouseButtonsLast.end() && it->second;
	return !pressedLastFrame && isMouseButtonDown(buttoncode);
}

bool GW::InputManager::quitRequested()
{
	return m_quitRequested;
}

GW::MouseMotion GW::InputManager::getMouseData()
{
	return m_mouseMotion;
}

void GW::InputManager::setMouseTrapped(bool value)
{
	SDL_SetRelativeMouseMode(SDL_bool(value));
}

void GW::InputManager::setWindowDimensions(int width, int height)
{
	m_width = width;
	m_height = height;
}

int GW::InputManager::getMouseWheelMovement()
{
	return m_wheelMovement;
}

void GW::InputManager::addAxis(std::string name, Axis axis)
{
	m_axes.insert(std::make_pair(name, axis));
}

float GW::InputManager::getAxisValue(std::string name)
{
	auto it = m_axes.find(name);
	if (it != m_axes.end()) {
		Axis axis = it->second;
		float sum = 0.0f;
		for (auto input : axis.axisInputs) {
			sum += input.scale * getValueDirection(input.code, input.type);
		}

		if(axis.limit)
			sum = glm::clamp(sum, axis.limitLower, axis.limitUpper);

		return sum;
	}
	return 0.0f;
}

float GW::InputManager::getValueDirection(int code, GW::AXISTYPE type)
{
	switch (type) {
	case GW::AXISTYPE::SDLKEYBOARD:
		return isKeyDown(code) ? 1.0f : 0.0f;
		break;
	case GW::AXISTYPE::SDLMOUSEBUTTON:
		return isMouseButtonDown(code) ? 1.0f : 0.0f;
		break;
	case GW::AXISTYPE::OTHER:
		//switch here
		switch (code) {
		case GW::INPUTCODE::MWHEELUP:
			return getMouseWheelMovement() > 0 ? 1.0f : 0.0f;
		case GW::INPUTCODE::MWHEELDOWN:
			return getMouseWheelMovement() < 0 ? 1.0f : 0.0f;
		case GW::INPUTCODE::MOUSEYUP:
			return glm::max((float)getMouseData().yRel, 0.0f);
		case GW::INPUTCODE::MOUSEYDOWN:
			return glm::max(-(float)getMouseData().yRel, 0.0f);
		case GW::INPUTCODE::MOUSEXRIGHT:
			return glm::max((float)getMouseData().xRel, 0.0f);
		case GW::INPUTCODE::MOUSEXLEFT:
			return glm::max(-(float)getMouseData().xRel, 0.0f);
		default:
			return 0.0f;
		}
	}

	return 0.0f;
}
