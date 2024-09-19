#include <lib/Cities.h>
#include <lib/ConfigParse.h>
#include <lib/Weather.h>

int main(int32_t argc, char** argv) {
    if (argc != 2) {
        ExitWithError("Incorrect path to config file.\n");
    }
    std::pair<std::vector<std::pair<std::string, int16_t>>, int32_t> parse_result = Parse(argv[1]);
    Cities cities(parse_result.first);
    Weather weather(cities, parse_result.second);
    weather.RunProgram();
}
