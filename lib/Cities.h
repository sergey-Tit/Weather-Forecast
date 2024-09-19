#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <cpr/cpr.h>
#include <iostream>
#include <nlohmann/json.hpp>

#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/component/mouse.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ConfigParse.h"

const std::string kApiKey = "pt3uHJYQ5ujmnPmf+yRrrw==EWxIpMq27mfEFhad";
const std::string kApiNinjaUrl = "https://api.api-ninjas.com/v1/city";
const std::string kOpenMeteoUrl = "https://api.open-meteo.com/v1/forecast";

std::string GetDescriptionOfWeather(const int16_t weather_code);

std::string GetErrorText(int32_t status_code);

std::string ConvertFloatToString(float number);

ftxui::Color GetRelativeHumidityColor(int16_t value);

ftxui::Color GetWindSpeedColor(float value);

ftxui::Color GetTemperatureColor(float value);

class Day {
public:
    void SetWeatherForecast(int16_t number_of_day, nlohmann::json& weather_forecast);

    void PrintWeather();

    ftxui::Element MakeDay();

private:
    std::string date_;
    std::vector<std::string> weather_description_;
    std::vector<float> temperature_;
    std::vector<float> apparent_temperature_;
    std::vector<float> wind_speed_;
    std::vector<int16_t> relative_humidity_;
    std::vector<float> precipitation_;

    std::string MakeTemperatureDescription(int16_t index);

    std::string MakeWindSpeedDescription(int16_t index);

    std::string MakeRelativeHumidityDescription(int16_t index);

    std::string MakePrecipitationDescription(int16_t index);

    ftxui::Element MakePicture();

    ftxui::Element MakeBox(const std::string& header, int16_t index_of_part);
};

class City {
public:
    City(std::string name, int16_t number_of_days);

    int32_t UpdateWeatherForecast();

    void RemoveDay();

    int32_t AppendDay();

    void Print();

    ftxui::Element MakeCityWeather();

private:
    std::string name_;
    int16_t number_of_days_;
    double latitude_;
    double longitude_;
    std::string cur_weather_description_;
    float cur_temperature_;
    float cur_apparent_temperature_;
    float cur_wind_speed_;
    int16_t cur_relative_humidity_;
    float cur_precipitation_;
    std::vector<Day> weather_of_each_day_;

    ftxui::Element MakeCurrentWeather();

    std::string MakeTemperatureDescription();

    std::string MakeWindSpeedDescription();

    std::string MakeRelativeHumidityDescription();

    std::string MakePrecipitationDescription();

    void GetCoordinates();

    void UpdateCurrentWeather(nlohmann::json& current_forecast);
};

class Cities {
public:
    Cities(std::vector<std::pair<std::string, int16_t>> cities);

    int32_t UpdateWeatherForecastOfCurrentCity();

    void DecreaseDaysOfCurrentCity();

    int32_t IncreaseDaysOfCurrentCity();

    void NextCity();

    void PreviousCity();

    void PrintWeather();

    ftxui::Element WeatherInCurrentCity();

private:
    std::vector<City> list_of_cities_;
    int32_t number_of_cities_;
    int16_t current_city_ = 0;
};
