#include "Game.h"

int main() 
{
    try 
    {
        Game game;
        game.InitHelpers();
        game.InitScene0();
        game.InitScene1();
        game.Start();
    }
    catch (std::exception& ex) 
    {
        ex;
    }
}
