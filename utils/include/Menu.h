#ifndef MENU_H
#define MENU_H

#include <string>
#include <iostream>

using namespace std;

class Menu {
public:
    static void draw(const string& title, const string options[], int numOptions) {
        int max_len = title.length();
        for (int i = 0; i < numOptions; i++) 
        {
            if (options[i].length() > max_len) 
            {
                max_len = options[i].length();
            }
        }
        max_len += 4;
        
        cout << "+" << string(max_len, '-') << "+" << endl;
        cout << "| " << title << string(max_len - title.length() - 1, ' ') << "|" << endl;
        cout << "+" << string(max_len, '-') << "+" << endl;
        for (int i = 0; i < numOptions; i++) 
        {
            cout << "| " << options[i] << string(max_len - options[i].length() - 1, ' ') << "|" << endl;
        }
        cout << "+" << string(max_len, '-') << "+" << endl;
    }  
};

#endif