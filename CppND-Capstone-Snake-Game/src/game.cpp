#include "game.h"
#include <iostream>
#include "SDL.h"
#include <fstream>
#include <iomanip>
#include <algorithm>

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1))
{
  LoadScoresFromFile();
  PlaceFood();
}
Game::~Game(){
  running = false;
}
void Game::Run(Controller const &controller, Renderer &renderer, std::size_t target_frame_duration) {
  
  std::thread updateThread(&Game::UpdateThread, this, target_frame_duration);
  
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;

  while (running) {
    frame_start = SDL_GetTicks();
    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    
    if (!running || !snake.alive)
    {
      break;
    }
    std::lock_guard<std::mutex> lock(dataMutex);
    renderer.Render(snake, food, extra, mine);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
  updateThread.join();
  extraFoodThread.join();
}

void Game::PlaceFood() {
  int x, y;
  int xd, yd;
  int xm, ym;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    
    xd = random_w(engine);
   	yd = random_h(engine);

    xm = random_w(engine);
    ym = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if ((!snake.SnakeCell(x, y)) && (!snake.SnakeCell(xd, yd)) &&(!snake.SnakeCell(xm,ym))) {
    	if (!is_extra_active) {
          	food.x = x;
      		food.y = y;
          	
        	extra.x = -1;
        	extra.y = -1;
          
          	mine.x=xm;
      		mine.y=ym;
          
          return;
   	  }else{
      
      	food.x = x;
      	food.y = y;
      
      	extra.x=xd;
      	extra.y=yd;
    
      	mine.x=xm;
      	mine.y=ym;

      return;
      }
    }
  }
}

void Game::Update() {
  if (!snake.alive) return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    if (!is_extra_active)
    { // Check if bonus food is already active
      std::lock_guard<std::mutex> lock(dataMutex);
      is_extra_active = true;
      extraFoodThread = std::thread(&Game::ExtraFoodTimer, this);
    }
    
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }
  //Adquire 2 score points and get double the speed gained with normal food, but growing the same size as the normal food
  else if(extra.x != -1 && extra.y != -1 && extra.x == new_x && extra.y == new_y){
    score = score + 2;
    PlaceFood();
    if (!is_extra_active)
    { // Check if bonus food is already active
      std::lock_guard<std::mutex> lock(dataMutex);
      is_extra_active = true;
      extraFoodThread = std::thread(&Game::ExtraFoodTimer, this);
    }
    
    snake.GrowBody();
    snake.speed += 0.04;
  }
  else if(mine.x == new_x && mine.y == new_y){
    running = false;
  }
}
void Game::ExtraFoodTimer()
{
  const int extraSeconds = 5;
  auto startTime = std::chrono::high_resolution_clock::now();
  std::unique_lock<std::mutex> lock(dataMutex);
  while (is_extra_active)
  {
    auto current_Time = std::chrono::high_resolution_clock::now();
    auto elapsed_Seconds = std::chrono::duration_cast<std::chrono::seconds>(current_Time - startTime).count();
    if (elapsed_Seconds >= extraSeconds && is_extra_active)
    {
      // Bonus food time is up
      is_extra_active = false;
      extra.x = -1;
      extra.y = -1;
      break;
    }
    // Wait for a short interval or until the condition_variable is notified
    condition_var.wait_for(lock, std::chrono::milliseconds(800));
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }

void Game::UpdateThread(std::size_t target_frame_duration)
{
  while (running)
  {
    std::lock_guard<std::mutex> lock(mtx);
    Update();
    std::this_thread::sleep_for(std::chrono::milliseconds(target_frame_duration));
  }
}
void Game::SaveScore(const std::string &playerName)
{
  Player player;
  player.name = playerName;
  player.score = score;
  playerScores.push_back(player);

  std::sort(playerScores.begin(), playerScores.end(),
            [](const Player &a, const Player &b)
            { return a.score > b.score; });

  std::ofstream file("highscore.txt");
  if (file.is_open())
  {
    for (const auto &p : playerScores)
    {
      file << p.name << " " << p.score << "\n";
    }
    file.close();
  }
}

void Game::DisplayTopPlayers()
{
  std::cout << "\nTop 5 Highscore:\n";
  for (int i = 0; i < std::min(5, static_cast<int>(playerScores.size())); ++i)
  {
    std::cout << std::setw(2) << i + 1 << ". " << std::setw(15) << playerScores[i].name
              << "   Score: " << playerScores[i].score << "\n";
  }
}

void Game::LoadScoresFromFile() {
  std::ifstream file("highscore.txt");
  if (file.is_open()) {
    Player player;
    while (file >> player.name >> player.score) {
      playerScores.push_back(player);
    }
    file.close();
  }
}