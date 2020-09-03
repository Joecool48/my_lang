#include "../include/error.hpp"

Error::Error() : errorType(ErrorType::NoException), lineNum(0), colNum(0) {}

Error::Error(ErrorType errorType) : errorType(errorType), lineNum(0), colNum(0) {}

Error::Error(ErrorType errorType, uint64_t lineNum, uint64_t colNum) : errorType(errorType), lineNum(lineNum), colNum(colNum) {}

Error::Error(ErrorType errorType, const string & msg, uint64_t lineNum, uint64_t colNum) : errorType(errorType), msg(msg), lineNum(lineNum), colNum(colNum) {}

Error::Error(ErrorType errorType, const string & msg) : errorType(errorType), msg(msg), lineNum(0), colNum(0) {}

string Error::what() {
    switch (errorType) {
        case ErrorType::MissingParenException:
            return ("Expected ')' after '(' at " + to_string(this->lineNum) + ":" + to_string(this->colNum));
        default:
            return "No exception";
    }
}

/*MissingParenException::MissingParenException(uint64_t lineNum, uint64_t colNum) {
    this->lineNum = lineNum;
    this->colNum = colNum;    
}
*/
/*const char * MissingParenException::what() const throw() {
    return ("Expected ')' after '(' at " + to_string(this->lineNum) + ":" + to_string(this->colNum)).c_str();
}
*/

void ErrorHandler::reportError(const Error & e) {
         
}
