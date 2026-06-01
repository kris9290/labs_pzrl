#include"HashTable.h"
#include<stdexcept>

HashTable::HashTable(size_t size) noexcept : _capacity(static_cast<int32_t>(size)), _filled(0), table(_capacity) {}

HashTable::~HashTable() = default;

static size_t raw_hash(const KeyType& key)
{
	size_t hash = 5381;
	for (char c : key)
	{
		hash = ((hash << 5) + hash) + static_cast<size_t>(c);
	}
	return hash;
}

size_t HashTable::hash_function(const KeyType& key) const
{
	return raw_hash(key) % _capacity;
}

void HashTable::insert(const KeyType& key, const ValueType& value)
{
	if(static_cast<double>(_filled + 1) / _capacity > 0.75)
	{
		int32_t new_capacity = _capacity*2;
		std::vector<std::list<std::pair<KeyType, ValueType>>> new_table(new_capacity);
		for(const auto& b : table)
		{
			for(const auto& pair : b)
			{
				size_t new_index = raw_hash(pair.first) % new_capacity;
				new_table[new_index].push_back(pair);
			}

		}
		table.swap(new_table);
		_capacity = new_capacity;
	}
	size_t index = hash_function(key);
	auto& b = table[index];
	for(auto& pair : b)
	{
		if (pair.first == key)
		{
			pair.second = value;
			return;
		}
	}
	b.emplace_back(key, value);
	++_filled;
}

bool HashTable::find(const KeyType& key, ValueType& value) const
{
	size_t index = hash_function(key);
	const auto& b = table[index];
	for(const auto& pair : b)
	{
		if(pair.first == key)
		{
			value = pair.second;
			return true;
		}
	}
	return false;
}

void HashTable::remove(const KeyType& key)
{
	size_t index = hash_function(key);
	auto& b = table[index];
	for(auto it = b.begin(); it != b.end(); ++it)
	{
		if(it->first == key)
		{
			b.erase(it);
			--_filled;
			return;
		}
	}
}

ValueType& HashTable::operator[](const KeyType& key)
{
	size_t index = hash_function(key);
	auto& b = table[index];
	for(auto& pair : b)
	{
		if(pair.first == key)
		{
			return pair.second;
		}
	}
	insert(key, ValueType{});
	for(auto& pair : table[hash_function(key)])
	{
		if(pair.first == key)
			return pair.second;
	}
	throw std::runtime_error("HashTable::operator[]: insertion failled");
}

double HashTable::getLoadFactor()
{
	return static_cast<double>(_filled)/_capacity;
}

