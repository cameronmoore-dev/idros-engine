#include "entry.h"
#include "engine.h"

namespace idrs
{
    int createGame(IGame *game)
    {
        Engine *engine = new Engine(game);
        engine->run();

        return 0;
    }
}