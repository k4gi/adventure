bravo:
	g++ bravo.cpp -lncurses -o bravo
adven:
	g++ map_loader.cpp unit.cpp roman.cpp dialog.cpp adven.cpp -lncurses -o adven
mapgen:
	g++ mapgen.cpp -o mapgen
