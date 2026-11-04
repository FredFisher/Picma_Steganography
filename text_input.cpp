//
// Created by JJ on 27/10/2026.
//

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "text_input.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <sstream>

#include "stb_image_write.h" //credit goes to the authors @ https://github.com/nothings/stb/blob/master/stb_image_write.h
#include "stb_image.h" //credit goes to the authors @ https://github.com/nothings/stb/blob/master/stb_image.h


text_input::text_input() {}

void text_input::showOptions() {
    std::cout<<"-----*Text to Picture*-----"<<std::endl;
    std::cout<<"Choose an option by entering 1 or 2 and hit enter."<<std::endl;
    std::cout<<"1. classic encrypt a textfile"<<std::endl;
    std::cout<<"2. sort encrypt a textfile"<<std::endl;
    std::cout<<"3. symmetric encrypt a textfile"<<std::endl;
}

void text_input::chooseOption () {
    int userInput = 0;
    std::string inputFilename;
    std::string outputFilename;
    std::vector<std::vector<size_t>> originalIndices;

    int width = 200;

    while (userInput < 1 || userInput > 3) {
        std::cin >> userInput;

        switch (userInput) {
            case 1:
                std::cout << "Please enter the filename you would like to encrypt"<<std::endl;
                std::cout << "Has to be an absolute path! Use double dashes for single dashes"<<std::endl;
                std::cout << "e.g.: C:\\\\Users\\\\JJ\\\\Desktop\\\\input.txt"<<std::endl;
                std::cin >> inputFilename;
                std::cout << "Please enter the output filename for the encrypted picture"<<std::endl;
                std::cout << "Has to be an absolute path! Use double dashes for single dashes"<<std::endl;
                std::cout << "e.g.: C:\\\\Users\\\\JJ\\\\Desktop\\\\output.png"<<std::endl;
                std::cin >> outputFilename;
                std::cout << "Please enter the width for the encrypted picture"<<std::endl;
                std::cout << "(A width of 200 would be 200 pixels wide)"<<std::endl;
                std::cin >> width;

                convertFileToString(inputFilename, outputFilename, width);

                break;

            case 2:
                std::cout << "Please enter the filename you would like to encrypt"<<std::endl;
                std::cout << "Has to be an absolute path! Use double dashes for single dashes"<<std::endl;
                std::cout << "e.g.: C:\\\\Users\\\\JJ\\\\Desktop\\\\input.txt"<<std::endl;
                std::cin >> inputFilename;
                std::cout << "Please enter the output filename for the encrypted picture"<<std::endl;
                std::cout << "Has to be an absolute path! Use double dashes for single dashes"<<std::endl;
                std::cout << "e.g.: C:\\\\Users\\\\JJ\\\\Desktop\\\\output.png"<<std::endl;
                std::cin >> outputFilename;
                std::cout << "Please enter the width for the encrypted picture"<<std::endl;
                std::cout << "(A width of 200 would be 200 pixels wide)"<<std::endl;
                std::cin >> width;

                convertFileToString(inputFilename, outputFilename, width);
                originalIndices = sortPixelsPerRow(outputFilename);

                std::cout << "Please enter the filename for the key"<<std::endl;
                std::cout << "Has to be an absolute path! Use double dashes for single dashes"<<std::endl;
                std::cout << "e.g.: C:\\\\Users\\\\JJ\\\\Desktop\\\\key.png"<<std::endl;
                std::cin >> outputFilename;
                convertStringToPicture(generateKeyString(originalIndices), outputFilename,width);

                break;

            case 3:
                /*
                std::cout << "Please enter the filename you would like to encrypt"<<std::endl;
                std::cout << "Has to be an absolute path! Use double dashes for single dashes"<<std::endl;
                std::cout << "e.g.: C:\\\\Users\\\\JJ\\\\Desktop\\\\input.txt"<<std::endl;
                std::cin >> inputFilename;
                std::cout << "Please enter the output filename for the encrypted picture"<<std::endl;
                std::cout << "Has to be an absolute path! Use double dashes for single dashes"<<std::endl;
                std::cout << "e.g.: C:\\\\Users\\\\JJ\\\\Desktop\\\\output.png"<<std::endl;
                std::cin >> outputFilename;
                std::cout << "Please enter the width for the encrypted picture"<<std::endl;
                std::cout << "(A width of 200 would be 200 pixels wide)"<<std::endl;
                std::cin >> width;

                convertFileToString(inputFilename, outputFilename, width);
                originalIndices = sortPixelsPerRow(outputFilename);

                std::cout << "Please enter the filename for the key"<<std::endl;
                std::cout << "Has to be an absolute path! Use double dashes for single dashes"<<std::endl;
                std::cout << "e.g.: C:\\\\Users\\\\JJ\\\\Desktop\\\\key.png"<<std::endl;
                std::cin >> outputFilename;
                convertStringToPicture(generateKeyString(originalIndices), outputFilename,width);
                */
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

std::vector<std::vector<size_t>> text_input::sortPixelsPerRow(std::string& outputFilename) {
    int width = 0, height = 0, channels = 0;

    unsigned char* img = stbi_load(outputFilename.c_str(), &width, &height, &channels, 1);
    if (img == NULL) {
        std::cerr << "Failed to load image: " << outputFilename << std::endl;
        return {};
    }
    std::vector<std::vector<size_t>> originalIndices(height, std::vector<size_t>(width));
    std::vector<unsigned char> sortedPixels(width * height);

    for (size_t i = 0; i < height; i++) {
        std::vector<size_t> rowIndices(width);
        std::iota(rowIndices.begin(), rowIndices.end(), 0);

        unsigned char* currentRow = img + i * width;

        std::sort(rowIndices.begin(), rowIndices.end(), [currentRow](size_t a, size_t b) {
            return currentRow[a] < currentRow[b];
        });

        originalIndices[i] = rowIndices;

        for (size_t j = 0; j < width; j++) {
            sortedPixels[i * width + j] = currentRow[rowIndices[j]];
        }
    }

    //std::string sortedFilename = "sorted" + outputFilename;
    //stbi_write_png(sortedFilename.c_str(), width, height, 1, sortedPixels.data(), width);
    stbi_write_png(outputFilename.c_str(), width, height, 1, sortedPixels.data(), width);

    stbi_image_free(img);

    return originalIndices;
}

std::string text_input::generateKeyString(std::vector<std::vector<size_t>> originalIndices) {
    size_t height = originalIndices.size(), width = height > 0 ? originalIndices[0].size() : 0;

    std::ostringstream oss(std::ios_base::binary);

    oss.write(reinterpret_cast<char*>(&height), sizeof(height));
    oss.write(reinterpret_cast<char*>(&width), sizeof(width));

    for (const auto& row : originalIndices) {
        oss.write(reinterpret_cast<const char*>(row.data()), row.size() * sizeof(size_t));
    }
    return std::string(oss.str());
}

