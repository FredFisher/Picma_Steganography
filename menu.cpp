//
// Created by JJ on 27/10/2026.
//

#include "menu.h"
#include "text_input.h"
#include "picture_input.h"

#include <iostream>
#include <ostream>


menu::menu() {

}

void menu::showOptions() {
    std::cout<<"-----*PICMA*-----"<<std::endl;
    std::cout<<"Welcome to the Picma menu!"<<std::endl;
    std::cout<<"Choose an option by entering 1 or 2 and hit enter."<<std::endl;
    std::cout<<"1. encrypt a textfile([...].txt) to a picture([...].png)"<<std::endl;
    std::cout<<"2. decrypt a picture([...].png) to a textfile([...].txt)"<<std::endl;
}

void menu::chooseOption () {
    int userInput = 0;

    while (userInput < 1 || userInput > 2) {
        std::cin >> userInput;

        switch (userInput) {
            case 1:{
                text_input input;
                input.showOptions();
                input.chooseOption();
                break;
            }
            case 2:
                //add pic to file
                break;

            default:
                std::cout<<"Wrong Input. Try Again"<<std::endl << std::endl;
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                userInput = 0;
        }
    }
}
