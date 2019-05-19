#include <fstream>
#include <iostream>
#include <string>

#define FILENAME "generated_map.dat"

int main() {

	int pad_y = 100, pad_x = 200;
	
	char buffer[pad_x];
	std::string output[pad_y];

	srand(0);
	for(int i=0;i<pad_y;i++) {
		for(int j=0;j<pad_x;j++) {
			if(i==0 || j==0 || i==pad_y-1 || j==pad_x-1) { //border
				buffer[j] = '#';
				//mvwaddch(map,i,j,'#');
			} else {
				switch(rand()%50) {
				case 0:
				//case 1:
				//case 2:
					buffer[j] = '^';
					//mvwaddch(map,i,j,'^');
					break;
				default:
					buffer[j] = '.';
					//mvwaddch(map,i,j,'.');
					break;
				}
			}
		}
		output[i] = buffer;
	}

	std::ofstream billy;

	billy.open(FILENAME, std::ios::trunc | std::ios::out);
	if(billy.is_open()) {
		for(int i=0;i<pad_y;i++) {
			billy << output[i] << std::endl;
		}
		std::cout << "Output done :)\n";
	} else std::cout << "Output failed :(\n";
	
	return 0;
}
