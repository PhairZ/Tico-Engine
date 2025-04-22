#pragma once

#include <memory>

#define SINGLETON(class_name) public:																					\
	class_name(class_name&) = delete;													\
	class_name& operator=(class_name&) = delete;										\
	class_name(class_name&&) = delete;													\
	class_name& operator=(class_name&&) = delete;										\
																						\
	friend std::unique_ptr<class_name> std::make_unique<class_name>();					\
	static class_name& get_singleton() {												\
		static std::unique_ptr<class_name> instance = std::make_unique<class_name>();	\
		return *instance;																\
	}																					\
private:																				\
	class_name()
