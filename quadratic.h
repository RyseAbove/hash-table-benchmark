#include <vector>
#include <functional>
#include "hash.h"
#pragma once

template <class T, size_t SIZE>
class QuadraticHash : public Hash<T,SIZE> {
	enum class STATUS: char {OPEN, FILLED, DELETED};
	std::vector<T> data;
	std::vector<STATUS> status;

	public:
		QuadraticHash() {
        	data.resize(SIZE);
        	status.resize(SIZE,STATUS::OPEN); 
		}	
   
    void remove(T old_data) override {
		const size_t h = std::hash<T>{}(old_data) % SIZE;
        for (size_t i = 0; i < SIZE; ++i) {
            size_t address = (h+i*i) % SIZE;
            if (status.at(address) == STATUS::OPEN) {
				return;
			};
            if (status.at(address) == STATUS::FILLED && data.at(address) == old_data) {
                status.at(address) = STATUS::DELETED;
				return;
          	 };
		}
    }

    bool search(T test_data) const override {
		const size_t h = std::hash<T>{}(test_data) % SIZE;
		for (size_t i = 0; i < SIZE; ++i) {
			size_t address = (h+i*i) % SIZE;
			if (status.at(address) == STATUS::OPEN) {
				return false;
			}
			if (status.at(address) == STATUS::FILLED && data.at(address) == test_data)
				return true;
			};
        return false;
    }

    void change(T old_data, T new_data) override {
		if (this->search(old_data)) {
			this->remove(old_data);
			this->insert(new_data);
		};
    }

	void insert(T new_data) override {
		const size_t h = std::hash<T>{}(new_data) % SIZE;
		for (size_t i = 0; i < SIZE; ++i) {
			size_t address = (h+i*i) % SIZE;
			if (status.at(address) == STATUS::FILLED) {
				if (data.at(address) == new_data) {
					return;
				};
				continue;
			};
            data.at(address) = new_data;
            status.at(address) = STATUS::FILLED;
            return;
            };
      }
};

