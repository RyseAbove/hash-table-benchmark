#pragma once
#include <vector>
#include <functional>

//INHERITANCE
//Base class that provides an interface the two subclasses will inherit
//This is an abstract (pure virtual) class that does nothing
//But it allows us to avoid copy/pasting code in main because the interface is the same for both hash tables
template <class T, size_t SIZE>
class Hash {
	public:
		virtual ~Hash() {}
		virtual void insert(T new_data) = 0;
		virtual void remove(T old_data) = 0;
		virtual bool search(T test_data) const = 0;
		virtual void change(T old_data, T new_data) = 0;
};

void die () {
	std::cout << "BAD INPUT" << std::endl;
	std::exit(0);
}
