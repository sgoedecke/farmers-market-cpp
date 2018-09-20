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

#include "SelectedTile.cpp"
#include "Player.cpp"
#include "World.cpp"

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
          player.handleKeys();
          switch(event.key.code) {
            case sf::Keyboard::E:
              selectedTile.reset(player.gridX, player.gridY, player.dir);
              break;
            case sf::Keyboard::Escape :
              window.close();
              break;
            default:
              break;
          }
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

