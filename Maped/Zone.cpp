#include "Zone.h"



Zone::Zone()
{
}

Zone::Zone(const std::string & z_name, const str_vec & neighbors, const str_vec & z_keys,map_zones& map ,const std::string & entry_key):explored(false)
{
	SetZoneName(z_name);
	SetNeighbors(neighbors);
	SetKeys(z_keys);
	SetEntryKey(entry_key);
	SetMap(map);

}


Zone::~Zone()
{
	p_map = nullptr;
}

void Zone::visit(str_set & KeyRing, str_queue & toVisit)
{
	if (canEnter(KeyRing))
	{
		explored = true;
		
		extractKeys(KeyRing);
		
		extractNeighbors(toVisit);
	}
	return;
}

bool Zone::canEnter(const str_set & keyRing) const
{
	return (entry_key == EMPTY_KEY || keyRing.find(entry_key) != keyRing.end());
}

void Zone::extractKeys(str_set & keyRing) const
{
	for (const std::string& k : keys)
		keyRing.insert(k);
}

void Zone::extractNeighbors(str_queue & toVisit) const
{
	for (const std::string& neigh : neighbors)
		toVisit.push(neigh);

}

void Zone::SetZoneName(const std::string & name)
{
	if (name.empty())
		throw std::invalid_argument("Zone name is empthy\n");
	
	zone_name = name;
}

void Zone::SetNeighbors(const str_vec & src)
{
	neighbors = src;

}

void Zone::SetKeys(const str_vec & keys)
{
	this->keys = keys;

}

void Zone::SetEntryKey(const std::string & e_key)
{
	entry_key = e_key;
}

void Zone::SetMap(map_zones & map)
{
	p_map = &map;
}

std::ostream & operator<<(std::ostream & os, const Zone & z)
{
	std::string tmp_key = EMPTY_KEY;
	
	
	if (os.good())
	{
		//Saving zone name and colouring it
		{
			os <<"\"" <<z.zone_name<<"\"";

			if (!z.explored)
				os << COL;
		
			os << ";\n";
		}

		//All edgeds and req keys
		for (const std::string& n_zone : z.neighbors)
		{
			//checks if n_zone is in the map
			map_zones::iterator it = (*z.p_map).find(n_zone);

			if (it != z.p_map->end())
			{
				os << "\"" << z.zone_name << "\"";
				os << SEP;
				os << "\"" << n_zone << "\"";

				//if the zone req a key
				if (it->second->entry_key != EMPTY_KEY)
				{
					os << "[label=" << "\"";
					os << it->second->entry_key;
					os << "\"]";
				
				}
				os << ";\n";
			}




		}

		//All keys in the current zone
		if (!z.keys.empty())
		{
			std::string formated_keys = "\l";

			for (const std::string key : z.keys)
			{
				formated_keys+=key;
				formated_keys += "\l";

			}

			os << "\"" << z.zone_name << "[label=\""
				<< z.zone_name << formated_keys
				<< "\"];\n";
		}
	}



	os.clear();

	return os;
}
