#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <time.h>
using namespace std;

// constants and global variables
const int WORLD_WIDTH = 2000;
const int WORLD_HEIGHT = 1500;
const int TILE_WIDTH = 100;
const sf::Vector2f SCALE = sf::Vector2f(5.f, 5.f);
int textureTick = 0;

struct Dir { enum Type { Up, Down, Left, Right }; };
enum GameState { OnSplashScreen, OnFarm, OnMenu, OnMarket };

GameState gameState;

#include "Audio.cpp"
Audio gameAudio;

#include "Alert.cpp"
Alert gameAlert;

#include "Menu.cpp"
Menu menu;

#include "Inventory.cpp"
#include "Animations.cpp"
Animations animations;

#include "Farm.cpp"
Farm farm;

#include "SelectedTile.cpp"
#include "Player.cpp"
#include "World.cpp"
#include "SplashScreen.cpp"
#include "Market.cpp"


int main() {
  srand(time(NULL)); // seed rng

  sf::Clock textureClock;
  sf::Clock gameClock;

  // load music
  gameAudio.loadBuffers();
  gameAudio.playMusic();

  // load textures
  Player player;
  player.speed = 8;
  player.loadTexture();

  World world;
  world.loadTexture();

  SelectedTile selectedTile;
  selectedTile.loadTexture();

  farm.loadTexture();
  farm.spawnRock(5, 5);
  farm.spawnRock(6, 5);
  farm.spawnRock(7, 5);
  farm.spawnRock(6, 3);
  farm.spawnRock(8, 9);
  farm.inventoryPtr = &player.inventory;

  gameAlert.loadTexture();

  animations.loadTexture();

  SplashScreen splashScreen;
  splashScreen.loadTexture();

  Market market;
  market.loadTexture();

  menu.loadTexture();

  gameState = OnSplashScreen;
  gameState = OnMarket;

  // initialize window and begin game loop

  sf::RenderWindow window(sf::VideoMode(WORLD_WIDTH, WORLD_HEIGHT), "Grow");
  window.setFramerateLimit(60);
  while (window.isOpen()) {

    // update textures
    if (textureClock.getElapsedTime().asSeconds() > 0.1f){
      textureTick++;
      if (textureTick >= 3) {
        textureTick = 0;
      }
      selectedTile.tick();
      gameAlert.tick();
      animations.tick();
      textureClock.restart();
    }

    if (gameState == OnFarm) {
      // update world
      if (gameClock.getElapsedTime().asSeconds() > 0.05f){
        player.move();
      }
    }

    // handle events/keys
    sf::Event event;
    while (window.pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:
          window.close();
          break;
        case sf::Event::KeyPressed:
		  if (event.key.code == sf::Keyboard::Escape) {
			window.close();
		  }

          // if any key is pressed on the splash screen, start the game
          if (gameState == OnSplashScreen) {
            gameState = OnMenu;
			menu.displayIntroMenu();
          }

		  if (gameState == OnMenu) {
			bool stillOnMenu = menu.handleKeysWithReturn(); 
			if (!stillOnMenu) {
			  gameState = OnFarm;
			}
		  }

		  if (gameState == OnFarm) {
			player.handleKeys();
			if (event.key.code == sf::Keyboard::E) {
			  selectedTile.reset(player.gridX, player.gridY, player.dir);
			  farm.interactWithTile(selectedTile.x,
				  selectedTile.y,
				  player.inventory.selectedItem);
			}
		  }

          if (gameState == OnMarket) {
            // handle market controls
          }
		  break;
        default:
          break;
      }
    }

    // draw
    window.clear();
    if (gameState == OnFarm) {
      window.draw(world.sprite);
      farm.drawInto(&window);
      animations.drawInto(&window);
      player.drawInto(&window);
      gameAlert.drawInto(&window);
      if (selectedTile.isActive) {
        window.draw(selectedTile.sprite);
      }
    }

    if (gameState == OnSplashScreen) {
      splashScreen.drawInto(&window);
    }

    if (gameState == OnMenu) {
      menu.drawInto(&window);
    }

    if (gameState == OnMarket) {
      market.drawInto(&window);
    }

    window.display();
  }
  return 0;
}

