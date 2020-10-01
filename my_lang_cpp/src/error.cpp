#include "../include/error.hpp"

Error::Error() : errorType(ErrorType::NoException), lineNum(0), colNum(0) {}

Error::Error(ErrorType errorType) : errorType(errorType), lineNum(0), colNum(0) {}

Error::Error(ErrorType errorType, uint64_t lineNum, uint64_t colNum) : errorType(errorType), lineNum(lineNum), colNum(colNum) {}

Error::Error(ErrorType errorType, const string & msg, uint64_t lineNum, uint64_t colNum) : errorType(errorType), msg(msg), lineNum(lineNum), colNum(colNum) {}

Error::Error(ErrorType errorType, const string & msg) : errorType(errorType), msg(msg), lineNum(0), colNum(0) {}

string Error::locationToString() {
    return to_string(this->lineNum) + ":" + to_string(this->colNum);
}

string Error::what() {
    switch (errorType) {
        case ErrorType::MissingParenException:
            return ("Expected ')' after '(' at " + locationToString());
        case ErrorType::ExpectedLeftParenException:
            return ("Expected '(' after " + msg + " at " + locationToString());
        case ErrorType::ExpectedIdentifierException:
            return ("Expected identifier after " + msg + " at " + locationToString());
        case ErrorType::ExpectedCurlyException:
            return ("Expected { after " + msg + " at " + locationToString());
        case ErrorType::AlreadyDeclaredException:
            return (msg + " at " + locationToString());
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

void ErrorHandler::reportError(Error e) {
    // this is all it'll do for now
    cout << e.what() << endl;
}
