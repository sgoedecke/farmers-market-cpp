struct Alert {
  sf::Texture alertTexture;
  sf::Sprite alertSprite;
  sf::Text message;
  sf::Font font;
  int tickCount;
  bool visible;

  void tick() {
    tickCount++;
    if (tickCount > 30) {
      tickCount = 0;
      visible = false;
    }
  }

  void loadTexture() {
    alertTexture.loadFromFile("./assets/alert.png");
    alertSprite.setTexture(alertTexture);
    alertSprite.setScale(SCALE);
    alertSprite.setPosition(sf::Vector2f(10, 10));

    font.loadFromFile("./assets/PressStart2P.ttf");
    tickCount = 0;
  }

  void setMessage(string s) {
    visible = true;
    message.setString(s);
    message.setPosition(sf::Vector2f(50,70));
    message.setFillColor(sf::Color::Black);
    message.setFont(font);
  }

  void drawInto(sf::RenderWindow* w) {
    if (visible) {
      w->draw(alertSprite);
      w->draw(message);
    }
  }
};
