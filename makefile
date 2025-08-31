G = g++ -Wextra -std=c++14 
f = chess.cpp
l = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

build:
	$(G) $(f) -o chess $(l)

run:
	./chess && clear

clean:
	rm chess

cl:
	clear
