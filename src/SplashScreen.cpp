struct SplashScreen {
  sf::Texture texture;
  sf::Sprite sprite;
  sf::Text header;
  sf::Text message;
  sf::Font font;

  void loadTexture() {
    texture.loadFromFile("./assets/country-platform-back.png");
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0,0,WORLD_WIDTH,WORLD_HEIGHT));
    sprite.setScale(sf::Vector2f(5.f, 5.f));

    font.loadFromFile("./assets/PressStart2P.ttf");

    header.setString("GROW");
    header.setStyle(sf::Text::Bold);
    header.setPosition((WORLD_WIDTH / 2) - 500, (WORLD_HEIGHT / 2) - 200);
    header.setFillColor(sf::Color::Black);
    header.setFont(font);
    header.setScale(sf::Vector2f(5.f, 5.f));

    message.setString("Press any key to start...");
    message.setPosition(WORLD_WIDTH / 2, (WORLD_HEIGHT / 2) + 200);
    message.setFillColor(sf::Color::Black);
    message.setFont(font);
  }

  void drawInto(sf::RenderWindow* w) {
    w->draw(sprite);
    w->draw(header);
    w->draw(message);
  }
};
