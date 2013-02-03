#include <iostream>

int main() {
	unsigned char r[256];

	for (int i=0;i<256;i++) {
		r[i] = i;
	}

	for (int i=0;i<256;i++) {
		int k = std::rand() % 256;
		unsigned char c = r[i];
		r[i] = r[k];
		r[k] = c;
	}

	for (int i=0;i<16;i++) {
		for (int j=0;j<16;j++) {
			std::cout << (int)r[i*16 + j] << ", ";
		}

		std::cout << std::endl;
	}
}
