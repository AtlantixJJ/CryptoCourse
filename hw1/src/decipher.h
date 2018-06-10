#ifndef DECIPHER_H_
#define DECIPHER_H_

#include <iostream>
#include <string>
#include "decipher.h"
using namespace std;


class Decipher {
public:
    Decipher(const string &a):c_str(a){}
    Decipher(){}
    ~Decipher(){}

    void reset(const string &a){p_str = c_str = a;}

    void ShiftCipher(int n=-1);
    void VigenereCipher(const string &key);
    void TBoxCipher(const int *ctbl, int m);
    void printCP()const{printCP(c_str.c_str(), p_str.c_str(), c_str.length(), 40);}
    /// src and dst cannot be the same
    static void TBox(const char *src, char *dst, const int *ctbl, int m);
    // src and dst can be the same
    static void SBox(const char *src, char *dst, int len, const char *stbl);
    // pretty print cipher and plain
    static void printCP(const char *cs, const char *ps, int len, int MAXLINELEN = 40);
private:
    string c_str;
    string p_str;
};

#endif