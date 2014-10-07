#include "Game.h"
#include "InputHandler.h"

int main() 
{
    try 
    {
        GameInputHandler inputHandler;
        Game game(inputHandler);
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
