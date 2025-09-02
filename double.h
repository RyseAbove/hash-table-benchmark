#include "hash.h"
#include <vector>
#include <functional>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/miller_rabin.hpp>
using boost::multiprecision::cpp_int;

bool checkPrime(cpp_int test) { return boost::multiprecision::miller_rabin_test(test, 25); }
cpp_int findPrime(cpp_int SIZE) {
	cpp_int testInt = SIZE - 1;
	while (testInt > 2) {
		if (checkPrime(testInt)) return testInt;
		--testInt;
	}
	return 2;
}


//INHERITANCE
//Base class that provides an interface the two subclasses will inherit
//This is an abstract (pure virtual) class that does nothing
//But it allows us to avoid copy/pasting code in main because the interface is the same for both hash tables
template <class T, size_t SIZE>

//YOU: Write a third hashing class here, following the pattern above. You can do it using double hashing, quadratic hashing, or the unordered_set STL class, whatever you like.
class DoubleHash final : public Hash<T,SIZE> { //This class cannot be inherited from
	//This bit of magic makes three constants: STATUS::OPEN, STATUS::FILLED, and STATUS::DELETED
	//OPEN means the bucket has never held a value in it
	//FILLED means it currently holds a valid value in it
	//DELETED means the value in it has been deleted and not replaced yet
	enum class STATUS : char {OPEN, FILLED, DELETED};
	std::vector<T> data;
	std::vector<STATUS> status; //Each element can be STATUS::OPEN, STATUS::FILLED or STATUS::DELETED
	cpp_int prime;
	
	int hash_1(T new_data) const {
		return new_data % SIZE;
	}
	
	// finish defining prime 
	
	int hash_2(T key) const {
		return static_cast<int>(prime - (key % prime));
	}

	public:
	DoubleHash() {
		data.resize(SIZE); //Set the vector to be SIZE big - SIZE is a template parameter
		status.resize(SIZE,STATUS::OPEN); //Set the state of every bucket to STATUS::OPEN
		prime = findPrime(SIZE);
	}
	
	size_t address = 0;
	void insert(T new_data) {
		size_t address = std::hash<T>{} (new_data) % SIZE;
		while (status.at(address) == STATUS::FILLED) {
			address = hash_2(new_data);
		}
		
	};
	//RTTI Run Time Type Information - vtable
	//Final turns this off if you get a LinearProbingHash& in a function parameter, it is devirtualized
	/*void insert(T new_data) final override {
		//Step 1 - Compute the initial bucket to try
		size_t address = std::hash<T>{}(new_data) % SIZE;
		//TODO: Make this not infinite loop on a full hash table
		while (status.at(address) == STATUS::FILLED)
			address = (address+1)%SIZE; //Move one bucket to the right, looping around
		//Ok, we've found an open spot, write the data in and mark it as filled
		data.at(address) = new_data;
		status.at(address) = STATUS::FILLED;
	}*/
	//YOU:
	void remove(T old_data) override {
		int h = hash_1(old_data);
		int h_2 = hash_2(old_data);

		for (int i = 0; i < SIZE; i++) {
			int bucket = (h + i * h_2) % SIZE;
			if (status[address] == STATUS::OPEN) return;
			if (status[address] == STATUS::FILLED && data[address] == old_data) {
				status[address] = STATUS::DELETED;
				return;
			}
		}
	};
	//YOU:
	bool search(T old_data) const override {
		int h = hash_1(old_data);
		int h_2 = hash_2(old_data);

		for (int i = 0; i < SIZE; i++) {
			int bucket = (h + i * h_2) % SIZE;
			if (status[address] == STATUS::OPEN) return false;
			if (status[address] == STATUS::FILLED && data[address] == old_data) return true;
		}
	};
	//YOU:
	void change(T old_data, T new_data) override {
		//Remember, only change it if it actually is in the hash table
		if (search(old_data)) {
			remove(old_data);
			insert(new_data);
		}
	};
};





