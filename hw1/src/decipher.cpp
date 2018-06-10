#include <iostream>
#include <string>
#include "decipher.h"
using namespace std;

void Decipher::ShiftCipher(int n) {
    int i, j;
    int temp;
    p_str = c_str;

    if(n >= 0) {
        for(j = 0; j < c_str.length(); j ++) {
            temp = c_str[j] - 'A';
            temp = (temp + 26 - n) % 26;
            p_str[j] = temp + 'A';
        }
        
        printf("Shift size: %d\n", n);
        printf("%s\n", p_str.c_str());
        return;
    }

    for(i = 0; i < 26; i ++) {
        for(j = 0; j < c_str.length(); j ++) {
            temp = c_str[j] - 'A';
            temp = (temp + 26 - i) % 26;
            p_str[j] = temp + 'A';
        }
        
        printf("Shift size: %d\n", i);
        printf("%s\n", p_str.c_str());
    }
}

void Decipher::TBoxCipher(const int *ctbl, int m) {
    int i, j;
    char *res = new char[c_str.length()+1];
    res[c_str.length()] = '\0';

    for(i = 0; i < c_str.length(); i += m) {
        TBox(c_str.c_str() + i, res + i, ctbl, m);
    }
    
    p_str = res;
    printf("%s\n", res);
    delete [] res;
}

void Decipher::TBox(const char *src, char *dst, const int *ctbl, int m) {
    for(int i = 0; i < m; i ++) {
        dst[ctbl[i]] = src[i];
    }
}

void Decipher::SBox(const char *src, char *dst, int len, const char *stbl) {
    char x;
    for(int i = 0; i < len; i ++) {
        dst[i] = stbl[src[i]-'A'];
    }
}

void Decipher::printCP(const char *cs, const char *ps, int len, int MAXLINELEN) {
    int line_num = len / MAXLINELEN + 1;
    int line_start = 0;

    printf("----- Cipher Compare -----\n\n");
    for (int i = 0; i < line_num; i ++) {
        line_start = i * MAXLINELEN;
        // print plain
        for (int j = 0; j < MAXLINELEN && line_start + j < len; j++) printf("%c", ps[line_start + j]);
        printf("\n");
        // print cipher
        for (int j = 0; j < MAXLINELEN && line_start + j < len; j++) printf("%c", cs[line_start + j]);
        printf("\n\n");
    }
    printf("----- End -----\n\n");
}

void Decipher::VigenereCipher(const string &key) {
    int i, t;
    for(i = 0; i < c_str.length(); i ++) {
        t = c_str[i] - 'A';
        t -= key[i%key.length()] - 'A';
        p_str[i] = (26 + t) % 26 + 'A';
    }
}