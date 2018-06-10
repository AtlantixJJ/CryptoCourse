#include "aescrypto.h"
#include "bytevector.h"

int main(int argc, char* argv[])
{
    ByteVector output, input = ByteVector::from_file((const char*)argv[2]);

    AES128Crypto aes(argv[1]);
    aes.set_init_vector(ByteVector("1234567890123456"));

    if (argv[4][0] == '0') {
        printf("Encrpyt\n");
        output = aes.encrypt(input);
    } else {
        printf("Decrpyt\n");
        output = aes.decrypt(input);
    }

    output.save_to_file((const char*)argv[3]);

    return 0;
}
