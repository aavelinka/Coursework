#ifndef FILEEXCEPTION_H
#define FILEEXCEPTION_H

#include "Exception.h"

using namespace std;

class FileException : public Exception 
{
public:
    FileException(int code, const string& message) : Exception(code, "File Error: " + message) {}
    ~FileException() {}
};

#endif