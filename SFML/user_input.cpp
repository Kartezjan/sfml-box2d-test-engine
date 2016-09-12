#include "user_input.h"

void produces_user_input::send_message(abstract_entity* source) {

	//update mouse pos

	cosmos.mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));


	//check if any of the desirable keys is pressed

	auto& keyboard_events = cosmos.message_queues.get_queue<input_message>();
	input_message message;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		message.key = input_key::W;
		keyboard_events.push_back(message);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		message.key = input_key::S;
		keyboard_events.push_back(message);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		message.key = input_key::A;
		keyboard_events.push_back(message);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		message.key = input_key::D;
		keyboard_events.push_back(message);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		message.key = input_key::SPACE;
		keyboard_events.push_back(message);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		message.key = input_key::UP;
		keyboard_events.push_back(message);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		message.key = input_key::DOWN;
		keyboard_events.push_back(message);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		message.key = input_key::LEFT;
		keyboard_events.push_back(message);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		message.key = input_key::RIGHT;
		keyboard_events.push_back(message);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
		message.key = input_key::Z;
		keyboard_events.push_back(message);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
		message.key = input_key::X;
		keyboard_events.push_back(message);
	}
	/*!!!!!!!!!!!!!!!!!!!!!!!*/
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F) && cosmos.universe_clock.getElapsedTime().asMilliseconds() - previous_timestamp >= 300) {
		previous_timestamp = cosmos.universe_clock.getElapsedTime().asMilliseconds();
		auto& camera_events = cosmos.message_queues.get_queue<camera_message>();
		camera_message cam_msg;
		cam_msg.type = event_type::FREE_CAMERA;
		camera_events.push_back(cam_msg);
	}
	/*!!!!!!!!!!!!!!!!!!!!!!!*/
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		message.key = input_key::LMB;
		keyboard_events.push_back(message);
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		message.key = input_key::RMB;
		keyboard_events.push_back(message);
	}

}