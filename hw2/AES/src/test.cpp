#include <ctime>
#include <string>
#include <assert.h>

#include "aescrypto.h"
#include "bytevector.h"
using namespace std;

void testcase_correct()
{
    const char input[] = "sfglskdfhklsdfghknsdfkbhsndkfbnksdfnb";
    const char key_128[] = "2333456";

    ByteVector b(key_128);
    AES128Crypto aes128(b);

    ByteVector cipher128 = aes128.encrypt(ByteVector(input));
    ByteVector dec128 = aes128.decrypt(ByteVector::from_hex_string(cipher128.to_hex_string().c_str()));

    printf("plain: %s\n", ByteVector(input).to_plain_text().c_str());

    printf("cipher(128): %s\n", cipher128.to_hex_string().c_str());

    printf("decrypted(128): %s\n", dec128.to_plain_text().c_str());
    assert(!strcmp(dec128.to_plain_text().c_str(), "sfglskdfhklsdfghknsdfkbhsndkfbnksdfnb"));
}

void testcase_speed()
{
    const int round = 5;
    const string fileName = "huge_file";
    const char key_128[] = "asdlbndifngilasisandig";

    ByteVector b(key_128);
    AES128Crypto aes128(b);
    ByteVector input = ByteVector::from_file(fileName.c_str());
    ByteVector output = input;

    long long size = 0;
    int begin = clock();
    for (int i = 0; i < round; i++)
    {
        size += output.length();
        output = aes128.encrypt(output);
    }

    for (int i = 0; i < round; i++)
    {
        size += output.length();
        output = aes128.decrypt(output);
    }

    int end = clock();
    double timeUsed = (1.0 * (end - begin) / CLOCKS_PER_SEC);
    printf("Time: %.3lf s\n", timeUsed);
    printf("Speed: %.3lf MB/s\n", 1.0 * size / (1 << 20) / timeUsed);

    assert(input == output);
}

int main()
{
    void (*testcases[])() = {
        testcase_correct,
        testcase_speed,
    };

    for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++)
    {
        int begin = clock();
        testcases[i]();
        int end = clock();
        printf("testcase #%d passed. (%.3lfs)\n", i, 1.0 * (end - begin) / CLOCKS_PER_SEC);
    }
    return 0;
}
