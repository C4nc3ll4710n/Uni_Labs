#include <sstream>
#include <vector>
#include <string>
#include <iostream>

struct TableRow {
    std::vector<std::string> cells;
};

//std::vector<TableRow> 
void parseTable(const char* HTTPResp) {
    const std::string response(HTTPResp);
    std::vector<TableRow> table;
    std::istringstream iss(response);
    std::string line;

    // Parse each row
    while (std::getline(iss, line)) {
        std::cout << "this is line:" << line << "\n";
    }

    //return table;
}