#include <stdint.h>
#define __SEA_V1__

#include "v1.h"


#include "sec_elem_abstr.h"




#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

#define PUB_KEY_LEN 128
#define RAND_NUM_LEN 128
#define MSG_LEN 128
#define SIG_LEN 128
#define PRINT_LOG

SE_STATUS se_configure_hardware(uint8_t  slave_address, uint8_t  bus, uint32_t baud, uint32_t pin_sda, uint32_t pin_scl)
{
#ifdef __SEA_V2__
	return 	return SE_UNKNOWN;
#elif defined __SEA_V3__
	return 	return SE_UNKNOWN;
#elif defined __SEA_V1__
	v1_configure(slave_address, bus, baud, pin_sda, pin_scl);
#endif

}


SE_STATUS se_init(uint8_t mode)
{
#ifdef __SEA_V2__
	return v2_init(mode);
#elif defined __SEA_V3__
	return v3_init();
#elif defined __SEA_V1__
	return v1_init();
#endif

}

SE_STATUS se_close(void)
{
#ifdef __SEA_V2__
  return v2_close_i2c();
#elif defined __SEA_V3__
  return v3_close_i2c();
#elif defined __SEA_V1__
  return v1_close_i2c();
#endif
}

SE_STATUS se_get_random(uint8_t* rand_out , uint8_t randomLen)
{
#ifdef __SEA_V2__
	return v2_get_random(rand_out,randomLen);
#elif defined __SEA_V1__
	return v1_get_random(rand_out,randomLen);
#elif defined __SEA_V3__
	return v3_get_random(rand_out,randomLen);
#else
	return SE_UNKNOWN;
#endif
}

SE_STATUS se_get_pubkey(uint8_t index , uint8_t* publicKey , uint16_t* publicKeyLen)
{
#ifdef __SEA_V2__
	return v2_get_pubkey(index,publicKey,publicKeyLen);
#elif defined __SEA_V1__
	return v1_get_pubkey(index,publicKey);
#elif defined __SEA_V3__
	return v3_get_pubkey(index,publicKey,publicKeyLen);
#else
	return SE_UNKNOWN;
#endif
}

SE_STATUS se_sign(uint8_t index , const uint8_t *msg, uint16_t msglen, uint8_t *pSignature, uint16_t *pSignatureLen)
{
#ifdef __SEA_V2__
	return v2_sign( index,msg, msglen, pSignature, pSignatureLen);
#elif defined __SEA_V1__
	return v1_sign( index ,msg, pSignature);
#elif defined __SEA_V3__
	return v3_sign( index, msg, msglen, pSignature, pSignatureLen);
#endif
}

SE_STATUS se_sign_raw(uint8_t index, const uint8_t *msg, uint16_t msglen, uint8_t *pSignature, uint16_t *pSignatureLen, uint8_t *rawSign)
{
#ifdef __SEA_V2__
	return v2_sign_raw( index, msg, msglen, pSignature, pSignatureLen, rawSign);
#elif defined __SEA_V1__
	return v1_sign_raw( index, msg, msglen, pSignature, pSignatureLen, rawSign);
#else
	return SE_UNKNOWN;
#endif
}



SE_STATUS se_generate_keypair(uint8_t index)
{
#ifdef __SEA_V2__
	return v2_generate_keypair( index );
#elif defined __SEA_V1__
	return v1_generate_keypair( index );
#elif defined __SEA_V3__
	return v3_generate_keypair( index );;
#endif
}

SE_STATUS se_save_key_pair(uint8_t index, const uint8_t *publicKey, uint16_t publicKeyLen, const uint8_t *privateKey, uint16_t privateKeyLen)
{
#ifdef __SEA_V2__
	return v2_save_key_pair( index, publicKey, publicKeyLen, privateKey, privateKeyLen);
#elif defined __SEA_V1__
	return v1_save_key_pair( index, publicKey);
#else
	return SE_UNKNOWN;
 #endif
}

SE_STATUS se_verify(uint8_t index, const uint8_t *pHash, uint16_t hashLen, const uint8_t *pSignature, uint16_t signatureLen)
{
#ifdef __SEA_V2__
	return v2_verify(index, pHash, hashLen, pSignature, signatureLen);
#elif defined __SEA_V1__
	return v1_verify(index, pHash , pSignature);
#elif defined __SEA_V3__
	return  v3_verify(index, pHash, hashLen, pSignature, signatureLen);
#endif
}

SE_STATUS se_verify_external(uint8_t index,const uint8_t *pKeyData, uint16_t keyDataLen, const uint8_t *pHash, uint16_t hashLen, const uint8_t *pSignature, uint16_t signatureLen)
{
#ifdef __SEA_V2__
	return v2_verify_external( index, pKeyData, keyDataLen, pHash, hashLen, pSignature, signatureLen );
#elif defined __SEA_V3__
	return v3_verify_external(pKeyData, keyDataLen, pHash, hashLen, pSignature, signatureLen);
#elif defined __SEA_V1__
	return v1_verify_external(pHash, pSignature, pKeyData);
 #endif
}

SE_STATUS se_write_data(uint16_t dataOffset, uint8_t *data, uint16_t dataLen)
{
#ifdef __SEA_V2__
	return v2_write_data( dataOffset, data, dataLen );
#elif defined __SEA_V3__
	return v3_write_data(dataOffset,data,dataLen);
#elif defined __SEA_V1__
	return v1_write_data(dataOffset,data,dataLen);
#endif
}

SE_STATUS se_read_data(uint16_t dataOffset, uint8_t *data, uint16_t dataLen)
{
#ifdef __SEA_V2__
	return v2_read_data(dataOffset,data,dataLen);
#elif defined __SEA_V3__
	return v3_read_data(dataOffset,data,dataLen);
#elif defined __SEA_V1__
	return v1_read_data(dataOffset,data,dataLen);
#endif
}

SE_STATUS se_get_sha256(uint8_t* pMessage, uint16_t msgLen, uint8_t* sha, uint16_t* shaLen)
{
#ifdef __SEA_V2__
	return v2_get_sha256(pMessage,msgLen,sha,shaLen);
#elif defined __SEA_V3__
	return v3_get_sha256(pMessage,msgLen,sha);
#elif defined __SEA_V1__
	return v1_get_sha256(pMessage,msgLen,sha,shaLen);
#endif
}
SE_STATUS se_wipe_device(uint8_t index)
{
#ifdef __SEA_V2__
	return v2_wipe_device(index);
#else
	return SE_UNKNOWN;
#endif
}



#pragma GCC diagnostic pop
