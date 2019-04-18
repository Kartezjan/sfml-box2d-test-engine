#pragma once

#include <memory>
#include <vector>

template<typename T, typename... CArgs>
std::vector<std::unique_ptr<T>> init_vec_ptr(const std::size_t count, CArgs&&... args)
{
	auto dest = std::vector<std::unique_ptr<T>>{};
	for (std::size_t i = 0; i < count; ++i)
		dest.emplace(dest.end(), std::make_unique<T>(std::forward<CArgs>(args)...));
	return dest;
}

template<typename T, typename... Args, template <typename, typename...> class Container, typename... CArgs >
Container<std::unique_ptr<T>, Args...> deep_copy_vec_ptr(const Container<std::unique_ptr<T>, Args...>& src, CArgs&&... args)
{
	auto dest = Container<std::unique_ptr<T>, Args...>{};
	for (const auto& e : src)
		dest.emplace(dest.end(), std::make_unique<T>(*e, std::forward<CArgs>(args)...));
	return dest;
}

template<typename T, typename K, typename... ContArgs, template <typename, typename, typename...> class Container, typename... FArgs>
Container<K, std::unique_ptr<T>, ContArgs...> deep_copy_vec_ptr(const Container<K, std::unique_ptr<T>, ContArgs...>& src, FArgs&&... args)
{
	auto dest = Container<K, std::unique_ptr<T>, ContArgs...>{};
	for (const auto& e : src)
		dest.emplace(e.first, std::unique_ptr<T>(new T{ *e.second, std::forward<FArgs>(args)... }));
	return dest;
}