/*
 * 加密接口类
 * 作者:康林
 */

#include "Encrypt.h"
#include "openssl/aes.h"
#include "Global/Global.h"

static const unsigned char Key[] =
{
    0x56, 0x9a, 0x28, 0x12, 0xfa, 0x59, 0xa1, 0x58,
    0x65, 0x7c, 0xc8, 0x92, 0x1a, 0x09, 0x10, 0x8a
};

CEncrypt::CEncrypt()
{
}

CEncrypt::~CEncrypt()
{
}

int CEncrypt::SetPassword(const char* pszPassword)
{
    int nLen = AES_BLOCK_SIZE;
    if(nLen > strlen(pszPassword))
        nLen = strlen(pszPassword);

    m_szPassword.resize(AES_BLOCK_SIZE);
    memcpy(&m_szPassword[0], Key, AES_BLOCK_SIZE);
    for(int i = 0; i < nLen; i++)
    {
        m_szPassword[i] ^= Key[i];
    }
        
    return 0;
}

int CEncrypt::Encode(const char* pIn, const int& inLen, char** pOut, int& outLen)
{
    int nRet = 0;
    
    AES_KEY key;
    nRet = AES_set_encrypt_key((const unsigned char*)m_szPassword.c_str(),  m_szPassword.size() * 8, &key);
    if(nRet < 0)
    {
        LOG_ERROR("Unable to set encryption key in AES:nRet=%d\n", nRet);
        return nRet;
    }
    
    char *pI = (char*)pIn;
    char *pO = NULL;
    int nBlock = inLen / AES_BLOCK_SIZE;
    int nFill = AES_BLOCK_SIZE - inLen % AES_BLOCK_SIZE;
    if(nFill)
        outLen = nBlock + 1;
    else
        outLen = nBlock;
    outLen *= AES_BLOCK_SIZE;
    pO = new char[outLen];
    if(!pO)
    {
        LOG_ERROR("Hasn't buffer\n");
        return -1;
    }
    
    *pOut = pO;
    
    for(int i = 0; i < nBlock; i++)
    {
        AES_encrypt((const unsigned char*)pI, (unsigned char*)pO, &key);
        pI += AES_BLOCK_SIZE;
        pO += AES_BLOCK_SIZE;
    }
    
    //最后一块
    if(nFill)
    {
        char buf[AES_BLOCK_SIZE];
        memset(buf, 0, AES_BLOCK_SIZE);
        memcpy(buf, pI, AES_BLOCK_SIZE);
        AES_encrypt((const unsigned char*)buf, (unsigned char*)pO, &key);
    }
    
    return nRet;
}

int CEncrypt::Dencode(const char* pIn, const int& inLen, char** pOut, int& outLen)
{
    int nRet = 0;
    AES_KEY key;
    nRet = AES_set_decrypt_key((const unsigned char*)m_szPassword.c_str(), m_szPassword.size() * 8, &key);
    if(nRet < 0)
    {
        LOG_ERROR("Unable to set dencryption key in AES:nRet=%d\n", nRet);
        return nRet;
    }
    
    char *pI = (char*)pIn;
    char *pO = NULL;
    outLen = inLen;
    pO = new char[outLen];
    if(!pO)
    {
        LOG_ERROR("Hasn't buffer\n");
        return -1;
    }
    
    memset(pO, 0, outLen);
    *pOut = pO;
    int nBlock = inLen / AES_BLOCK_SIZE;
    for(int i = 0; i < nBlock; i++)
    {
        AES_decrypt((const unsigned char*)pI, (unsigned char*)pO, &key);
        pI += AES_BLOCK_SIZE;
        pO += AES_BLOCK_SIZE;
    }
    return nRet;
}

int CEncrypt::Dencode(const char* pIn, const int& inLen, std::string& szOut)
{
    char *pOut = NULL;
    int nOutLen = 0;
    int nRet = 0;
    nRet = Dencode(pIn, inLen, &pOut, nOutLen);
    if(nRet)
    {
        LOG_ERROR("Dencode error\n");
        return nRet;
    }

    if(pOut)
    {
        szOut = pOut;
        delete [] pOut;
    }
    return nRet;
}



