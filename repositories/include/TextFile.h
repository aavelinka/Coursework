#ifndef TEXTFILE_H
#define TEXTFILE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <type_traits>

using namespace std;

template <typename T>
class TextFile : public File
{
public:
    TextFile(const string& fname) : File(fname) {}
    ~TextFile() = default;

    void saveRecord(const T& object);
    T* readRecord();
    vector<T*> readAllRecords();
    void clearFile() const;
    void openFile(fstream& file, ios_base::openmode mode) const;
    bool hasRecords() const {
        ifstream file(filename);
        return file.good() && file.peek() != EOF;
    }
};

#include "TextFile.tpp"

#endif