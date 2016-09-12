//BASED ON: https://github.com/TeamHypersomnia/Augmentations/blob/master/augs/entity_system/storage_for_message_queues.h

#pragma once
#include <tuple>
#include <vector>
#include "templates.h"

namespace detail {
	template<class T>
	struct make_vector { typedef std::vector<T> type; };
}

template<class... Queues>
class message_storage {
	typedef tuple_of_t<::detail::make_vector, Queues...> tuple_type;
	tuple_type queues;

public:

	template <typename T>
	void post(const T& message_object) {
	get_queue<T>().push_back(message_object);
	}

	template <typename T>
	void post(const std::vector<T>& messages) {
		get_queue<T>().insert(get_queue<T>().end(), messages.begin(), messages.end());
	}

	template <typename T>
	std::vector<T>& get_queue() {
		return std::get<std::vector<T>>(queues);
	}

	template <typename T>
	void clear_queue() {
		return get_queue<T>().clear();
	}

	template <typename T>
	void delete_marked(std::vector<T>& messages) {
		messages.erase(std::remove_if(messages.begin(), messages.end(), [](const T& msg) {
			return msg.delete_this_message;
		}), messages.end());
	}

	template <typename T>
	void delete_marked() {
		delete_marked_messages(get_queue<T>());
	}

	void delete_all_marked() {
		for_each_in_tuple(queues, [this](auto& q) { delete_marked(q); });
	}

	template <typename T>
	void check_lifetime(std::vector<T>& messages) {
		for (T& msg : messages) {
			if (msg.lifetime < 1)
				msg.delete_this_message = true;
			else
				--msg.lifetime;
		}
	}

	template <typename T>
	void check_lifetime() {
		check_lifetime(get_queue<T>());
	}

	void check_all_lifetimes() {
		for_each_in_tuple(queues, [this](auto& q) { check_lifetime(q); });
	}

	void flush_queues() {
		for_each_in_tuple(queues, [this](auto& q) { q.clear(); });
	}
};

