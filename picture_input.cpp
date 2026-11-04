//
// Created by JJ on 27/10/2026.
//

#include "picture_input.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "stb_image.h"
#include "stb_image_write.h" //credit goes to the authors @ https://github.com/nothings/stb/blob/master/stb_image_write.h

picture_input::picture_input() {}

void picture_input::showOptions() {
    std::cout<<"-----*Picture to Text*-----"<<std::endl;
    std::cout<<"Choose an option by entering 1 or 2 and hit enter."<<std::endl;
    std::cout<<"1. classic decrypt a picture"<<std::endl;
    std::cout<<"2. symmetric decrypt a picture"<<std::endl;
}

void picture_input::chooseOption () {
    int userInput = 0;
    std::string inputFilename;
    std::string outputFilename;

    while (userInput < 1 || userInput > 2) {
        std::cin >> userInput;

        switch (userInput) {
            case 1:
                std::cout << "Please enter the filename you would like to decrypt"<<std::endl;
                std::cout << "Has to be an absolute path! Use double dashes for single dashes"<<std::endl;
                std::cout << "e.g.: C:\\\\Users\\\\JJ\\\\Desktop\\\\input.png"<<std::endl;
                std::cin >> inputFilename;
                std::cout << "Please enter the output filename for decrypted message"<<std::endl;
                std::cout << "Has to be an absolute path! Use double dashes for single dashes"<<std::endl;
                std::cout << "e.g.: C:\\\\Users\\\\JJ\\\\Desktop\\\\output.txt"<<std::endl;
                std::cin >> outputFilename;

                convertStringToFile(convertPictureToString(inputFilename, outputFilename),outputFilename);

                break;

            case 2:
                //add sorted
                break;

            case 3:
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

std::string picture_input::convertPictureToString(const std::string& inputFilename, const std::string& outputFilename) {
    int imgWidth, imgHeight, imgChannels;
    unsigned char* imgData = stbi_load(inputFilename.c_str(), &imgWidth, &imgHeight, &imgChannels, 1);

    if (imgData == NULL) {
        return "Error loading file";
    }
    std::string content(reinterpret_cast<char*>(imgData), imgWidth * imgHeight);

    stbi_image_free(imgData);

    return content;
}

bool picture_input::convertStringToFile(const std::string& text, const std::string& outputFilename) {
    if (text.empty()) {
        return false;
    }
    std::ofstream file(outputFilename, std::ios::binary);
    if (file.is_open()) {
        return false;
    }
    file.write(text.data(), text.size());
    file.close();

    return true;
}

// toDo: funktion get real indices from key

// toDo: funktion reassamble original picture via original indices
