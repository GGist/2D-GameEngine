/***********************************************************************************
*This is a utility class for generating sprites based off of current images. You   *
*can flip sprites horizontally and/or vertically aswell as overwrite a current     *
*color with an alpha channel. This class requires the graphics, window, and system *
*modules of SFML (2.0 or later).                                                   *
***********************************************************************************/

#include <iostream>
#include <cctype>
#include <fstream>
#include <SFML/Graphics.hpp>

using namespace std;

static const unsigned int MIN_COMMAND_LENGTH = 9, MAX_COLOR_LENGTH = 3, MAX_COLOR = 255;
static const string QUIT = "quit";

struct Image_Mod {
    Image_Mod() : horizontal(false), vertical(false),
    alpha(false), cReplace(), sAppend() {}

    bool horizontal,
         vertical,
         alpha;
    sf::Color cReplace;
    string sAppend;
};

void purgeWhite(string& input);
bool isValid(string input, Image_Mod& im);

int main()
{
    cout << "a";
    //Can flip and/or create alpha channel
    cout << "\t\t\t\t**Sprite Manipulator**" << endl
         << "Instructions:" << endl
         << "1. Create a folder in this directory called \"data\"" << endl
         << "2. Create a text file called \"Manifest\" in the folder data" << endl
         << "3. Populate Manifest with the file names of each image you would like to"<< endl << "   manipulate, each separated by an enter" << endl
         << "4. Paste all of the images you would like to manipulate in data" << endl << endl;

    cout << "When you are ready to start this utility enter \"start\" followed by at least one" << endl
         << "tag and the string you wish to append to the names of the generated images" << endl
         << "Tags: " << endl << "\t-fv(flip vertical)" << endl << "\t-fh (flip horizontal)" << endl
         << "\t-a (R, G, B) (alpha with the specified color to replace)" << endl << endl;

    cout << "Ex: start -a (255, 0, 0) -fv _Left" << endl
         << "This will create an alpha channel on the color red, flip the images" << endl
         << "vertically, and save the new images with the current filename appended" << endl
         << "with _Left" << endl;


    string userInput;
    Image_Mod iMod;

    cout << endl << "$ ";
    getline(cin, userInput);
    transform(userInput.begin(), userInput.end(), userInput.begin(), ::tolower);

    while (userInput != QUIT) {
        while (!isValid(userInput, iMod)) {
            cout << endl << "Invalid Command" << endl;
            cout << endl << "$ ";
            getline(cin, userInput);
        }


        ifstream manifest;
        string filename;
        sf::Image image;
        vector<string> fileNames(25);

        manifest.open("data/Manifest.txt");

        while (manifest >> filename) {
            image.loadFromFile("data/" + filename + ".png");

            if (iMod.horizontal)
                image.flipHorizontally();
            if (iMod.vertical)
                image.flipVertically();
            if (iMod.alpha)
                image.createMaskFromColor(iMod.cReplace);

            image.saveToFile("data/" + filename + iMod.sAppend + ".png");
        }

        manifest.close();

        cout << endl << "Finished..." << endl;

        cout << endl << "$ ";
        getline(cin, userInput);
        transform(userInput.begin(), userInput.end(), userInput.begin(), ::tolower);
    }

    return 0;
}

void purgeWhite(string& input) {
    int count = 0;
    bool segmentDone = false;

    for (unsigned int i = 0; i < input.length(); ++i) {
        if (input[i] == ' ') {
            ++count;
            segmentDone = false;
        } else {
            segmentDone = true;
        }

        if (segmentDone && count != 0) {
            input.erase(i - count, count);
            i -= count;
            count = 0;
        }
    }
}

bool isValid(string input, Image_Mod& im) {
    purgeWhite(input);
    //Used to preserve filename
    string preservedInput = input;

    transform(input.begin(), input.end(), input.begin(), ::tolower);

    //9 is the minimum amount of characters required for the program
    if (input.find("start-") != 0 || input.length() < MIN_COMMAND_LENGTH)
        return false;

    int index = 6;
    string convert = "";
    int r = 0, g = 0, b = 0, digits = 0;

    while (index < input.length()) {
        switch(input[index])
        {
        case 'f':
            //Flip
            ++index;
            if (input[index] == 'v')
                im.vertical = true;
            else if (input[index] == 'h')
                im.horizontal = true;
            else
                return false;

            ++index;
            break;
        case 'a':
            //Alpha Channel
            convert.clear();
            r = 0;
            g = 0;
            b = 0;
            digits = 0;

            //Increment One or Two (to skip parenthesis)
            if (input[++index] == '(')
                ++index;

            //Pull out first and second color codes
            for (int i = 0; i < 2; ++i) {
                while (input[index] != ',' && digits <= MAX_COLOR_LENGTH) {
                    convert.push_back(input[index]);
                    ++index;
                    ++digits;
                }
                if (digits > MAX_COLOR_LENGTH) {
                    return false;
                } else {
                    if (i == 0)
                        r = atoi(convert.c_str());
                    else
                        g = atoi(convert.c_str());

                    convert.clear();
                    digits = 0;
                }
                ++index;
            }

            //Grab last color code
            while (input[index] != ')' && input[index] != '-' && digits <= MAX_COLOR_LENGTH) {
                convert.push_back(input[index]);
                ++index;
                ++digits;
            }
            if (digits > MAX_COLOR_LENGTH) {
                return false;
            } else {
                b = atoi(convert.c_str());
            }

            //Check color codes are valid
            if (r > MAX_COLOR || g > MAX_COLOR || b > MAX_COLOR)
                return false;

            im.alpha = true;
            im.cReplace.r = r;
            im.cReplace.g = g;
            im.cReplace.b = b;

            ++index;
            break;
        default:
            //Append result filename
            im.sAppend = preservedInput.substr(index, input.size() - index);
            index = input.length();
            break;
    }
    }

    return true;
}

//Test for function isValid()
/*
if (iMod.horizontal)
    cout << "horizontal" << endl;
if (iMod.vertical)
    cout << "vertical" << endl;
if (iMod.alpha)
    cout << "alpha " << static_cast<int>(iMod.cReplace.r) << ' ' << static_cast<int>(iMod.cReplace.g) << ' ' << static_cast<int>(iMod.cReplace.b) << ' ' << endl;
cout << iMod.sAppend << endl;
*/
