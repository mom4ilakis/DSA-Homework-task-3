#pragma once
#include <unordered_set>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <iostream>
#include <unordered_map>

typedef std::vector<std::string> str_vec;
typedef std::queue<std::string> str_queue;
typedef std::unordered_set<std::string> str_set;

#define SEP " -> "
#define COL "[color=red, style=filled, fillcolor=\"#ffefef\"]"
#define EMPTY_KEY "NULL"
#define QT "\""

class Map
{
	

private:
	class Zone
	{
	public:
		Zone();
		Zone(const std::string& z_name, const str_vec& neighbors, const str_vec& z_keys, const std::string& entry_key = EMPTY_KEY);
		~Zone();



		void visit(str_set& KeyRing);

		void makeExplored();

		//keyRing holds all the keys the "player" has collected thus far
		bool canEnter(const str_set& keyRing) const;

		//friend std::ostream& operator<<(std::ostream& os, const Zone& z);
		
		bool entrable()const;

		const str_vec& getKeys()const;
		const str_vec& getNeighbors()const;
		const std::string& getZoneName()const;
		const std::string& getEntryKey()const;

		void changeEntryKey(const std::string& new_key);
		void addNeighbor(const std::string& new_neighbor);
		void addKey(const std::string& key);
		void changeName(const std::string& new_name);

		void saveToFile(std::ostream& os, std::unordered_map<std::string, Map::Zone>& game_map)const;

	private:

		//if any there are any keys in this zone they will be aded to keyRing
		void extractKeys(str_set& keyRing)const;

		void extractNeighbors(str_queue& toVisit)const;

		void SetZoneName(const std::string& name);
		void SetNeighbors(const str_vec& src);
		void SetKeys(const str_vec& keys);
		void SetEntryKey(const std::string& e_key);
		
		

	private:
		std::string zone_name;
		str_vec neighbors;
		str_vec keys;
		std::string entry_key;
		bool explored;
	};

public:
	Map();
	~Map();


	void loadFromFile(const std::string& FILENAME ="TestInput.txt");

	void saveToFile(const std::string& FILENAME = "TestOutput.gv");

	bool explore(const std::string& starting_zone, const std::string& last_zone ="NONE");

private:
	void enqueue(std::queue<Zone>& queue, const Zone& z);
private:
	std::unordered_map<std::string, Map::Zone> game_map;
	std::unordered_set<std::string> KeyRing;
	std::ofstream logFile;
	std::string test_str;

};

