G =  g++ -Wextra -std=c++14
f = Main.cpp Game.cpp Board.cpp TextureManager.cpp
l = -lSDL2 -lSDL2_image -lSDL2_ttf

build:
	$(G) $(f) -o chess $(l)

run:
	./chess

clean:
	rm chess

cl:
	clear

