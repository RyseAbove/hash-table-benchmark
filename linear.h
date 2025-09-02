#pragma once
#include <vector>
#include <functional>
#include "hash.h"

//INHERITANCE
//Base class that provides an interface the two subclasses will inherit
//This is an abstract (pure virtual) class that does nothing
//But it allows us to avoid copy/pasting code in main because the interface is the same for both hash tables

//YOU: Write three hash tables, one for linear probing, one for chaining, one using any other method you want (Quadratic, Double Hashing, or using the STL unordered_set class are all ok)

template <class T, size_t SIZE>
class LinearProbingHash final : public Hash<T,SIZE> { //This class cannot be inherited from
	//This bit of magic makes three constants: STATUS::OPEN, STATUS::FILLED, and STATUS::DELETED
	//OPEN means the bucket has never held a value in it
	//FILLED means it currently holds a valid value in it
	//DELETED means the value in it has been deleted and not replaced yet
	enum class STATUS : char {OPEN, FILLED, DELETED};
	std::vector<T> data;
	std::vector<STATUS> status; //Each element can be STATUS::OPEN, STATUS::FILLED or STATUS::DELETED
	int inserts = 0;
	public:
	LinearProbingHash() {
		data.resize(SIZE); //Set the vector to be SIZE big - SIZE is a template parameter
		status.resize(SIZE,STATUS::OPEN); //Set the state of every bucket to STATUS::OPEN
	}
	//RTTI Run Time Type Information - vtable
	//Final turns this off if you get a LinearProbingHash& in a function parameter, it is devirtualized
	void insert(T new_data) final override {
		//Step 1 - Compute the initial bucket to try
		size_t address = std::hash<T>{}(new_data) % SIZE;
		//Step 2 - We will start at the bucket at "address" and then move to the right,
		//  looping around to the start if necessary, to find first available bucket
		//  Either an OPEN bucket or a DELETED bucket will do.
		//TODO: Make this not infinite loop on a full hash table
		if (inserts >= SIZE) die();
		while (status.at(address) == STATUS::FILLED) {
			address = (address+1)%SIZE;
		}
		//Ok, we've found an open spot, write the data in and mark it as filled
		data.at(address) = new_data;
		status.at(address) = STATUS::FILLED;
		inserts++;
	}
	//YOU:
	void remove(T old_data) override {
		size_t address = std::hash<T>{}(old_data) % SIZE;
		bool found = false;

		for (int i = 0; i < SIZE; i++) {
			if (status.at(address) == STATUS::FILLED && data.at(address) == old_data) {
				found = true;
				break;
			} else if (status.at(address) == STATUS::OPEN) die();
			address = (address+1)%SIZE;
		}
		if (found) {
			status.at(address) = STATUS::DELETED;
			inserts--;
		} else die();
	}
	//YOU:
	bool search(T old_data) const override {
		size_t address = std::hash<T>{}(old_data) % SIZE;
		
		for (int i = 0; i < SIZE; i++) {
			if (status.at(address) == STATUS::FILLED && data.at(address) == old_data) return true;
			else if (status.at(address) == STATUS::OPEN) return false;
			address = (address+1)%SIZE;
		}

		return false;
	}
	//YOU:
	void change(T old_data, T new_data) override {
		//Remember, only change it if it actually is in the hash table
		
		size_t address = std::hash<T>{}(old_data) % SIZE;
		bool found = false;

		for (int i = 0; i < SIZE; i++) {
			if (status.at(address) == STATUS::FILLED && data.at(address) == old_data) {
				found = true;
				break;
			} else if (status.at(address) == STATUS::OPEN) die();
			address = (address+1)%SIZE;
		}
		if (found) data.at(address) = new_data;
		else die();
	}
};
