//
// Created by JJ on 27/10/2026.
//

#ifndef PICMA_PICTURE_INPUT_H
#define PICMA_PICTURE_INPUT_H

#include <string>

class picture_input {
    public:
        picture_input();
        void showOptions();
        void chooseOption ();
        std::string convertPictureToString(const std::string& inputFilename, const std::string& outputFilename);
        bool convertStringToFile(const std::string& text, const std::string& outputFilename);

};


#endif //PICMA_PICTURE_INPUT_H
