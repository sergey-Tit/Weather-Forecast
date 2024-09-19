#include "ConfigParse.h"

void ExitWithError(std::string error_text) {
    std::cerr << error_text;
    exit(EXIT_FAILURE);
}

void CheckCorrectnessOfConfigFile(const nlohmann::json& config_file_data) {
    if (config_file_data.size() != 2) {
        ExitWithError("Incorrect config file.");
    }
    if (config_file_data.find("cities") == config_file_data.end() || config_file_data.find("frequency") == config_file_data.end()) {
        ExitWithError("Incorrect config file.");
    }
    if (!config_file_data["frequency"].is_number() || config_file_data["frequency"] == 0) {
        ExitWithError("Incorrect config file.");
    }
    nlohmann::json cities_data = config_file_data["cities"];
    if (!cities_data.is_array()) {
        ExitWithError("Incorrect config file.");
    }
    for (int16_t i = 0; i < cities_data.size(); ++i) {
        if (cities_data[i].size() != 2) {
            ExitWithError("Incorrect config file.");
        }
        if (cities_data[i].find("name") == cities_data[i].end() || cities_data[i].find("days") == cities_data[i].end()) {
            ExitWithError("Incorrect config file.");
        }
        if (!cities_data[i]["name"].is_string()) {
            ExitWithError("Incorrect config file.");
        }
        if (!cities_data[i]["days"].is_number() || cities_data[i]["days"] == 0) {
            ExitWithError("Incorrect config file.");
        }
    }
}

std::pair<std::vector<std::pair<std::string, int16_t>>, int32_t> Parse(const std::string& config_file_path) {
    std::pair<std::vector<std::pair<std::string, int16_t>>, int32_t> result;
    std::vector<std::pair<std::string, int16_t>> list_of_cities;
    std::ifstream config_file(config_file_path);
    if (!config_file.is_open()) {
        ExitWithError("Failed to open file.\n");
    }
    if (std::string_view(config_file_path.end() - 5, config_file_path.end()) != ".json") {
        ExitWithError("Must be .json config file.\n");
    }
    nlohmann::json config_file_data = nlohmann::json::parse(config_file);
    CheckCorrectnessOfConfigFile(config_file_data);
    result.second = config_file_data["frequency"];
    for (int16_t i = 0; i < config_file_data["cities"].size(); ++i) {
        std::pair<std::string, int16_t> city;
        city.first = config_file_data["cities"][i]["name"];
        city.second = std::min(static_cast<int16_t>(config_file_data["cities"][i]["days"]), kMaxDays);
        list_of_cities.push_back(city);
    }
    result.first = list_of_cities;

    return result;
}
