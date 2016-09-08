/* BASED ON: https://github.com/TeamHypersomnia/Augmentations/blob/master/augs/entity_system/storage_for_message_queues.h
	Author: Patryk Czachurski 
*/

#pragma once

#include <tuple>
#include <vector>

template<int... Is>
struct seq { };

template<int N, int... Is>
struct gen_seq : gen_seq<N - 1, N - 1, Is...> { };

template<int... Is>
struct gen_seq<0, Is...> : seq<Is...> { };


template<class T>
struct make_vector { typedef std::vector<T> type; };

template<typename T, typename F, int... Is>
void for_each(T&& t, F f, seq<Is...>)
{
	auto l = { (f(std::get<Is>(t)), 0)... };
}

template<typename... Ts, typename F>
void for_each_in_tuple(const std::tuple<Ts...>& t, F f)
{
	for_each(t, f, gen_seq<sizeof...(Ts)>());
}

template<template<typename> class Mod,
	typename ...Args>
	struct tuple_of {
	typedef std::tuple<typename Mod<Args>::type...> type;
};

template<template<typename> class Mod,
	typename ...Args>
	using tuple_of_t = typename tuple_of<Mod, Args...>::type;

template<class... Queues>
class message_storage {
	typedef tuple_of_t<make_vector, Queues...> tuple_type;
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

	void flush_queues() {
		for_each_in_tuple(queues, [this](auto& q) { q.clear(); });
	}


};