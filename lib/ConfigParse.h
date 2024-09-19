#pragma once

#include <fstream>
#include <nlohmann/json.hpp>
#include <vector>
#include <string>
#include <cstdint>
#include <iostream>

const int16_t kMaxDays = 15;

void ExitWithError(std::string error_text);

void CheckCorrectnessOfConfigFile(const nlohmann::json& config_file_data);

std::pair<std::vector<std::pair<std::string, int16_t>>, int32_t> Parse(const std::string& config_file_path);
