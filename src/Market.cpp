struct Market {
  sf::Texture texture;
  sf::Sprite sprite;
  sf::Texture selectedTableTexture;
  sf::Sprite selectedTableSprite;
  sf::Sprite table1;
  sf::Sprite table2;
  sf::Sprite table3;
  sf::Sprite table4;
  sf::Sprite table5;
  Crop crop1;
  Crop crop2;
  Crop crop3;
  Crop crop4;
  Crop crop5;
  int animTick;
  int customers;
  int maxCustomers;
  int selectedTable;
  vector<Crop> goods;
  vector<sf::Sprite> goodSprites;

  void loadTexture() {
    selectedTable = 1;
    animTick = 0;
    customers = 0;
    maxCustomers = 5;
    selectedTableTexture.loadFromFile("./assets/selectedtable.png");
    selectedTableSprite.setTexture(selectedTableTexture);
    selectedTableSprite.setScale(sf::Vector2f(15.f, 15.f));
    


    texture.loadFromFile("./assets/market-bg.png");
    sprite.setTexture(texture);
    sprite.setScale(sf::Vector2f(15.f, 15.f));
    selectTable(selectedTable);

    table1.setPosition(sf::Vector2f(285-30, 1200));

    goods.push_back(crop1);
    goods.push_back(crop2);
    goods.push_back(crop3);
    goods.push_back(crop4);
    goods.push_back(crop5);

    goodSprites.push_back(table1);
    goodSprites.push_back(table2);
    goodSprites.push_back(table3);
    goodSprites.push_back(table4);
    goodSprites.push_back(table5);
    for (int i = 0; i < goodSprites.size(); i++) {
      goodSprites[i].setTexture(cropsTexture);
      goodSprites[i].setScale(sf::Vector2f(10.f, 10.f));
      goodSprites[i].setPosition(sf::Vector2f(285*(i+1) + 30, 1280));
    }
    updateGoodsTextures();
  }

  void selectTable(int i) {
    selectedTable = i;
    selectedTableSprite.setPosition(sf::Vector2f(285*(i+1) - 30, 1200));
  }

  void updateGoodsTextures() {
    for (int i = 0; i < goods.size(); i++) {
      switch(goods[i].type) {
        case Crop::Radish:
          goodSprites[i].setTextureRect(sf::IntRect(0,0,16,16));
          break;
        case Crop::Eggplant:
          goodSprites[i].setTextureRect(sf::IntRect(0,3*16,16,16));
          break;
        case Crop::Corn:
          goodSprites[i].setTextureRect(sf::IntRect(0,9*16,16,16));
          break;
        default:
          break;
      }
    }
  }

  void forwardSelection() {
    switch (goods[selectedTable].type) {
      case Crop::Radish:
        goods[selectedTable].type = Crop::Eggplant;
        break;
      case Crop::Eggplant:
        goods[selectedTable].type = Crop::Corn;
        break;
      case Crop::Corn:
        goods[selectedTable].type = Crop::Radish;
        break;
    }
    updateGoodsTextures();
  }

  void backSelection() {
    switch (goods[selectedTable].type) {
      case Crop::Radish:
        goods[selectedTable].type = Crop::Corn;
        break;
      case Crop::Eggplant:
        goods[selectedTable].type = Crop::Radish;
        break;
      case Crop::Corn:
        goods[selectedTable].type = Crop::Eggplant;
        break;
    }
    updateGoodsTextures();
  }

  void handleKeyRelease(sf::Keyboard::Key releasedKey) {
    switch(releasedKey) {
      case sf::Keyboard::Num1:
        selectTable(0);
        break;
      case sf::Keyboard::Num2:
        selectTable(1);
        break;
      case sf::Keyboard::Num3:
        selectTable(2);
        break;
      case sf::Keyboard::Num4:
        selectTable(3);
        break;
      case sf::Keyboard::Num5:
        selectTable(4);
        break;
      case sf::Keyboard::A:
        forwardSelection();
        break;
      case sf::Keyboard::D:
        backSelection();
        break;
      default:
        break;
    }
  }

  void tick() {
    animTick++;
  }

  void drawInto(sf::RenderWindow* w) {
    w->draw(sprite);
    w->draw(selectedTableSprite);

    for (int i = 0; i < goodSprites.size(); i++) {
      w->draw(goodSprites[i]);
    }
  }
};
