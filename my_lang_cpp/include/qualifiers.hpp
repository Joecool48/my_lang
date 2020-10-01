#ifndef QUALIFIERS_H
#define QUALIFIERS_H

#include<unordered_map>
#include<string>

using namespace std;

class QualifierInfo {
    // TODO leave blank for now
    QualifierInfo();
};

// very simple class that for now maintains a few initialized maps
class Qualifiers {
    unordered_map<string, QualifierInfo> functionQualifiers;
    unordered_map<string, QualifierInfo> classQualifiers;
    unordered_map<string, QualifierInfo> varQualifiers;
    Qualifiers(); // just an init constructor
}

#endif /* QUALIFIERS_H */
