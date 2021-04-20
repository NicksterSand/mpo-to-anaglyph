#include <iostream>
#include <fstream>
#include <sstream>

#define STBI_ONLY_JPEG
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/stb_image_write.h"

int main(int argc, char *argv[]){
	if(argc < 2){
		std::cout << "Usage: " << argv[0] << " FILEPATH" << std::endl;
		return 1;
	}
	for(int i = 1; i < argc; i++){
		std::cout << "Opening " << argv[i] << std::endl;
		std::ifstream infile(argv[i], std::ios::binary);
		if(!infile.is_open()){
			std::cout << "Could not open file: " << argv[i] << std::endl;
			return 1;
		}	
		std::stringstream ss;
		ss << infile.rdbuf();
		std::string fileString = ss.str();

		char stringFind[4] = {(char)-1,(char)-40,(char)-1,(char)-31};
		std::size_t found = fileString.find(stringFind, 4);
		if(found != std::string::npos){
			std::cout << "found" << std::endl;
		}else{
			std::cout << "Not found" << std::endl;
			return 1;
		}
		std::string jpg1 = fileString.substr(0,found);
		std::string jpg2 = fileString.substr(found);
		std::ofstream outfile1("temp1.jpg");
		std::ofstream outfile2("temp2.jpg");
		outfile1 << jpg1;
		outfile2 << jpg2;
		infile.close();
		outfile1.close();
		outfile2.close();

		int x1, y1, n1, x2, y2, n2;
		unsigned char* img1 = stbi_load("temp1.jpg", &x1, &y1, &n1, 0);
		unsigned char* img2 = stbi_load("temp2.jpg", &x2, &y2, &n2, 0);
		
		size_t imgsize = x1 * y1 * n1;
		unsigned char* anaglyph = (unsigned char*)malloc(imgsize);

		for(unsigned char* p1 = img1, *p2 = img2, *a = anaglyph; p1 != img1 + imgsize; p1 += n1, p2 += n2, a += n1){
			*a = (uint8_t)(*p1);
			*(a+1) = (uint8_t)(*(p2 + 1));
			*(a+2) = (uint8_t)(*(p2 + 2));
		}

		stbi_write_jpg("Anaglyph.jpg", x1, y1, n1, anaglyph, 100);	
		
		stbi_image_free(img1);
		stbi_image_free(img2);

		std::remove("temp1.jpg");
		std::remove("temp2.jpg");
	}
	return 0;
}
