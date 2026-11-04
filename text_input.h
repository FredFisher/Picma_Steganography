//
// Created by JJ on 27/10/2026.
//

#ifndef PICMA_TEXT_INPUT_H
#define PICMA_TEXT_INPUT_H

#include <string>
#include <vector>


class text_input {
    public:
        text_input();
        void showOptions();
        void chooseOption ();
        bool convertFileToString(const std::string& inputFilename, const std::string& outputFilename, int width);
        bool convertStringToPicture(const std::string& text, const std::string& outputFilename, int width);
        std::vector<std::vector<size_t>>sortPixelsPerRow(std::string& outputFilename);
        //bool generateKeyPicture(std::vector<std::vector<size_t>> originalIndices, std::string& outputFilename);
        std::string generateKeyString(std::vector<std::vector<size_t>> originalIndices);
};


#endif //PICMA_TEXT_INPUT_H
