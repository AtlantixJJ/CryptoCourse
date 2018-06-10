#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "decipher.h"
using namespace std;

void search_cipher(int n, const string &cipher_text, char candidate[26][26], char *stbl, char *buf) {
    if (n == 26) {
        Decipher::SBox(cipher_text.c_str(), buf, cipher_text.length(), stbl);
        Decipher::printCP(cipher_text.c_str(), buf, cipher_text.length(), 80);
        return;
    }

    int i = 0;
    if (candidate[n][0] == 0) {
        stbl[n] = '.';
        search_cipher(n + 1, cipher_text, candidate, stbl, buf);
    } else {
        while(candidate[n][i] != 0 && i < 26) {
            stbl[n] = candidate[n][i];
            i ++;
            search_cipher(n + 1, cipher_text, candidate, stbl, buf);
        }
    }
}

void solve() {
    int i, j;
    string str("BEEAKFYDJXUQYHYJIQRYHTYJIQFBQDUYJIIKFUHCQD");
    Decipher decoder1(str);
    printf("Solve problem 1.5:\n");
    decoder1.ShiftCipher(16);
    printf("----------\n\n");

    decoder1.reset(string("ETEGENLMDNTNEOORDAHATECOESAHLRMI"));
    printf("Solve problem 1.16:\n");
    int a[8] = {2, 4, 6, 1, 8, 3, 5, 7};
    for(int i = 0; i < 8; i ++) a[i] --;
    decoder1.TBoxCipher(a, 8);
    printf("----------\n\n");

    string cipher_text = "EMGLOSUDCGDNCUSWYSFHNSFCYKDPUMLWGYICOXYSIPJCKQPKUGKMGOLICGINCGACKSNISACYKZSCKXECJCKSHYSXCGOIDPKZCNKSHICGIWYGKKGKGOLDSILKGOIUSIGLEDSPWZUGFZCCNDGYYSFUSZCNXEOJNCGYEOWEUPXEZGACGNFGLKNSACIGOIYCKXCJUCIUZCFZCCNDGYYSFEUEKUZCSOCFZCCNCIACZEJNCSHFZEJZEGMXCYHCJUMGKUCY";
    decoder1.reset(cipher_text);
    printf("Solve problem 1.21(a):\n");
    ifstream candf("cand_21_1.txt");
    // make a substitution table
    char *stbl = new char[27]; stbl[26] = '\0';
    for(i = 0; i < 26; i ++) stbl[i] = '.';

    char *buf = new char[cipher_text.length() + 1]; buf[cipher_text.length()] = '\0';
    char candidate[26][26], c;

    for(i = 0; i < 26; i ++) for(j = 0; j < 26; j ++) candidate[i][j] = 0;

    while(true) {
        candf >> buf;
        c = buf[0];
        if (c == '!') break;
        if ('A' <= c && c <= 'Z') {
            int cnt = 0; char temp;
            while(true) {
                candf >> buf;
                temp = buf[0];
                if(temp == '-') break;
                candidate[c-'A'][cnt++] = temp;
            }
            i = 0;
        }
    }

    search_cipher(0, cipher_text, candidate, stbl, buf);
    delete [] stbl, buf;
    printf("----------\n\n");
}

void KasiskiTest(const string &s) {
    int i, j, l;
    string substr;
    for(l = 3; l <= 100; l ++) {
        printf("--%d--\n", l);
        for(i = 0; i < s.length() - l; i ++) {
            substr = s.substr(i, l);
            size_t pos = s.find(substr, i+1);
            if(pos < 1000) cout <<substr << " " << pos - i << endl;
        }
    }
}

int g_count[26];

double OverlapIndexAnalysis(const string &s) {
    memset(g_count, 0, sizeof(int) * 26);
    for(int i = 0; i < s.length(); i++) {
        g_count[s[i]-'A']++;
    }
    double res = 0, temp = 0;
    for(int i = 0; i < 26; i ++) {
        temp = (double)g_count[i] * g_count[i] / (double) (s.length() * s.length());
        res += temp;
    }
    return res;
}

void VigenereAnalysis(const string &s) {
    int m = 1, i, j;
    double res = 0, temp = 0;
    char *segment = new char[512];
    // enumerate segment
    for(m = 1; m < s.length(); m ++) {
        res = 0;
        for(i = 0; i < m; i ++) {
            for(j = i; j < s.length(); j += m)
                segment[(j-i)/m] = s[j];
            segment[(j-i)/m] = '\0';
            temp = OverlapIndexAnalysis(string(segment));
            res += (temp - 0.065) * (temp - 0.065);
        }
        res = sqrt(res) / m / 0.065;
        if(res < 0.04) printf("%lf %d\n", res, m);
    }
    delete [] segment;
}

void printSegment(const string &s, int m) {
    int i, j;
    for(i = 0; i < m; i ++) {
        for(j = i; j < s.length(); j += m) {
            printf("%c", s[j]);
        }
        printf("\n");
    }
}

int main() {
    int i, j;
    ifstream fin("data.txt");

    string cipher_text;
    fin >> cipher_text;

    KasiskiTest(cipher_text);
    printf("Vigenere Analysis:\n");
    VigenereAnalysis(cipher_text);
    // m = 10
    //printSegment(cipher_text, 10);
    Decipher decoder1(cipher_text);
    decoder1.VigenereCipher(string("BGMRMYXOLP"));
    decoder1.printCP();
    return 0;
}