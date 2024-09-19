#include "Weather.h"

Weather::Weather(Cities cities, uint32_t frequency) : cities_(cities), frequency_(frequency) {}

void Weather::RunProgram() {
    std::cout << frequency_ << "\n\n";
    using namespace ftxui;
        auto screen = ScreenInteractive::TerminalOutput();
    auto renderer = Renderer([&] { return cities_.WeatherInCurrentCity(); });
    auto component = CatchEvent(renderer, [&](Event event) {
        if (event == Event::Character('+')) {
            int32_t status_code = cities_.IncreaseDaysOfCurrentCity();
            if (status_code != 200) {
                std::cout << GetErrorText(status_code);
                screen.Exit();
            }
            return true;
        }
        if (event == Event::Character('-')) {
            cities_.DecreaseDaysOfCurrentCity();
            return true;
        }
        if (event == Event::Character('n')) {
            cities_.NextCity();
            return true;
        }
        if (event == Event::Character('p')) {
            cities_.PreviousCity();
            return true;
        }
        if (event == Event::Escape) {
            screen.Exit();
            return true;
        }
        return false;
    });
    Loop loop(&screen, component);

    uint32_t time = 0;
    while (!loop.HasQuitted()) {
        ++time;
        if (time == frequency_ * 10) {
            cities_.UpdateWeatherForecastOfCurrentCity();
            time = 0;
        }
        loop.RunOnce();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
