struct Inventory {
  enum Item { Nothing, Fertilizer, Seeds, Hoe, WateringCan };

  sf::Texture texture;
  sf::Sprite sprite;
  sf::Texture selectedTexture;
  sf::Sprite selectedSprite;
  Item selectedItem;

  void loadTexture() {
    texture.loadFromFile("./assets/inventory-banner.png");
    sprite.setTexture(texture);
    sprite.setScale(SCALE);
    selectedTexture.loadFromFile("./assets/selectedtile.png");
    selectedSprite.setTexture(selectedTexture);
    selectedSprite.setScale(SCALE);
    // display inventory centered, at bottom of screen
    sprite.setPosition(sf::Vector2f((WORLD_WIDTH / 2) - (36 * 5), WORLD_HEIGHT - (36 * 5)));
    setSelectedTexture();
  }

  void setSelectedTexture() {
    int pX;
    switch(selectedItem) {
      case(Nothing):
        pX = -999;
        break;
      case(Hoe):
        pX = 30;
        break;
      case(Seeds):
        pX = 10;
        break;
      case(Fertilizer):
        pX = -30;
        break;
      case(WateringCan):
        pX = -10;
        break;
      default:
        break;
    }
    selectedSprite.setPosition(sf::Vector2f((WORLD_WIDTH / 2) - (pX * 5), WORLD_HEIGHT - (30 * 5)));
  }

  void handleKeys() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
      selectedItem = Hoe;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
      selectedItem = Seeds;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
      selectedItem = WateringCan;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
      selectedItem = Fertilizer;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0)) {
      selectedItem = Nothing;
    }
    setSelectedTexture();
  }
};

