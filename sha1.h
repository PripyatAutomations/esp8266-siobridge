#if	!defined(_SIOBRIDGE_sha1_h)
#define	_SIOBRIDGE_sha1_h

#include <inttypes.h>

#define HASH_LENGTH 20
#define BLOCK_LENGTH 64

extern void sha1_init(void);
extern void sha1_initHmac(const uint8_t* secret, uint8_t secretLength);
extern uint8_t* sha1_result(void);
extern uint8_t* sha1_resultHmac(void);
extern void sha1_write(uint8_t);
extern void sha1_writeArray(uint8_t *buffer, uint8_t size);

#endif
