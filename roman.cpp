#include "roman.h"

std::string roman(int input) {
	std::string output = "";

	std::string numerals[3][4] = {
		{"I", "IV", "V", "IX"},
		{"X", "XL", "L", "XC"},
		{"C", "CD", "D", "CM"}
	};

	int ones = input % 10;
	int tens = (input % 100) / 10;
	int hunds = (input % 1000) / 100;
	int thous = input / 1000;

	for(int i=0; i<thous; i++) {
		output.append("M");
	}
	output.append( roman_convert(numerals[2], hunds) );
	output.append( roman_convert(numerals[1], tens) );
	output.append( roman_convert(numerals[0], ones) );

	return output;
}

std::string roman_convert(std::string num[], int input) {
	std::string output = "";

	if(input == 9) {
		output.append( num[3] );
	} else if(input > 4) {
		output.append( num[2] );
		for(int i=0; i < input-5; i++) {
			output.append( num[0] );
		}
	} else if(input == 4) {
		output.append( num[1] );
	} else {
		for(int i=0; i < input; i++) {
			output.append( num[0] );
		}
	}

	return output;
}

