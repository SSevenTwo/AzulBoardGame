#ifndef GAMEENGINECALLBACK_H
#define GAMEENGINECALLBACK_H

#include <iostream>
#include <string>
#include "../Player/headers/Player.h"
#include "../Components/headers/Factory.h"

class GameEngineCallback{
public:
    GameEngineCallback();
    ~GameEngineCallback();

    // Prints board of a player
    void playerBoardUpdate(std::vector<std::shared_ptr<Player>> players, int noOfPlayers);

    // Prints whose turn is it now
    void playerTurnUpdate(std::string playerName);

    // Prints the results of the turn (eg. turn success , error)
    void playerTurnResult(std::string turnResult);

    // Prints results of the end of round (board + points)
    void playerEndOfRoundResult(std::vector<std::shared_ptr<Player>> players, int noOfPlayers);

    // Prints results of the end of game (board + points + winner)
    void playerEndOfGameResult(std::vector<std::shared_ptr<Player>> players, int noOfPlayers);

    // Prints board components (factories)
    void boardComponentUpdate(std::vector<std::shared_ptr<Factory>> factories, bool use2ndFactory);

private:


};

#endif // GAMEENGINECALLBACK_H