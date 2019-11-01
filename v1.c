#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <sys/time.h>
#include <stdlib.h>

#include <atca_status.h>
#include <atca_device.h>
#include <atca_command.h>
#include <atca_iface.h>
#include <atca_cfgs.h>
#include <atca_host.h>
#include <atca_execution.h>
#include <atca_basic.h>
#include <atca_helpers.h>

#define ATCAPRINTF

#define PUBLIC_KEY_SIZE 64
#define PRIVATE_KEY_SIZE 32
#define SIGNATURE_SIZE 64

#define __SUCCESS__

#include "v1.h"


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"



ATCA_STATUS v1_init()
{
	ATCAIfaceCfg *gCfg = &cfg_ateccx08a_i2c_default;
	ATCA_STATUS rv = ATCA_GEN_FAIL;
	rv = atcab_init(gCfg);

	return rv;
}
ATCA_STATUS  v1_close_i2c()
{
	return (atcab_release());
}
ATCA_STATUS  v1_get_random(uint8_t* rand_out , uint8_t randomLen)
{
	ATCAPacket packet;
    ATCACommand ca_cmd = _gDevice->mCommands;
    ATCA_STATUS status = ATCA_GEN_FAIL;

    do
    {
        // build an random command
        packet.param1 = RANDOM_SEED_UPDATE;
        packet.param2 = 0x0000;

        if ((status = atRandom(ca_cmd, &packet)) != ATCA_SUCCESS)
        {
            break;
        }

        if ((status = atca_execute_command(&packet, _gDevice)) != ATCA_SUCCESS)
        {
            break;
        }

        if (packet.data[ATCA_COUNT_IDX] != RANDOM_RSP_SIZE)
        {
            status = ATCA_RX_FAIL;
            break;
        }

        if (rand_out)
        {
            memcpy(rand_out, &packet.data[ATCA_RSP_DATA_IDX], RANDOM_NUM_SIZE);
        }
    }
    while (0);


    return status;
}

ATCA_STATUS  v1_get_pubkey(uint8_t index , uint8_t* publicKey)
{
	ATCA_STATUS rv = ATCA_BAD_PARAM;
	if (index != 0 && index != 2 && index != 7 )
	return ((atcab_read_pubkey(index,publicKey)));
	rv = atcab_get_pubkey( index, publicKey);
	return rv;
}

ATCA_STATUS  v1_sign(uint8_t index , const uint8_t *msg, uint8_t *pSignature)
{
	ATCA_STATUS rv = ATCA_BAD_PARAM;
	rv = atcab_sign( index, msg, pSignature);

	return rv;
}
ATCA_STATUS  v1_sign_raw(uint16_t key_id, const uint8_t *msg, uint16_t msglen, uint8_t *pSignature, uint16_t *pSignatureLen, uint8_t *rawSign)
{
	ATCA_STATUS rv = ATCA_BAD_PARAM;
	if(msglen < 32 && *pSignatureLen < SIGN_RSP_SIZE)
		rv = atcab_sign(key_id, msg, pSignature);

	return rv;
}

ATCA_STATUS  v1_generate_keypair(uint8_t index)
{
	ATCA_STATUS rv;
	uint8_t public_key[64] = {0};

	rv = atcab_genkey(index , public_key);

	if(rv == ATCA_SUCCESS)
	{
		printf("Public key for index %d = ",index);
		for (size_t i = 0; i < 64; i++)
		{
			printf("0x%02x ",public_key[i]);
		}
		puts("\n");
	}


	return rv;
}

ATCA_STATUS v1_verify(uint8_t index, const uint8_t *pHash,  const uint8_t *pSignature)
{
	ATCA_STATUS status = ATCA_GEN_FAIL;
    uint8_t nonce_target = NONCE_MODE_TARGET_TEMPKEY;
    uint8_t verify_source = VERIFY_MODE_SOURCE_TEMPKEY;

    if (pHash == NULL || pSignature == NULL)
    {
        return ATCA_BAD_PARAM;
    }

        // Load message into device
        if (_gDevice->mCommands->dt == ATECC608A)
        {
            // Use the Message Digest Buffer for the ATECC608A
            nonce_target = NONCE_MODE_TARGET_MSGDIGBUF;
            verify_source = VERIFY_MODE_SOURCE_MSGDIGBUF;
        }
        if ((status = atcab_nonce_load(nonce_target, pHash, 32)) != ATCA_SUCCESS)
        {
            return status;
        }

        status = atcab_verify(VERIFY_MODE_STORED | verify_source, index, pSignature, NULL, NULL, NULL);
		return status;


}

ATCA_STATUS  v1_verify_external(const uint8_t *message, const uint8_t *signature, const uint8_t *public_key)
{
    ATCA_STATUS status = ATCA_GEN_FAIL;
    uint8_t nonce_target = NONCE_MODE_TARGET_TEMPKEY;
    uint8_t verify_source = VERIFY_MODE_SOURCE_TEMPKEY;
 	bool is_verified = false;

    if (signature == NULL || message == NULL || public_key == NULL)
    {
        return ATCA_BAD_PARAM;
    }

    do
    {
        // Load message into device
        if (_gDevice->mCommands->dt == ATECC608A)
        {
            // Use the Message Digest Buffer for the ATECC608A
            nonce_target = NONCE_MODE_TARGET_MSGDIGBUF;
            verify_source = VERIFY_MODE_SOURCE_MSGDIGBUF;
        }
        if ((status = atcab_nonce_load(nonce_target, message, 32)) != ATCA_SUCCESS)
        {
            break;
        }

        status = atcab_verify(VERIFY_MODE_EXTERNAL | verify_source, VERIFY_KEY_P256, signature, public_key, NULL, NULL);
		return status;

    }
    while (0);

    return (status);
}

ATCA_STATUS v1_write_data(uint16_t dataOffset, uint8_t *data, uint16_t dataLen)
{
	ATCA_STATUS status = atcab_write_bytes_zone(ATCA_ZONE_DATA, 15, dataOffset, data, dataLen);
	return (status);

}

ATCA_STATUS v1_read_data(uint16_t dataOffset, uint8_t *data, uint16_t dataLen)
{
	ATCA_STATUS status = atcab_read_bytes_zone(ATCA_ZONE_DATA, 15, dataOffset, data, dataLen);
	return (status);

}

ATCA_STATUS v1_get_sha256(uint8_t* pMessage, uint16_t msgLen, uint8_t* sha, uint16_t*shaLen)
{
	ATCA_STATUS status;
	uint8_t *pMessageHolder = pMessage;
	status = atcab_sha_base(SHA_MODE_SHA256_START, 0, NULL, NULL, NULL);
	if (status != ATCA_SUCCESS)
		return status;
	if(msgLen <= 64)
	{
		status = atcab_sha_base(SHA_MODE_SHA256_UPDATE, msgLen, pMessageHolder, NULL, NULL);
		if (status != ATCA_SUCCESS)
			return status;
	}
	else
	{
		while (msgLen > 0)
		{
			if (msgLen >= 64)
			{
				status = atcab_sha_base(SHA_MODE_SHA256_UPDATE, 64, pMessageHolder, NULL, NULL);
				pMessageHolder = pMessageHolder + 64;
				msgLen = msgLen - 64;
				if (status != ATCA_SUCCESS)
					return status;
			}
			else
			{
				status = atcab_sha_base(SHA_MODE_SHA256_UPDATE, msgLen, pMessageHolder, NULL, NULL);
				pMessageHolder = pMessageHolder + msgLen;
				msgLen = 0;
				if (status != ATCA_SUCCESS)
					return status;
			}

		}
	}



    status = atcab_sha_base(SHA_MODE_SHA256_END | SHA_MODE_TARGET_OUT_ONLY, 0, NULL, sha, shaLen);
	return status;
}

ATCA_STATUS v1_save_key_pair(uint16_t slot, const uint8_t *public_key)
{
	ATCA_STATUS status = ATCA_SUCCESS;
	uint8_t public_key_formatted[ATCA_BLOCK_SIZE * 3];
	int block;

	// Check the pointers
	if (public_key == NULL)
	{
		return (ATCA_BAD_PARAM);
	}

	// The 64 byte P256 public key gets written to a 72 byte slot in the following pattern
	// | Block 1                     | Block 2                                      | Block 3       |
	// | Pad: 4 Bytes | PubKey[0:27] | PubKey[28:31] | Pad: 4 Bytes | PubKey[32:55] | PubKey[56:63] |

	memset(public_key_formatted, 0, sizeof(public_key_formatted));
	memcpy(&public_key_formatted[4], &public_key[0], 32);   // Move X to padded position
	memcpy(&public_key_formatted[40], &public_key[32], 32); // Move Y to padded position

	// Using this instead of atcab_write_zone_bytes, as that function doesn't work when
	// the data zone is unlocked
	for (block = 0; block < 3; block++)
	{
		status = atcab_write_zone(ATCA_ZONE_DATA, slot, block, 0, &public_key_formatted[ATCA_BLOCK_SIZE * block], ATCA_BLOCK_SIZE);
		if (status != ATCA_SUCCESS)
		{
			break;
		}
	}

	return (status);


}

#pragma GCC diagnostic pop
