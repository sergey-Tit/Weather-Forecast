include(FetchContent)

FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 3020c34ae2b732121f37433e61599c34535e68a8)
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
        GIT_TAG v5.0.0
)
FetchContent_MakeAvailable(ftxui)

add_library(weather
        Cities.cpp
        ConfigParse.cpp
        Weather.cpp
)

target_link_libraries(weather
        PUBLIC cpr::cpr
        PUBLIC nlohmann_json::nlohmann_json
        PUBLIC ftxui::screen
        PUBLIC ftxui::dom
        PUBLIC ftxui::component)