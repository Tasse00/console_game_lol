#include "./src/g.h"
#include "./src/games/g2048.h"
#include "./src/games/gmines.h"

int main()
{

    auto runner = g::Runner();

    runner.registerGame(std::string("2048"),
                        std::string("2048"),
                        []() -> g::Game * { return g2048::Game::interactive_build(); });

    runner.registerGame(std::string("MineSweeper"),
                        std::string("mines"),
                        []() -> g::Game * { return gmines::Game::buildGame(); });

    runner.menu();

    return 0;
}
