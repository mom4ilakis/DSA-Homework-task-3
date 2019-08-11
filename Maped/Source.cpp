#include "Map.h"


const std::string generateMap();
void fill_w_test_input(std::ofstream& os);
void fill_keys(std::ofstream& os);

int main()
{
	Map m;
	std::string inputfile;
	std::string question;

	std::cout<<"Do you want to add additional zones, YES or NO\n";
	std::cin >> question;

	inputfile = question == "YES" ? generateMap() : "DefaultInput.txt";


	m.loadFromFile(inputfile);
	m.explore("village");
	m.saveToFile();

	return 0;
}

const std::string generateMap()
{
	std::string filename;

	std::cout << "Enter filename for input file\n";
	std::cin >> filename;

	std::ofstream outFile;
	outFile.open(filename, std::ios::trunc);

	if (outFile.is_open())
	{
		std::string name_of_zone;

		std::string adj_zone("Apple_Orchard_");
		std::string tmp;

		size_t n = 0;

		
		std::cout << "Enter zone name\n";

		std::cin >> name_of_zone;

		std::cout << "Enter number of adj zones to "<<name_of_zone<<"\n";
		
		std::cin >> n;

		outFile << "[zones]\n";
		
		fill_w_test_input(outFile);

		for (int i = 0; i < n; ++i)
		{
			outFile << name_of_zone << SEP;
			tmp = adj_zone + std::to_string(i);
			outFile << tmp << ";\n";

			outFile << tmp << SEP << name_of_zone << ";\n";

		}

		fill_keys(outFile);

	}


	outFile.close();




	return filename;

}

void fill_w_test_input(std::ofstream & os)
{
	std::ifstream inFile("SavedInput_zones.txt");

	if (inFile.is_open())
	{
		std::string line;

		while (inFile.good() && !inFile.eof())
		{
			std::getline(inFile, line, ';');
			os << line;
			if (line != "\n")
				os << ";\n";
		}

	}
	
	inFile.close();



}

void fill_keys(std::ofstream & os)
{
	std::ifstream inFile("SavedInput_keys.txt");

	if (inFile.is_open())
	{
		std::string line;

		while (inFile.good() && !inFile.eof())
		{
			std::getline(inFile, line, ';');
			os << line;
			if (line != "[keys]" && line!="\n")
				os << ";\n";
		}


	}

	inFile.close();

}
