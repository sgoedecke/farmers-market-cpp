game: ./src/game.cpp
	clang++ ./src/game.cpp -o ./bin/game -lsfml-system -lsfml-graphics -lsfml-window -lsfml-audio --std=c++11
