#include "Cities.h"

std::string GetDescriptionOfWeather(const int16_t weather_code) {
    switch (weather_code) {
        case 0:
            return "Clear sky";
        case 1:
            return "Mainly clear";
        case 2:
            return "Partly cloudy";
        case 3:
            return "Overcast";
        case 45:
            return "Fog";
        case 48:
            return "Depositing rime fog";
        case 51:
            return "Light drizzle";
        case 53:
            return "Moderate drizzle";
        case 55:
            return "Dense intensity drizzle";
        case 56:
            return "Light freezing drizzle";
        case 57:
            return "Dense intensity freezing drizzle";
        case 61:
            return "Slight rain";
        case 63:
            return "Moderate rain";
        case 65:
            return "Heavy rain";
        case 66:
            return "Light freezing rain";
        case 67:
            return "Heavy intensity freezing rain";
        case 71:
            return "Slight snow fall";
        case 73:
            return "Moderate snow fall";
        case 75:
            return "Heavy intensity snow fall";
        case 77:
            return "Snow grains";
        case 80:
            return "Slight rain shower";
        case 81:
            return "Moderate rain shower";
        case 82:
            return "Violent rain shower";
        case 85:
            return "Slight snow shower";
        case 86:
            return "Heavy snow shower";
        case 95:
            return "Thunderstorm";
        case 96:
            return "Thunderstorm with slight hail";
        case 99:
            return "Thunderstorm with heavy hail";
        default:
            return "";
    }
}

std::string GetErrorText(int32_t status_code) {
    if (status_code == 0) {
        return "\n\nNo internet connection.\n";
    }
    if (status_code >= 400 && status_code < 500) {
        return "\n\nClient error.\n";
    }
    if (status_code >= 500) {
        return "\n\nServer error.\n";
    }
    return "\n\nSomething went wrong.\n";
}

std::string ConvertFloatToString(float number) {
    std::string converted_number = std::to_string(number);
    int16_t index = converted_number.find('.');
    converted_number = std::string_view(converted_number.begin(), converted_number.begin() + index + 2);
    return converted_number;
}

ftxui::Color GetRelativeHumidityColor(int16_t value) {
    using namespace ftxui;
    if (value >= 75) {
        return Color::DeepSkyBlue1;
    }
    if (value >= 50 && value < 75) {
        return Color::SkyBlue1;
    }
    if (value >= 25 && value < 50) {
        return Color::LightSkyBlue1;
    }
    return Color::White;
}

ftxui::Color GetWindSpeedColor(float value) {
    using namespace ftxui;
    if (value >= 20) {
        return Color::DarkRed;
    }
    if (value >= 10 && value < 20) {
        return Color::Red;
    }
    return Color::RedLight;
}

ftxui::Color GetTemperatureColor(float value) {
    using namespace ftxui;
    if (value >= 20) {
        return Color::Orange1;
    }
    if (value >= 10) {
        return Color::Yellow;
    }
    if (value >= 0) {
        return Color::LightYellow3;
    }
    if (value > -10) {
        return Color::LightSkyBlue1;
    }
    if (value > -20) {
        return Color::SkyBlue1;
    }
    return Color::DeepSkyBlue1;
}

void Day::SetWeatherForecast(int16_t number_of_day, nlohmann::json& weather_forecast) {
    std::string date_with_time = weather_forecast["hourly"]["time"][24 * number_of_day];
    date_ = std::string_view(date_with_time.begin(), date_with_time.begin() + 10);
    for (int16_t i = 0; i < 24; i += 6) {
        weather_description_.push_back(GetDescriptionOfWeather(weather_forecast["hourly"]["weather_code"][24 * number_of_day + i]));
        temperature_.push_back(weather_forecast["hourly"]["temperature_2m"][24 * number_of_day + i]);
        apparent_temperature_.push_back(weather_forecast["hourly"]["apparent_temperature"][24 * number_of_day + i]);
        wind_speed_.push_back(weather_forecast["hourly"]["wind_speed_10m"][24 * number_of_day + i]);
        relative_humidity_.push_back(weather_forecast["hourly"]["relative_humidity_2m"][24 * number_of_day + i]);
        precipitation_.push_back(weather_forecast["hourly"]["precipitation"][24 * number_of_day + i]);
    }
}

ftxui::Element Day::MakeDay() {
    using namespace ftxui;
    auto content = hbox(MakeBox("Night", 0), MakeBox("Morning", 1), MakeBox("Afternoon", 2), MakeBox("Evening", 3)) | yflex_grow;

    return window(text(date_) | hcenter, content | yflex_grow) | yflex_grow;
}

std::string Day::MakeTemperatureDescription(int16_t index) {
    std::string temperature = ConvertFloatToString(temperature_[index]);
    std::string apparent_temperature = ConvertFloatToString(apparent_temperature_[index]);
    std::string result = "";
    if (temperature[0] != '-') {
        result.push_back('+');
    }
    result += temperature + " (";
    if (apparent_temperature[0] != '-') {
        result.push_back('+');
    }
    result += apparent_temperature + ") °C";

    return result;
}

std::string Day::MakeWindSpeedDescription(int16_t index) {
    return ConvertFloatToString(wind_speed_[index]) + " km/h";
}

std::string Day::MakeRelativeHumidityDescription(int16_t index) {
    return std::to_string(relative_humidity_[index]) + "%";
}

std::string Day::MakePrecipitationDescription(int16_t index) {
    return ConvertFloatToString(precipitation_[index]) + " mm";
}

ftxui::Element Day::MakeBox(const std::string &header, int16_t part_of_day) {
    using namespace ftxui;
    auto content = vbox(text(weather_description_[part_of_day]) | color(Color::LightGreen), text(MakeTemperatureDescription(part_of_day)) | color(
                                GetTemperatureColor(apparent_temperature_[part_of_day])),
                        text(MakeWindSpeedDescription(part_of_day)) | color(GetWindSpeedColor(wind_speed_[part_of_day])),
                        text(MakeRelativeHumidityDescription(part_of_day)) | color(
                    GetRelativeHumidityColor(relative_humidity_[part_of_day])),
                        text(MakePrecipitationDescription(part_of_day)));

    return window(text(header) | hcenter, content) | xflex_grow;
}

City::City(std::string name, int16_t number_of_days) : name_(name), number_of_days_(number_of_days) {
    weather_of_each_day_.resize(number_of_days);
    GetCoordinates();
    int32_t status_code = UpdateWeatherForecast();
    if (status_code != 200) {
        ExitWithError(GetErrorText(status_code));
    }
}

int32_t City::UpdateWeatherForecast() {
    cpr::Response daily_forecast = cpr::Get(cpr::Url{kOpenMeteoUrl},
                          cpr::Parameters{{"latitude",std::to_string(latitude_)},
                                            {"longitude",std::to_string(longitude_)},
                                            {"hourly", "weather_code,temperature_2m,apparent_temperature,wind_speed_10m,relative_humidity_2m,precipitation"},
                                            {"forecast_days", std::to_string(number_of_days_)},
                                            {"timezone", "auto"}});
    cpr::Response current_forecast = cpr::Get(cpr::Url{kOpenMeteoUrl},
                            cpr::Parameters{{"latitude",std::to_string(latitude_)},
                                            {"longitude",std::to_string(longitude_)},
                                            {"current", "weather_code,temperature_2m,apparent_temperature,wind_speed_10m,relative_humidity_2m,precipitation"},
                                            {"forecast_days", std::to_string(number_of_days_)},
                                            {"timezone", "auto"}});

    if (daily_forecast.status_code == 200 && current_forecast.status_code == 200) {
        nlohmann::json received_daily_forecast_data = nlohmann::json::parse(daily_forecast.text);
        nlohmann::json received_current_forecast_data = nlohmann::json::parse(current_forecast.text);
        UpdateCurrentWeather(received_current_forecast_data);
        for (int16_t i = 0; i < number_of_days_; ++i) {
            weather_of_each_day_[i].SetWeatherForecast(i, received_daily_forecast_data);
        }
    }

    return daily_forecast.status_code;
}

void City::RemoveDay() {
    if (number_of_days_ == 1) {
        return;
    }
    --number_of_days_;
    weather_of_each_day_.pop_back();
}

int32_t City::AppendDay() {
    if (number_of_days_ == kMaxDays) {
        return 200;
    }
    ++number_of_days_;
    weather_of_each_day_.push_back(Day());
    return UpdateWeatherForecast();
}

ftxui::Element City::MakeCityWeather() {
    using namespace ftxui;
    std::vector<ftxui::Element> list_of_weather;
    for (int16_t i = 0; i < number_of_days_; ++i) {
        list_of_weather.push_back(weather_of_each_day_[i].MakeDay());
    }
    auto content = vbox(MakeCurrentWeather(), list_of_weather) | yflex_grow;

    return window(text(name_) | hcenter, content | yflex_grow) | yflex_grow;
}

ftxui::Element City::MakeCurrentWeather() {
    using namespace ftxui;
    auto content = vbox(text(cur_weather_description_) | color(Color::LightGreen), text(MakeTemperatureDescription()) | color(
                                GetTemperatureColor(cur_apparent_temperature_)),
                        text(MakeWindSpeedDescription()) | color(GetWindSpeedColor(cur_wind_speed_)), text(MakeRelativeHumidityDescription())
                        | color(GetRelativeHumidityColor(cur_relative_humidity_)),
                        text(MakePrecipitationDescription()));

    return window(text("Current") | hcenter, content);
}

std::string City::MakeTemperatureDescription() {
    std::string temperature = ConvertFloatToString(cur_temperature_);
    std::string apparent_temperature = ConvertFloatToString(cur_apparent_temperature_);
    std::string result = "";
    if (temperature[0] != '-') {
        result.push_back('+');
    }
    result += temperature + " (";
    if (apparent_temperature[0] != '-') {
        result.push_back('+');
    }
    result += apparent_temperature + ") °C";

    return result;
}

std::string City::MakeWindSpeedDescription() {
    return ConvertFloatToString(cur_wind_speed_) + " km/h";
}

std::string City::MakeRelativeHumidityDescription() {
    return std::to_string(cur_relative_humidity_) + "%";
}

std::string City::MakePrecipitationDescription() {
    return ConvertFloatToString(cur_precipitation_) + " mm";
}

void City::GetCoordinates() {
    cpr::Response response = cpr::Get(cpr::Url{kApiNinjaUrl},
                                      cpr::Header{{"X-Api-Key", kApiKey}},
                                      cpr::Parameters{{"name", name_}});
    if (response.status_code == 200 && response.text != "[]") {
        nlohmann::json received_data = nlohmann::json::parse(response.text);
        latitude_ = received_data[0]["latitude"];
        longitude_ = received_data[0]["longitude"];
    }
    else if (response.text == "[]") {
        ExitWithError("Incorrect city name.\n");
    }
    else {
        ExitWithError(GetErrorText(response.status_code));
    }
}

void City::UpdateCurrentWeather(nlohmann::json& current_forecast) {
    cur_weather_description_ = GetDescriptionOfWeather(current_forecast["current"]["weather_code"]);
    cur_temperature_ = current_forecast["current"]["temperature_2m"];
    cur_apparent_temperature_ = current_forecast["current"]["apparent_temperature"];
    cur_wind_speed_ = current_forecast["current"]["wind_speed_10m"];
    cur_relative_humidity_ = current_forecast["current"]["relative_humidity_2m"];
    cur_precipitation_ = current_forecast["current"]["precipitation"];
}

Cities::Cities(std::vector<std::pair<std::string, int16_t>> cities) : number_of_cities_(cities.size()) {
    for (int16_t i = 0; i < number_of_cities_; ++i) {
        list_of_cities_.push_back(City(cities[i].first, cities[i].second));
    }
}

int32_t Cities::UpdateWeatherForecastOfCurrentCity() {
    return list_of_cities_[current_city_].UpdateWeatherForecast();
}

void Cities::DecreaseDaysOfCurrentCity() {
    list_of_cities_[current_city_].RemoveDay();
}

int32_t Cities::IncreaseDaysOfCurrentCity() {
    return list_of_cities_[current_city_].AppendDay();
}

void Cities::NextCity() {
    current_city_ = (current_city_ + 1) % number_of_cities_;
}

void Cities::PreviousCity() {
    if (current_city_ == 0) {
        current_city_ = number_of_cities_ - 1;
    }
    else {
        --current_city_;
    }
}

ftxui::Element Cities::WeatherInCurrentCity() {
    return list_of_cities_[current_city_].MakeCityWeather();
}
