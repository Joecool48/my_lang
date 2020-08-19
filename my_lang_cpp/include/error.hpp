#ifndef ERROR_HPP
#define ERROR_HPP

#include<stdexcept>
#include<string>
#include<iostream>

using namespace std;

enum class ErrorType {
    MissingParenException,
    NoException
};


class Error : public exception {
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
        const char * what() const throw();
};

#endif
