//
// Created by JJ on 27/10/2026.
//

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "text_input.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "stb_image_write.h" //credit goes to the authors @ https://github.com/nothings/stb/blob/master/stb_image_write.h

text_input::text_input() {}

void text_input::showOptions() {
    std::cout<<"-----*Text to Picture*-----"<<std::endl;
    std::cout<<"Choose an option by entering 1 or 2 and hit enter."<<std::endl;
    std::cout<<"1. classic encrypt a textfile"<<std::endl;
    std::cout<<"2. symmetric encrypt a textfile"<<std::endl;
}

void text_input::chooseOption () {
    int userInput = 0;
    std::string inputFilename;
    std::string outputFilename;
    int width = 200;

    while (userInput < 1 || userInput > 2) {
        std::cin >> userInput;

        switch (userInput) {
            case 1:
                std::cout << "Please enter the filename you would like to encrypt"<<std::endl;
                std::cout << "Has to be an absolute path! Use double dashes for single dashes"<<std::endl;
                std::cout << "e.g.: C:\\\\Users\\\\JJ\\\\Desktop\\\\test.txt"<<std::endl;
                std::cin >> inputFilename;
                std::cout << "Please enter the output filename you would like to encrypt"<<std::endl;
                std::cout << "Has to be an absolute path! Use double dashes for single dashes"<<std::endl;
                std::cout << "e.g.: C:\\\\Users\\\\JJ\\\\Desktop\\\\output.png"<<std::endl;
                std::cin >> outputFilename;

                //add input for width

                convertFileToString(inputFilename, outputFilename, width);

                break;

            case 2:
                //add symmetric
                break;

            default:
                std::cout<<"Wrong Input. Try Again"<<std::endl << std::endl;
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                userInput = 0;
        }
    }
}

bool text_input::convertFileToString(const std::string& inputFilename, const std::string& outputFilename, int width) {
    std::ifstream file(inputFilename, std::ios::binary);

    if (!file.is_open()) {
        return false;
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    if (content.empty()) {
        return false;
    }

    return convertStringToPicture(content, outputFilename, width);
}

bool text_input::convertStringToPicture(const std::string& text, const std::string& outputFilename, int width) {
    if (text.empty() || width <= 0) {
        return false;
    }

    int dataSize = text.size();
    int height = (dataSize + width - 1) / width;

    std::vector<unsigned char> pixels(width * height, 0);

    for (size_t i = 0; i < text.size(); i++) {
        pixels[i] = static_cast<unsigned char>(text[i]);
    }

    int success = stbi_write_png(outputFilename.c_str(), width, height, 1, pixels.data(), width);
    return success != 0;
}
