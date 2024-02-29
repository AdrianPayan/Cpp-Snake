#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"

int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};
  
  std::string playerName;
  std::cout << "Enter username: ";
  std::getline(std::cin, playerName);
  /*std::thread updateThread(&Game::UpdateThread, this, target_frame_duration);*/

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;
  {
    Game game(kGridWidth, kGridHeight);
  	std::thread t1(&Game::UpdateSnake, &game);
  	std::thread t2(&Game::UpdateFood, &game);
  	std::thread t3(&Game::UpdateExtra, &game);
  
  	t1.join();
  	t2.join();
  	t3.join();
    game.Run(controller, renderer, kMsPerFrame);
    game.SaveScore(playerName);
    game.DisplayTopPlayers();

    std::cout << "Game has terminated successfully!\n";
    std::cout << "Score: " << game.GetScore() << "\n";
    std::cout << "Size: " << game.GetSize() << "\n";
  }
  return 0;
}