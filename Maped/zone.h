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
//typedef std::unordered_map<std::string, Zone* > map_zones;

#define SEP " -> ";
#define COL "[color=red, style=filled, fillcolor=\"#ffefef\"];\n"
#define EMPTY_KEY "NULL"

class Zone
{
public:
	Zone();
	Zone(const std::string& z_name, const str_vec& neighbors, const str_vec& z_keys,  map_zones& map ,const std::string& entry_key = EMPTY_KEY);
	~Zone();

	

	void visit(str_set& KeyRing, str_queue& toVisit);

	//keyRing holds all the keys the "player" has collected thus far
	bool canEnter(const str_set& keyRing) const;
	
	friend std::ostream& operator<<(std::ostream& os, const Zone& z);

private:
	
	//if any there are any keys in this zone they will be aded to keyRing
	void extractKeys(str_set& keyRing)const;
	void extractNeighbors(str_queue& toVisit)const;
	
	void SetZoneName(const std::string& name);
	void SetNeighbors(const str_vec& src);
	void SetKeys(const str_vec& keys);
	void SetEntryKey(const std::string& e_key);
	void SetMap( map_zones& map);
	//void saveToFile(std::ostream& os);

private:
	std::string zone_name;
	str_vec neighbors;
	map_zones* p_map;
	str_vec keys;
	std::string entry_key;
	bool explored;
};


//std::ostream& operator<<(std::ostream& os, const Zone& z);
