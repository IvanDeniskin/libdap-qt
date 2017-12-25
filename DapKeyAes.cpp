#include <QString>
#include "DapKeyAes.h"
#include <QDebug>
#include <stdio.h>
#include <stdlib.h>


void DapKeyAes::encode(QByteArray& dataIn, QByteArray& dataOut)
{
    size_t inputsLength = dataIn.length() + 1;
    const size_t encLength = ( (inputsLength + AES_BLOCK_SIZE)
                                / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;

    byte* in = (byte*) calloc(1, encLength);
    memcpy(in, dataIn.constData(), dataIn.size());

    unsigned char * data_out = new unsigned char[encLength];
    unsigned char *iv_enc = new unsigned char[sizeof(unsigned char) * AES_BLOCK_SIZE];

    memset(iv_enc, 0, sizeof(unsigned char) * AES_BLOCK_SIZE);

    AES_KEY enc_key;
    AES_set_encrypt_key(m_keyStr, AES_KEY_LENGTH, &enc_key);
    AES_cbc_encrypt(in,data_out, inputsLength, &enc_key,
                    iv_enc, AES_ENCRYPT);

    dataOut.clear();
    dataOut.append((const char*)data_out, encLength);

    free(in);
    delete [] iv_enc;
    delete [] data_out;
}


void DapKeyAes::decode(QByteArray& dataIn, QByteArray& dataOut)
{
    size_t inLength = dataIn.length();
    unsigned char *iv_dec = new unsigned char[sizeof(unsigned char) * AES_BLOCK_SIZE];
    unsigned char *out = new unsigned char[inLength];

    memset(iv_dec, 0, sizeof(unsigned char) * AES_BLOCK_SIZE);
    memset(out, 0, sizeof(unsigned char) * inLength);

    AES_KEY dec_key;
    AES_set_decrypt_key(m_keyStr, AES_KEY_LENGTH, &dec_key);
    AES_cbc_encrypt((unsigned char*)dataIn.constData(), out, dataIn.size(),
                    &dec_key,iv_dec, AES_DECRYPT);

    dataOut.clear();
    dataOut.append((const char*)out, dataIn.size());

    delete[] iv_dec;
    delete[] out;
}


bool DapKeyAes::init(const QString& str_key)
{
    m_keyStr = new unsigned char[AES_KEY_LENGTH];
    memcpy(m_keyStr, str_key.toStdString().c_str(), AES_KEY_LENGTH);
    return true;
}
