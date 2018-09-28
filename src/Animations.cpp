struct Animation {
  enum Type { Water, Dig, RemoveRock };

  int tickCount;
  int x;
  int y;

  Type type;
};

bool animationHasExpired(Animation a) {
  return (a.tickCount > 2);
}

struct Animations {
  sf::Texture waterAnimationTexture;
  sf::Sprite waterAnimationSprite;
  sf::Texture digAnimationTexture;
  sf::Sprite digAnimationSprite;
  sf::Texture removeRockAnimationTexture;
  sf::Sprite removeRockAnimationSprite;
  vector<Animation> animations;

  void loadTexture() {
    waterAnimationTexture.loadFromFile("./assets/water-animation.png");
    waterAnimationSprite.setTexture(waterAnimationTexture);
    waterAnimationSprite.setScale(SCALE);

    digAnimationTexture.loadFromFile("./assets/dig-animation.png");
    digAnimationSprite.setTexture(digAnimationTexture);
    digAnimationSprite.setScale(SCALE);

    removeRockAnimationTexture.loadFromFile("./assets/remove-rock-animation.png");
    removeRockAnimationSprite.setTexture(removeRockAnimationTexture);
    removeRockAnimationSprite.setScale(SCALE);
  }

  void spawnAnimation(int x, int y, Animation::Type type) {
     Animation anim;
     anim.type = type;
     anim.x = x;
     anim.y = y;
     anim.tickCount = 0;
     animations.push_back(anim);
  }

  void tick() {
    for(int i = 0; i < animations.size(); i++) {
      animations[i].tickCount++;
    }

    // remove expired animations
    animations.erase(
        std::remove_if(animations.begin(), animations.end(), animationHasExpired),
        animations.end()
        );
  }

  void drawInto(sf::RenderWindow* w) {
    for (Animation a : animations) {
      sf::Sprite* sprite;
      switch(a.type) {
        case(Animation::Type::Water):
          sprite = &waterAnimationSprite;
          break;
        case(Animation::Type::Dig):
          sprite = &digAnimationSprite;
          break;
        case(Animation::Type::RemoveRock):
          sprite = &removeRockAnimationSprite;
          break;
      }
      if (a.tickCount == 0) {
        sprite->setTextureRect(sf::IntRect(0,0,20,20));
      } else if (a.tickCount == 1) {
        sprite->setTextureRect(sf::IntRect(20,0,20,20));
      } else {
        sprite->setTextureRect(sf::IntRect(40,0,20,20));
      }

      sprite->setPosition(sf::Vector2f(a.x * TILE_WIDTH, a.y * TILE_WIDTH));
      w->draw(*sprite);
    }
  }
};
