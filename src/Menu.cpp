struct Menu {
  sf::Sprite sprite;
  sf::Texture texture;
  sf::Sprite bgSprite;
  sf::Texture bgTexture;
  sf::Font font;
  sf::Text text;
  bool visible;

  void loadTexture() {
    visible = false;
    font.loadFromFile("./assets/PressStart2P.ttf");
    text.setFillColor(sf::Color::Black);
    text.setFont(font);
    text.setPosition(sf::Vector2f(250, 600));

    texture.loadFromFile("./assets/menu.png");
    sprite.setTexture(texture);
    sprite.setScale(SCALE); 

    bgTexture.loadFromFile("./assets/world-bg-full.png");
    bgSprite.setTexture(bgTexture);
    bgSprite.setScale(SCALE); 
    bgSprite.setColor(sf::Color(128,128,128,128));
    bgSprite.setTextureRect(sf::IntRect(0,0,WORLD_WIDTH,WORLD_HEIGHT));

    bgSprite.setPosition(sf::Vector2f(0,0));
    sprite.setPosition(sf::Vector2f(200, 200));
  }

  void displayIntroMenu() {
    text.setString("Welcome to your new farm! \n\n\nGrow some crops. Water. \nHarvest. \n\nOn Saturday, you'll take your \ncrops to market.\n\nEnter your house to\nadvance to a new day.\n\n\n\n\nPress ENTER to begin.");
    visible = true;
    gameState = OnMenu;
  }
  void displayMarketMenu() {
    text.setString("Tomorrow is market day! \n\n\nOne more day to harvest. \n\nIf your crops aren't quite\nready, use fertilizer. \n\n\n\n\nPress ENTER to continue.");
    visible = true;
    gameState = OnMenu;
  }

  bool handleKeyRelease(sf::Keyboard::Key releasedKey) {
    if (releasedKey == sf::Keyboard::A) {
      //switchFocus()
    } else if (releasedKey == sf::Keyboard::D) {
      //switchFocus()
    } else if (releasedKey == sf::Keyboard::Return) {
      visible = false;
    }
    return visible;
  }

  void drawInto(sf::RenderWindow* w) {
    if (visible) {
      w->draw(bgSprite);
      w->draw(sprite);
      w->draw(text);
    }
  }
};
