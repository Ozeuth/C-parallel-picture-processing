#include <iostream>
#include <list>
#include <thread>
#include <fstream>

#include "Colour.hpp"
#include "Utils.hpp"
#include "Picture.hpp"
#include "PicLibrary.hpp"

using namespace std;
enum Commands {
    liststore, load, unload, save, exits, display, invert, grayscale, rotate, flip, blur, strat, faulty
};
static std::map<std::string, Commands> commandList;

void Initialize() {
    commandList["liststore"] = Commands::liststore;
    commandList["load"] = Commands::load;
    commandList["unload"] = Commands::unload;
    commandList["save"] = Commands::save;
    commandList["exit"] = Commands::exits;
    commandList["display"] = Commands::display;
    commandList["invert"] = Commands::invert;
    commandList["grayscale"] = Commands::grayscale;
    commandList["rotate"] = Commands::rotate;
    commandList["flip"] = Commands::flip;
    commandList["blur"] = Commands::blur;
    commandList["strat"] = Commands::strat;
    commandList["faulty"] = Commands::faulty;
}

int main(int argc, char **argv) {

    cout << "------------------------------------------------------" << endl;
    cout << "                   Welcome to Hell!                   " << endl;
    cout << "------------------------------------------------------" << endl;

    // write main     IO loop that runs the command-line interpreter interactive shell
    Initialize();
    PicLibrary *library = new PicLibrary();
    list<thread> threadList;

    int currentStep = 0;  // So that we know relative priority of objects in queue
    // First, we read whatever arguments had been passed to it
    for (int i = 1; i < argc; i++) {
        string fileName = argv[i];
        unsigned int indexOfLastSlash = fileName.find_last_of("\\/");
        if (indexOfLastSlash != string::npos) {
            fileName.erase(0, indexOfLastSlash + 1);
        }
        library->loadpicture(argv[i], fileName,
                             -1); // We can leave command Order at -1 as they should all be done sequentially.
    }
/*
    // REMOVE THIS LATER
    ifstream inFile;
    inFile.open("../example_input.txt");

    if (!inFile) {
        cerr << "Unable to open file file";
        exit(1);   // call system to stop
    }
*/
    string command;
    while (getline(cin, command)) {
        // << "-------------------- Your Input: ---------------------" //
        vector<string> allArguments;
        Commands commandToExecute;
        if (command.empty()){
            commandToExecute = commandList["faulty"];
        } else {
            string toAdd;
            cout << command << endl;
            stringstream commandstream(command);
            while (commandstream >> toAdd) {
                allArguments.push_back(toAdd);
            }
            // "---------------------- Results: ----------------------" //

            string commands = allArguments.front();
            if (commandList.find(commands) == commandList.end()) {
                // There was no such commmand, so we head to a blank, warning state
                commandToExecute = commandList["faulty"];
            } else { commandToExecute = commandList[commands]; }
        }

        switch (commandToExecute) {
            case Commands::liststore: {
                library->add(currentStep, "NULL");
                std::thread currentThread(&PicLibrary::print_picturestore, library, currentStep);
                threadList.push_back(std::move(currentThread));
                //library->print_picturestore(currentStep);
                break;
            }
            case Commands::load: {
                // 1. Path 2. Name
                library->add(currentStep, allArguments[2]);
                std::thread currentThread(&PicLibrary::loadpicture, library, allArguments[1], allArguments[2],
                                          currentStep);
                threadList.push_back(std::move(currentThread));
                //library->loadpicture(allArguments[1], allArguments[2], currentStep);  // We send it off to a thread to complete
                break;
            }
            case Commands::unload: {
                // 1. Name
                library->add(currentStep, allArguments[1]);
                std::thread currentThread(&PicLibrary::unloadpicture, library, allArguments[1], currentStep);
                threadList.push_back(std::move(currentThread));
                //library->unloadpicture(allArguments[1], currentStep);
                break;
            }
            case Commands::save: {
                // 1. Name 2. Path
                library->add(currentStep, allArguments[1]);
                std::thread currentThread(&PicLibrary::savepicture, library, allArguments[1], allArguments[2],
                                          currentStep);
                threadList.push_back(std::move(currentThread));
                //library->savepicture(allArguments[1], allArguments[2], currentStep);
                break;
            }
            case Commands::exits: {
                goto exitPhase; // Unfortunate use of goto. But in this case makes logical sense
            }
            case Commands::display: {
                // 1. Name
                library->add(currentStep, allArguments[1]);
                std::thread currentThread(&PicLibrary::display, library, allArguments[1], currentStep);
                threadList.push_back(std::move(currentThread));
                //library->display(allArguments[1], currentStep);
                break;
            }
            case Commands::invert: {
                // 1. Name
                library->add(currentStep, allArguments[1]);
                std::thread currentThread(&PicLibrary::invert, library, allArguments[1], currentStep);
                threadList.push_back(std::move(currentThread));
                //library->invert(allArguments[1], currentStep);
                break;
            }
            case Commands::grayscale: {
                // 1. Name
                library->add(currentStep, allArguments[1]);
                std::thread currentThread(&PicLibrary::grayscale, library, allArguments[1], currentStep);
                threadList.push_back(std::move(currentThread));
                //library->grayscale(allArguments[1], currentStep);
                break;
            }
            case Commands::rotate: {
                // 1. 90/180/270 2. Name
                library->add(currentStep, allArguments[2]);
                int angle;
                angle = std::stoi(allArguments[1]);
                std::thread currentThread(&PicLibrary::rotate, library, angle, allArguments[2], currentStep);
                threadList.push_back(std::move(currentThread));
                //library->rotate(angle, allArguments[2], currentStep);
                break;
            }
            case Commands::flip: {
                // 1. H/V 2. Name
                library->add(currentStep, allArguments[2]);
                char hv;
                hv = allArguments[1].at(0);
                std::thread currentThread(&PicLibrary::flipVH, library, hv, allArguments[2], currentStep);
                threadList.push_back(std::move(currentThread));
                //library->flipVH(hv, allArguments[2], currentStep);
                break;
            }
            case Commands::blur: {
                // 1. Name
                library->add(currentStep, allArguments[1]);
                std::thread currentThread(&PicLibrary::blur, library, allArguments[1], currentStep);
                threadList.push_back(std::move(currentThread));
                //library->blur(allArguments[1], currentStep);
                break;
            }
            case Commands::strat: {
                // 1. Type of Parallelism
                // NOTE: This is not thread safe. It is possible that a function calling blur BEFORE
                // strat change, will change to do this strat. I can change this but it will require
                // a more complicated synchronisation list to include method type. Besides, users
                // not care how they get their results, merely that they get it.
                library->changeStrat(allArguments[1]);
                break;
            }
            case Commands::faulty: {
                cout << "Syntax of this command is wrong!" << endl;
                // Should i decrement currentStep?
                break;
            }
            default: {
                cout << "It should never enter default case" << endl;
                break;
            }
        }
        currentStep++;
    }

    exitPhase:
    // This is what exit does
    std::list<thread>::iterator it;
    for (it = threadList.begin(); it != threadList.end(); it++) {
        it->join();
    }
    return 0;
}

