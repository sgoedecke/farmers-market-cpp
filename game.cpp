#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
using namespace std;

const int WORLD_WIDTH = 2000;
const int WORLD_HEIGHT = 1500;
const int TILE_WIDTH = 100;
const sf::Vector2f SCALE = sf::Vector2f(5.f, 5.f);
int textureTick = 0;

struct Dir { enum Type { Up, Down, Left, Right }; };

struct SelectedTile {
  int x, y;
  int totalLife;
  int tickCount;
  bool isActive;

  sf::Texture texture;
  sf::Sprite sprite;

  void loadTexture() {
    texture.loadFromFile("selectedtile.png");
    sprite.setTexture(texture);
    sprite.setScale(SCALE);
  }

  void reset(int nx, int ny, Dir::Type dir) {
    x = nx;
    y = ny;
    switch(dir) {
      case Dir::Up:
        y -= TILE_WIDTH;
        break;
      case Dir::Down:
        y += TILE_WIDTH;
        break;
      case Dir::Left:
        x -= TILE_WIDTH;
        break;
      case Dir::Right:
        x += TILE_WIDTH;
        break;
    }
    // snap to grid
    x = (x / TILE_WIDTH) * TILE_WIDTH;
    y = (y / TILE_WIDTH) * TILE_WIDTH;
    totalLife = 10;
    tickCount = 0;
    isActive = true; 
    sprite.setPosition(sf::Vector2f(x, y));
  }

  void tick() {
    tickCount++;
    if (tickCount >= totalLife) {
      isActive = false;
    }
  }
};

struct Player {
  float x, y;
  float speed;
  bool isMoving;
  Dir::Type dir;
  sf::Texture texture;
  sf::Sprite sprite;

  void loadTexture() {
    texture.loadFromFile("hero.png");
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0,0,20,40));
    sprite.setScale(SCALE);
  }

  void beginMove(Dir::Type d) {
    dir = d;
    isMoving = true;
  }

  void setTexture() {
    int tX;
    switch(dir) {
      case Dir::Up:
        tX = 3;
        break;
      case Dir::Down:
        tX = 6;
        break;
      case Dir::Left:
        tX = 9;
        break;
      case Dir::Right:
        tX = 0;
        break;
    }
    tX = tX + textureTick;
    sprite.setTextureRect(sf::IntRect((tX * 19), 0, 18, 40));
  }

  void move() {
    int nx = x;
    int ny = y;
    if (!isMoving) {
      return;
    }
    switch(dir) {
      case Dir::Up:
        ny -= speed;
        break;
      case Dir::Down:
        ny += speed;
        break;
      case Dir::Left:
        nx -= speed;
        break;
      case Dir::Right:
        nx += speed;
        break;
    }
    const bool isInBounds = (nx >= 0 && nx + TILE_WIDTH < WORLD_WIDTH && ny >= 0 && ny + (TILE_WIDTH * 2) < WORLD_HEIGHT);
    if (isInBounds) {
      setTexture();
      x = nx;
      y = ny;
      sprite.setPosition(sf::Vector2f(x, y));
    }
  }
};

struct World {
  sf::Texture texture;
  sf::Sprite sprite;

  void loadTexture() {
    texture.loadFromFile("worldbg.png");
    texture.setRepeated(true);
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0,0,WORLD_WIDTH,WORLD_HEIGHT));
    sprite.setScale(SCALE);
  }
};

int main() {
  sf::Clock textureClock;
  sf::Clock gameClock;

  // load textures
  Player player;
  player.speed = 8;
  player.loadTexture();
  World world;
  world.loadTexture();
  SelectedTile selectedTile;
  selectedTile.loadTexture();

  sf::RenderWindow window(sf::VideoMode(WORLD_WIDTH, WORLD_HEIGHT), "Grow");
  window.setFramerateLimit(60);
  while (window.isOpen()) {

    if (textureClock.getElapsedTime().asSeconds() > 0.1f){
      // update textures
      textureTick++;
      if (textureTick >= 3) {
        textureTick = 0;
      }
      selectedTile.tick();
      textureClock.restart();
    }

    if (gameClock.getElapsedTime().asSeconds() > 0.05f){
      // update world
      player.move();
    }

    sf::Event event;
    while (window.pollEvent(event)) {

      // handle events/keys
      switch (event.type) {
        case sf::Event::Closed:
          window.close();
          break;
        case sf::Event::KeyPressed:
          switch(event.key.code) {
            case sf::Keyboard::W:
              player.beginMove(Dir::Up);
              break;
            case sf::Keyboard::A:
              player.beginMove(Dir::Left);
              break;
            case sf::Keyboard::S:
              player.beginMove(Dir::Down);
              break;
            case sf::Keyboard::D:
              player.beginMove(Dir::Right);
              break;
            case sf::Keyboard::E:
              selectedTile.reset(player.x, player.y, player.dir);
              break;
            case sf::Keyboard::Escape :
              window.close();
              break;
            default:
              break;
          }
          break;
        case sf::Event::KeyReleased:
          player.isMoving = false;
          break;
        default:
          break;
      }

    }



    // draw
    window.clear();
    window.draw(world.sprite);
    window.draw(player.sprite);
    if (selectedTile.isActive) {
      window.draw(selectedTile.sprite);
    }
    window.display();
  }
  return 0;
}

