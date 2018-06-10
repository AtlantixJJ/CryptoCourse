#ifndef _AES_H
#define _AES_H

#include "bytevector.h"

class AES128Crypto
{
public:
    AES128Crypto(const ByteVector& key);
    virtual ~AES128Crypto();

    void set_init_vector(const ByteVector& iv) { m_iv = iv;}
    ByteVector encrypt(const ByteVector& text) const;
    ByteVector decrypt(const ByteVector& cipher) const;

private:
    void encrypt_single_block(uint8_t* state) const;
    void decrypt_single_block(uint8_t* state) const;
    int  valid_pad(const ByteVector& text) const;
    void add_round_key(uint8_t* state, const uint32_t* roundKey) const;
    void sub_byte(uint8_t* state) const;
    void inv_sub_byte(uint8_t* state) const;
    void expand_key(const ByteVector& key);
    void shift_row(uint8_t* state) const;
    void inv_shift_row(uint8_t* state) const;
    void mix_col(uint8_t* state) const;
    void inv_mix_col(uint8_t* state) const;

private:
    static const uint32_t RCON[10];
    static const uint8_t SBOX[16][16];
    static const uint8_t ISBOX[16][16];

    int Nk, Nb, Nr, m_block_bytes;
    uint32_t* m_w;
    ByteVector m_iv;
};

#endif // _AES_H
