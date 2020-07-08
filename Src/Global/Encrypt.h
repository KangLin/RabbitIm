/*
 * 加密接口类
 * 作者:康林
 * 用法:
 *         本类可以单独使用,它信赖c++标准库,openssl库(-lcrypto)  
 */

#ifndef CENCRYPT_H
#define CENCRYPT_H
#include <string>

class CEncrypt
{
public:
    CEncrypt();
    ~CEncrypt();

    int SetPassword(const char* pszPassword);
    //pIn:输入要加密的数据指针
    //inlen:输入要加密的数据的长度
    //pOut:输出加密了的数据指针.调用者使用完成后,需要用delete []*pOut
    //outLen:加密了的数据长度
    int Encode(const char* pIn, const int &inLen, char** pOut, int &outLen);
    //pIn:输入要解密的数据指针
    //inlen:输入要解密的数据的长度
    //pOut:输出解密了的数据指针.调用者使用完成后,需要用delete []*pOut
    //outLen:解密了的数据长度
    int Dencode(const char* pIn, const int &inLen, char** pOut, int &outLen);
    int Dencode(const char* pIn, const int &inLen, std::string &szOut);
    
private:
    std::string m_szPassword;
};

#endif // CENCRYPT_H

