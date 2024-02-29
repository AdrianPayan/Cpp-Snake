#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include <thread>
#include <mutex>
#include <condition_variable>

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  ~Game();
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;

  void SaveScore(const std::string &playerName);
  void DisplayTopPlayers();
  void PlaceExtraFood();
  void ExtraFoodTimer();

 private:
  Snake snake;
  SDL_Point food;
  SDL_Point extra;
  SDL_Point mine;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};
  bool is_extra_active = false;
  std::thread extraFoodThread;

  void PlaceFood();
  void Update();
  void LoadScoresFromFile();
  
  struct Player
  {
    std::string name;
    int score;
  };
  std::vector<Player> playerScores;
  std::mutex mtx;
  std::mutex dataMutex;
  std::condition_variable condition_var;
  
  void UpdateThread(std::size_t target_frame_duration);
  bool running{true};
};

#endif