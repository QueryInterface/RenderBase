#include "Game.h"
#include "InputHandler.h"

int main() 
{
    try 
    {
        GameInputHandler inputHandler;
        inputHandler.SetMoveSpeed(5.0);
        inputHandler.SetRotateSpeed(5.0);
        Game game(inputHandler);
        game.InitHelpers();
        game.InitScene0();
        //game.InitScene1();
        game.Start();
    }
    catch (std::exception& ex) 
    {
        ex;
    }
}
