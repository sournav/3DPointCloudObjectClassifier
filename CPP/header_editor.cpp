//
//  main.cpp
//  CloudFileEditer
//
//  Created by Abdullah AbuAljamal on 9/29/18.
//  Copyright © 2018 Abdullah. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;
string  NEW_FILE_DIRCTORY, ORIGINAL_FILE_DIRCTORY;
int getNum(string myfile) { // Function that takes the number of points from the 10th line and save to an int "num"
    fstream infile;
    string line;
    string line10;
    int count = 0;
    infile.open(myfile);
    while ( getline (infile,line) ) { // goes through the lines
        count++;
        if (count == 10) {
            line10 = line; // save line 10 to a string
            break;
        }
    }
    infile.close();
    
    string tmp;
    stringstream ss;
    ss << line10;
    int value;
    while (!ss.eof()) {
        ss >> tmp;
        if (stringstream(tmp) >> value) { // get the number from the string and return it as a int
            cout << value << endl;
        }
        tmp = "";
    }
    return value;
    
}

void fileGen(string orgFile, string newFile, int num) { // Function that get the Point Cloud Data file and write the points into a new file with number of points in the first line
    fstream infile;
    ofstream outfile;
    infile.open(orgFile);
    outfile.open(newFile);
    outfile << num << "\n"; // write the number of points in the first line
    string line;
    int lineCount = 0;
    while (getline(infile, line)) { // write the points to the new file
        lineCount++;
        if (lineCount > 11) {
            outfile << line << "\n";
        }
    }
        
    
}

int main () {
    int num;
    num = getNum(ORIGINAL_FILE_DIRCTORY);
    fileGen(ORIGINAL_FILE_DIRCTORY , NEW_FILE_DIRCTORY , num);

    return 0;
}
