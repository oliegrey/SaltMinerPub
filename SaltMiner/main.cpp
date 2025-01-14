#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/ColorConsoleAppender.h>

#include "SaltMiner.h"
#include "World.h"

int main() {
    plog::init(plog::debug, "Log.txt");
    plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::get()->addAppender(&consoleAppender);

    /*Random random{2345};
    std::array<uint8_t, 8> result{random.at(0, 0)};*/

    World dummy;
    dummy.drawTemplate();

    SaltMiner saltMiner{};
    saltMiner.start();

    return 0;
}