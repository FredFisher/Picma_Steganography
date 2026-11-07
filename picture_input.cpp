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


picture_input::picture_input() {}

void picture_input::showOptions() {
    std::cout<<"-----*Picture to Text*-----"<<std::endl;
    std::cout<<"Choose an option by entering 1 or 2 and hit enter."<<std::endl;
    std::cout<<"1. classic decrypt a picture"<<std::endl;
    std::cout<<"2. sort decrypt a picture"<<std::endl;
    std::cout<<"3. symmetric decrypt a picture"<<std::endl;
}

void picture_input::chooseOption () {
    int userInput = 0;
    std::string inputFilename;
    std::string outputFilename;
    std::string keyFilename;
    //hardcoded testvariable
    std::string outputOriginalText;


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
                std::cout << "e.g.: C:\\\\Users\\\\JJ\\\\Desktop\\\\key.png"<<std::endl;
                std::cin >> keyFilename;

                revertSortPixels(inputFilename, outputFilename,convertKeyStringToIndices(convertKeyPictureToString(keyFilename)));

                outputOriginalText = outputFilename;
                outputOriginalText.replace(outputOriginalText.length() - 4, 4, "OriginalText.txt");

                convertPictureToFile(outputFilename, outputOriginalText);
                break;

            case 3:


                break;

            default:
                std::cout<<"Wrong Input. Try Again"<<std::endl << std::endl;
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                userInput = 0;
        }
    }
}

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

std::string picture_input::convertKeyPictureToString(const std::string &keyFilename) {
    int imgWidth, imgHeight, imgChannels;
    unsigned char* imgData = stbi_load(keyFilename.c_str(), &imgWidth, &imgHeight, &imgChannels, 1);

    if (!imgData) {
        throw std::runtime_error("Error loading key file");
    }
    std::string keyString(reinterpret_cast<char*>(imgData), static_cast<size_t>(imgWidth) * imgHeight);

    stbi_image_free(imgData);

    return keyString;
}

std::vector<std::vector<size_t>> picture_input::convertKeyStringToIndices(const std::string& keyString) {
    const size_t headerSize = 2 * sizeof(size_t);
    if (keyString.size() < headerSize) {
        throw std::runtime_error("Key string too short");
    }
    const char* ptr = keyString.data();
    size_t height = 0;
    size_t width = 0;
    std::memcpy(&height, ptr, sizeof(height));
    ptr += sizeof(height);
    std::memcpy(&width, ptr, sizeof(width));
    ptr += sizeof(width);

    if (height == 0 || width == 0 || height > (keyString.size() - headerSize) / sizeof(size_t) / width) {
        throw std::runtime_error("Key string invalid");
    }

    std::vector<std::vector<size_t>> originalIndices(height, std::vector<size_t>(width));
    for (size_t i = 0; i < height; i++) {
        std::memcpy(originalIndices[i].data(), ptr, width * sizeof(size_t));
        ptr += width * sizeof(size_t);
    }
    return originalIndices;
}

bool picture_input::revertSortPixels(const std::string& inputFilename, const std::string& outputFilename, const std::vector<std::vector<size_t>>& originalIndices) {
    if (originalIndices.empty()) {
        return false;
    }
    int imgWidth = 0, imgHeight = 0, imgChannels = 0;
    unsigned char* imgData = stbi_load(inputFilename.c_str(), &imgWidth, &imgHeight, &imgChannels, 1);

    if (imgData == NULL) {
        std::cerr<<"Error loading file"<<std::endl;
        return false;
    }

    if (imgHeight != originalIndices.size() || imgWidth != originalIndices[0].size()) {
        std::cerr<<"Error reverting file"<<std::endl;
        stbi_image_free(imgData);
        return false;
    }

    std::vector<unsigned char> restoredPixels(imgWidth * imgHeight);

    for (int i = 0; i < imgHeight; i++) {
        unsigned char* currentRow = imgData + i * imgWidth;
        const auto& indices = originalIndices[i];

        for (size_t j = 0; j < imgWidth; j++) {
            restoredPixels[i * imgWidth + indices[j]] = currentRow[j];
        }
    }
    int success = stbi_write_png(outputFilename.c_str(), imgWidth, imgHeight, 1, restoredPixels.data(), imgWidth);
    stbi_image_free(imgData);
    return success != 0;
}
