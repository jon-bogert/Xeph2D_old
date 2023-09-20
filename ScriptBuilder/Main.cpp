#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>

std::string className;
std::uint32_t id = 0;

int main()
{
	// ===== Load Settings =====
	std::string line;
	std::ifstream settingsFile("../ScriptBuilder.settings");
	while (std::getline(settingsFile, line))
	{
		std::string label;
		std::string value;
		std::stringstream stream(line);
		std::getline(stream, label, '=');
		std::getline(stream, value);
		if (label == "currID")
		{
			uint64_t val = std::stoul(value);
			if (val == UINT32_MAX)
			{
				std::cout << "Error: Maximum id value reached\n";
				std::cin.get();
				return 1;
			}
			id = static_cast<uint32_t>(val);
		}
	}
	settingsFile.close();


	// ===== User Input =====
	std::cout << "Enter Component Name: ";
	std::getline(std::cin, className);

	// ===== Create Files from Templates =====
	std::ifstream fileIn("../template/comp_h.template");
	std::ofstream fileOut("Scripts/" + className + ".h");

	std::regex pattern;
	while (std::getline(fileIn, line))
	{
		pattern = "@NAME@";
		line = std::regex_replace(line, pattern, className);
		pattern = "@ID@";
		line = std::regex_replace(line, pattern, std::to_string(id));

		fileOut << line << std::endl;
	}

	fileIn.close();
	fileOut.close();
	fileIn.open("../template/comp_cpp.template");
	fileOut.open("Scripts/" + className + ".cpp");
	while (std::getline(fileIn, line))
	{
		pattern = "@NAME@";
		line = std::regex_replace(line, pattern, className);
		pattern = "@ID@";
		line = std::regex_replace(line, pattern, std::to_string(id));

		fileOut << line << std::endl;
	}

	fileIn.close();
	fileOut.close();

	id++;

	// ===== Save Settings =====
	fileOut.open("../ScriptBuilder.settings");
	fileOut << "currID=" << id << std::endl;
	fileOut.close();

	return 0;
}