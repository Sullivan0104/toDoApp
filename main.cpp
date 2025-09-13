#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <iterator>
#include <sstream>
#include <algorithm>

using namespace std;

    vector<string> toDoList;

    vector<int> parse_LineForNumber(string& itemNumberLine){
        vector<int> numbers;
        stringstream ss(itemNumberLine);
        string token;

        while (getline(ss, token, ',')){
            stringstream subss(token);
            int num;
            while(subss >> num){
                numbers.push_back(num);
            }
        }
        return numbers;
    }

    void printWrappedItem(const string& item, int number, size_t maxWidth) {
        string prefix = "\033[38;2;255;182;193m" + to_string(number) + ".\033[0m "; 
        size_t prefixLen = number < 10 ? 3 : to_string(number).length() + 2;

        size_t start = 0;
        while (start < item.length()) {
            size_t availableWidth = (start == 0) ? maxWidth - prefixLen : maxWidth - 4;
            size_t end = start + availableWidth;

            if (end >= item.length()) {
                if (start == 0)
                    cout << prefix << "\033[38;2;135;206;250m" << item.substr(start) << "\033[0m" << "\n\n";
                else
                    cout << "    " << "\033[38;2;135;206;250m" << item.substr(start) << "\033[0m" << "\n\n";
                break;
            }

            size_t breakPos = item.rfind(" ", end);
            if (breakPos == string::npos || breakPos <= start)
                breakPos = end;

            if (start == 0)
                cout << prefix << "\033[38;2;135;206;250m" << item.substr(start, breakPos - start) << "\033[0m" << "\n";
            else
                cout << "    " << "\033[38;2;135;206;250m" << item.substr(start, breakPos - start) << "\033[0m" << "\n";

            start = breakPos + 1; // Skip space
        }
    }


    void printList(void)
    {
        int counter = 1;

        // Header
        cout << "\033[38;2;255;182;193m" << "\n" << "TO-DO-LIST" << "\n";
        cout << "==================================================================================" << "\033[0m" << "\n";

        // Items
        for(const string& i : toDoList)
        {
            printWrappedItem(i, counter, 82);
            counter++;
        }

        // Footer line
        cout << "\033[38;2;255;182;193m==================================================================================\033[0m" << "\n";


    cout << "\033[1mAccepted Commands: "
     // Add - green text
     << "\033[48;2;50;50;50;38;2;0;255;0m Add \033[0m\033[1m, "
     // Remove - red text
     << "\033[48;2;50;50;50;38;2;255;0;0m Remove \033[0m\033[1m, "
     // Clear - yellow text
     << "\033[48;2;50;50;50;38;2;255;255;0m Clear \033[0m\033[1m, "
     // Exit - magenta text
     << "\033[48;2;50;50;50;38;2;255;0;255m Exit \033[0m"
     << "\033[0m" << "\n";
    }

    void clearScreen(){
        #ifdef _WIN32
        system("cls");
        #else
        system("clear");
        #endif
    }


int main(int argc, char const *argv[])
{

    ifstream importFile("./list.txt");
    string line {};

    if(importFile.is_open())
    {
        while(getline(importFile, line)){
            toDoList.push_back(line);
        }
        importFile.close();
    }

    while(true)
    {
        clearScreen();
        printList();

        string input {};
        getline(cin, input);

        if(input == "exit"|| input =="Exit"){
            break;
        }

        if(input == "add"|| input == "Add"){
            string newItem;
            cout << "Enter Item: ";
            getline(cin, newItem);
            toDoList.push_back(newItem);
        }

        if(input == "remove"|| input == "Remove"){
        string itemNumberLine {};
           cout << "Choose Item(s) to Remove(1, 2, 5, ...): " << "\n";
           getline(cin, itemNumberLine);

           vector<int> itemNumbers = parse_LineForNumber(itemNumberLine);

           sort(itemNumbers.begin(), itemNumbers.end(), greater<int>());

           for(size_t index : itemNumbers){
            if(index > 0 && index <= toDoList.size()){
                toDoList.erase(toDoList.begin() + (index - 1));
            }
           }
        }

        if(input == "clear"|| input == "Clear"){
            toDoList.erase(toDoList.begin(),toDoList.end());
        }

        ofstream exportFile("./list.txt");
        if(exportFile.is_open()){
            ostream_iterator<string> output_iterator(exportFile, "\n");
            copy(begin(toDoList), end(toDoList), output_iterator);
        }

    }

    return 0;
}
