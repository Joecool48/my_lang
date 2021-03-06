#ifndef ERROR_HPP
#define ERROR_HPP

#include<stdexcept>
#include<string>
#include<iostream>
#include<vector>

using namespace std;

enum class ErrorType {
    MissingParenException,
    ExpectedLeftParenException,
    NoException
};


class Error {
    public:
        ErrorType errorType;
        string msg;
        uint64_t lineNum;
        uint64_t colNum;
        Error();
        explicit Error(ErrorType errorType);
        Error(ErrorType errorType, uint64_t lineNum, uint64_t colNum);
        Error(ErrorType errorType, const string & msg, uint64_t lineNum, uint64_t colNum);
        Error(ErrorType errorType, const string & msg);
        string locationToString();
        string what();
};

class ErrorHandler {
    public:
    void reportError(Error e);
    private:
    vector<Error> errors;
};


#endif
