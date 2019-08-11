#include "Map.h"



Map::Map()
{
	logFile.open("Log.txt");
}


Map::~Map()
{
	logFile.close();
}

void Map::loadFromFile(const std::string & FILENAME)
{


	std::cout << "Importing from..." << FILENAME <<'\n';


	std::ifstream inputF(FILENAME);

	if (!inputF.is_open())
		throw std::runtime_error("Could not open file: " + FILENAME);

	std::string line, vertex1, vertex2, key;
	size_t j = 0;
	bool isListKeys = false;

	inputF >> line;

	if (line != "[zones]")
	{
		inputF.clear();
		inputF.close();
		throw std::logic_error("File type not supported" + FILENAME);
	}
	while (inputF.good() & !inputF.eof())
	{
		line.clear();
		vertex1.clear();
		vertex2.clear();
		key.clear();

		j = 0;

		while (inputF.peek() == '\n')
			inputF.get();

		if (inputF.peek() == '[')
		{
			std::getline(inputF, line, ']');

			if (line == "[keys")
				isListKeys = true;
			else
			{
				inputF.close();
				throw std::logic_error("Unrecognised character in input file" + FILENAME);
			}

			inputF.get();

			if (inputF.peek() == ';')
				inputF.get();

		}

		std::getline(inputF, line, ';');

		if (inputF.eof())
			break;


		if (isListKeys)
		{
			//extracting key
			while (line[j] != '-')
				key.push_back(line[j++]);
			//removing white space
			key.pop_back();
		//number of characters in -> , combined with righside white space
		j += 3;

		while (j < line.size())
			vertex1.push_back(line[j++]);

		game_map[vertex1].addKey(key);
		}
		else
		{
			while (line[j] != '-')
				vertex1.push_back(line[j++]);

			vertex1.pop_back();

			j += 3;


			while (j < line.size() && line[j] != '[')
				vertex2.push_back(line[j++]);

			if (j < line.size())
			{
				vertex2.pop_back();

				++j;

				while (line[j] != ']')
					key.push_back(line[j++]);
				game_map[vertex1].addNeighbor(vertex2);
				game_map[vertex1].changeName(vertex1);

				game_map[vertex2].changeName(vertex2);
				game_map[vertex2].changeEntryKey(key);
			}
			else
			{
				game_map[vertex1].addNeighbor(vertex2);
				game_map[vertex2].changeEntryKey(EMPTY_KEY);
				game_map[vertex1].changeName(vertex1);

				game_map[vertex2].changeName(vertex2);
			}
		}
	}


	inputF.close();
}

void Map::saveToFile(const std::string & FILENAME)
{
	
	std::ofstream outFile(FILENAME);

	std::cout << "Saving to..." << FILENAME << '\n';

	if (outFile.is_open())
	{
		outFile << "digraph{\n";

		for (const std::pair<std::string, Zone> p : game_map)
			p.second.saveToFile(outFile, game_map);
		
		outFile << '}';
	}
	
	
	

	outFile.close();

}

bool Map::explore(const std::string& starting_zone, const std::string& last_zone)
{
	
	printf("Exploring...%s \n", starting_zone.c_str());

	logFile <<"Entered: " << starting_zone << " from: " << last_zone <<'\n';

	std::string tmp_str;

	Zone tmp_zone = game_map[starting_zone];

	std::queue<Zone> lockedZones, unlockedZones;

	//will determinate if we can go back to last_zone
	bool seen_last_zone = false;

	//adding keys to key ting
	game_map[starting_zone].visit(KeyRing);

	for (const std::string& neigh : tmp_zone.getNeighbors())
	{
		if (!seen_last_zone && neigh == last_zone)
			seen_last_zone = true;
		else
		{
			if (game_map[neigh].entrable())
			{
				unlockedZones.push(game_map[neigh]);
			}
			else
				lockedZones.push(game_map[neigh]);
		}
	}

	size_t size_unlocked = unlockedZones.size();
	
	//exploring unlocked zones
	for (size_t i = 0; i < size_unlocked; ++i)
	{
		//if we can return form first unlocked zone
		if (explore(unlockedZones.front().getZoneName(),starting_zone))
			unlockedZones.pop();
		else//we exit the fucntion
			return false;
	}

	size_t size_locked = lockedZones.size();

	//exploring locked zones;
	for (size_t i = 0; i < size_locked; ++i)
	{
		std::unordered_set<std::string>::iterator it = KeyRing.find(lockedZones.front().getEntryKey());
		
		if (it != KeyRing.end())
		{
			//geting the name of the first locked zone in the queue
			tmp_str = lockedZones.front().getZoneName();

			//check if this is the zone we came from
			if (!seen_last_zone && tmp_str == last_zone)
				seen_last_zone = true;

			//if we can come back from tmp_zone, we can continue exploring
			if (explore(tmp_str,starting_zone))
				lockedZones.pop();
			else//we exit the function
				return false;
		}
		else//cycles the locked zones
		{
			lockedZones.push(lockedZones.front());
			lockedZones.pop();

		}


	}

	return seen_last_zone;
}

void Map::enqueue(std::queue<Zone>& queue, const Zone& z)
{

	for (const std::string& neigh : z.getNeighbors())
	{
		queue.push(game_map[neigh]);
	}

}





Map::Zone::Zone():explored(false)
{
}

Map::Zone::Zone(const std::string & z_name, const str_vec & neighbors, const str_vec & z_keys, const std::string & entry_key) :explored(false)
{
	SetZoneName(z_name);
	SetNeighbors(neighbors);
	SetKeys(z_keys);
	SetEntryKey(entry_key);
}


Map::Zone::~Zone()
{
	
}

void Map::Zone::visit(str_set & KeyRing)
{
	makeExplored();
	//adding all the keys in this zone to the KeyRing
	for (const std::string& key : keys)
		KeyRing.insert(key);
}

void Map::Zone::makeExplored()
{
	explored = true;
}

bool Map::Zone::canEnter(const str_set & keyRing) const
{
	return (entry_key == EMPTY_KEY || keyRing.find(entry_key) != keyRing.end());
}

bool Map::Zone::entrable() const
{
	return entry_key == EMPTY_KEY;
}

const str_vec & Map::Zone::getKeys()const
{
	return keys;
}

const str_vec & Map::Zone::getNeighbors()const
{
	return neighbors;
}

const std::string & Map::Zone::getZoneName() const
{
	return zone_name;
}

const std::string & Map::Zone::getEntryKey()const
{
	return entry_key;
}

void Map::Zone::changeEntryKey(const std::string & new_key)
{
	SetEntryKey(new_key);
}

void Map::Zone::addNeighbor(const std::string & new_neighbor)
{
	if (new_neighbor.empty())
		throw std::invalid_argument("Name for neighbor is empty\n");

	neighbors.push_back(new_neighbor);
}

void Map::Zone::addKey(const std::string & key)
{
	if (key.empty())
		throw std::invalid_argument("key is empty");

	keys.push_back(key);
}

void Map::Zone::changeName(const std::string & new_name)
{

	SetZoneName(new_name);
}

void Map::Zone::extractKeys(str_set & keyRing) const
{
	for (const std::string& k : keys)
		keyRing.insert(k);
}

void Map::Zone::extractNeighbors(str_queue & toVisit) const
{
	for (const std::string& neigh : neighbors)
		toVisit.push(neigh);

}

void Map::Zone::SetZoneName(const std::string & name)
{
	if (name.empty())
		throw std::invalid_argument("Zone name is empthy\n");

	zone_name = name;
}

void Map::Zone::SetNeighbors(const str_vec & src)
{
	neighbors = src;

}

void Map::Zone::SetKeys(const str_vec & keys)
{
	this->keys = keys;

}

void Map::Zone::SetEntryKey(const std::string & e_key)
{
	entry_key = e_key;
}

void Map::Zone::saveToFile(std::ostream & os, std::unordered_map<std::string, Map::Zone>& game_map) const
{
	std::string tmp_key = EMPTY_KEY;


	if (os.good())
	{
		//Saving zone name and colouring it
		{
			os << QT << zone_name << QT;

			if (!explored)
				os << COL;

			os << ";\n";
		}

		//All edgeds and req keys
		if (!neighbors.empty())
		{
			for (const std::string name_neigh : neighbors)
			{
				os << QT << zone_name << QT << SEP << QT << name_neigh << QT;

				if (game_map[name_neigh].entry_key != EMPTY_KEY)
				{
					os << "[label=" << QT << game_map[name_neigh].entry_key << QT << ']';
				}

				os << ";\n";

			}



		}





		//All keys in the current zone
		if (!keys.empty())
		{
			std::string formated_keys = "\\l";

			for (const std::string key : keys)
			{
				formated_keys += key;
				formated_keys += "\\l";

			}

			os << QT << zone_name <<QT<< "[label=\"\\l"
				<< zone_name << formated_keys
				<< "\"];\n";
		}
	}



	os.clear();
}


