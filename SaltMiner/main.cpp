#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/ColorConsoleAppender.h>

#include <turbojpeg.h>

#include "SaltMiner.h"

int main() {
    plog::init(plog::debug, "Log.txt");
    plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::get()->addAppender(&consoleAppender);
    
    SaltMiner saltMiner{};
    saltMiner.start();

    return 0;
}