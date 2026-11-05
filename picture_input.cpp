//
// Created by JJ on 27/10/2026.
//

#include "picture_input.h"

#include <iostream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <vector>
#include <fstream>


#include "stb_image.h"
#include "stb_image_write.h" //credit goes to the authors @ https://github.com/nothings/stb/blob/master/stb_image_write.h
#include "text_input.h"

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
    std::string keyFilename;

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

                convertPictureToFile(inputFilename, outputFilename);

                break;

            case 2:
                std::cout << "Please enter the filename you would like to decrypt"<<std::endl;
                std::cout << "Has to be an absolute path! Use double dashes for single dashes"<<std::endl;
                std::cout << "e.g.: C:\\\\Users\\\\JJ\\\\Desktop\\\\sortedInput.png"<<std::endl;
                std::cin >> inputFilename;
                std::cout << "Please enter the output filename for decrypted message"<<std::endl;
                std::cout << "Has to be an absolute path! Use double dashes for single dashes"<<std::endl;
                std::cout << "e.g.: C:\\\\Users\\\\JJ\\\\Desktop\\\\output.txt"<<std::endl;
                std::cin >> outputFilename;
                std::cout << "Please enter the key filename for decrypted message"<<std::endl;
                std::cout << "Has to be an absolute path! Use double dashes for single dashes"<<std::endl;
                std::cout << "e.g.: C:\\\\Users\\\\JJ\\\\Desktop\\\\keyInput.txt"<<std::endl;
                std::cin >> keyFilename;

                convertKeyStringToIndices(convertPictureToString(keyFilename));

                //convertPictureToFile(inputFilename, outputFilename);
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
/*
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

*/

bool picture_input::convertPictureToFile(const std::string& inputFilename, const std::string& outputFilename) {
    int width, height, channels;
    unsigned char* imgData = stbi_load(inputFilename.c_str(), &width, &height, &channels, 1);

    if (imgData == NULL) {
        return false;
    }

    std::ofstream file(outputFilename);
    if (!file.is_open()) {
        return false;
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            unsigned char pixels = imgData[i * width + j];

            file << static_cast<char>(pixels);
        }
        file << std::endl;
    }
    stbi_image_free(imgData);

    return true;
}


std::string picture_input::convertPictureToString(const std::string& keyFilename) {
    int imgWidth, imgHeight, imgChannels;
    std::string outputString;
    unsigned char* imgData = stbi_load(keyFilename.c_str(), &imgWidth, &imgHeight, &imgChannels, 1);

    if (imgData == NULL) {
        return "Error loading file";
    }
    outputString.clear();

    for (int i = 0; i < imgHeight; i++) {
        for (int j = 0; j < imgWidth; j++) {
            unsigned char pixels = imgData[i * imgWidth + j];
            outputString += static_cast<char>(pixels);
        }
        outputString += "\n";
    }

    stbi_image_free(imgData);

    return outputString;
}


std::vector<std::vector<size_t>> picture_input::convertKeyStringToIndices(const std::string& keyString) {
    if (keyString.size() < sizeof(size_t) * 2) {
        throw std::runtime_error("Key string is too short to be a valid key string");
    }
    const char* ptr = keyString.data();

    size_t height = 0;
    size_t width = 0;

    std::memcpy(&height, ptr, sizeof(height));
    ptr += sizeof(height);

    std::memcpy(&width, ptr, sizeof(width));
    ptr += sizeof(width);

    size_t expected_size = sizeof(size_t) * 2 + height * width * sizeof(size_t);
    if (keyString.size() != expected_size) {
        throw std::runtime_error("Key string is not a valid key string");
    }

    std::vector<std::vector<size_t>> originalIndices(height, std::vector<size_t>(width));

    for (size_t i = 0; i < height; i++) {
        std::memcpy(originalIndices[i].data(), ptr, sizeof(size_t));
        ptr += width * sizeof(size_t);
    }
    return originalIndices;
}

// toDo: funktion get real indices from key

// toDo: funktion reassamble original picture via original indices

bool picture_input::revertSortPixels(const std::string& inputFilename, const std::string& outputFilename) {}
