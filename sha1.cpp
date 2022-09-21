#include <string.h>
#include "repeater.h"
#include "sha1.h"

#define SHA1_K0 0x5a827999
#define SHA1_K20 0x6ed9eba1
#define SHA1_K40 0x8f1bbcdc
#define SHA1_K60 0xca62c1d6

union _sha1_buffer {
  uint8_t b[BLOCK_LENGTH];
  uint32_t w[BLOCK_LENGTH/4];
} sha1_buffer;
union _sha1_state {
  uint8_t b[HASH_LENGTH];
  uint32_t w[HASH_LENGTH/4];
} sha1_state;



uint8_t sha1_InitState[] = {
  0x01,0x23,0x45,0x67, // H0
  0x89,0xab,0xcd,0xef, // H1
  0xfe,0xdc,0xba,0x98, // H2
  0x76,0x54,0x32,0x10, // H3
  0xf0,0xe1,0xd2,0xc3  // H4
};

uint8_t sha1_bufferOffset;
uint32_t sha1_byteCount;
uint8_t sha1_keyBuffer[BLOCK_LENGTH];
uint8_t sha1_innerHash[HASH_LENGTH];

void sha1_init(void) {
  memcpy(state.b, sha1_InitState, HASH_LENGTH);
  sha1_byteCount = 0;
  sha1_bufferOffset = 0;
}

uint32_t rol32(uint32_t number, uint8_t bits) {
  return ((number << bits) | (uint32_t)(number >> (32-bits)));
}

void sha1_hashblock(void) {
  uint8_t i;
  uint32_t a,b,c,d,e,t;

  a=state.w[0];
  b=state.w[1];
  c=state.w[2];
  d=state.w[3];
  e=state.w[4];
  for (i=0; i<80; i++) {
    if (i>=16) {
      t = sha1_buffer.w[(i+13)&15] ^ sha1_buffer.w[(i+8)&15] ^ sha1_buffer.w[(i+2)&15] ^ sha1_buffer.w[i&15];
      sha1_buffer.w[i&15] = rol32(t,1);
    }
    if (i<20) {
      t = (d ^ (b & (c ^ d))) + SHA1_K0;
    } else if (i<40) {
      t = (b ^ c ^ d) + SHA1_K20;
    } else if (i<60) {
      t = ((b & c) | (d & (b | c))) + SHA1_K40;
    } else {
      t = (b ^ c ^ d) + SHA1_K60;
    }
    t+=rol32(a,5) + e + sha1_buffer.w[i&15];
    e=d;
    d=c;
    c=rol32(b,30);
    b=a;
    a=t;
  }
  state.w[0] += a;
  state.w[1] += b;
  state.w[2] += c;
  state.w[3] += d;
  state.w[4] += e;
}

void addUncounted(uint8_t data) {
  sha1_buffer.b[bufferOffset ^ 3] = data;
  sha1_bufferOffset++;

  if (sha1_bufferOffset == BLOCK_LENGTH) {
     sha1_hashblock();
     sha1_bufferOffset = 0;
  }
}

void sha1_write(uint8_t data) {
  ++sha1_byteCount;
  sha1_addUncounted(data);

  return;
}

void sha1_writeArray(uint8_t *buffer, uint8_t size){
    while (size--) {
        sha1_write(*buffer++);
    }
}

void pad(void) {
  // Implement SHA-1 padding (fips180-2 ˜5.1.1)

  // Pad with 0x80 followed by 0x00 until the end of the block
  sha1_addUncounted(0x80);
  while (sha1_bufferOffset != 56) sha1_addUncounted(0x00);

  // Append length in the last 8 bytes
  sha1_addUncounted(0); // We're only using 32 bit lengths
  sha1_addUncounted(0); // But SHA-1 supports 64 bit lengths
  sha1_addUncounted(0); // So zero pad the top bits
  sha1_addUncounted(byteCount >> 29); // Shifting to multiply by 8
  sha1_addUncounted(byteCount >> 21); // as SHA-1 supports bitstreams as well as
  sha1_addUncounted(byteCount >> 13); // byte.
  sha1_addUncounted(byteCount >> 5);
  sha1_addUncounted(byteCount << 3);
}

uint8_t* sha1_result(void) {
  // Pad to complete the last block
  pad();

  // Swap byte order back
  uint8_t i;
  for (i=0; i<5; i++) {
    uint32_t a,b;
    a=state.w[i];
    b=a<<24;
    b|=(a<<8) & 0x00ff0000;
    b|=(a>>8) & 0x0000ff00;
    b|=a>>24;
    state.w[i]=b;
  }

  // Return pointer to hash (20 characters)
  return state.b;
}

#define HMAC_IPAD 0x36
#define HMAC_OPAD 0x5c

void sha1_initHmac(const uint8_t* key, uint8_t keyLength) {
  uint8_t i;
  memset(sha1_keyBuffer,0,BLOCK_LENGTH);
  if (keyLength > BLOCK_LENGTH) {
    // Hash long keys
    sha1_init();
    for (; keyLength--; )
       sha1_write(*key++);

    memcpy(sha1_keyBuffer, sha1_result(), HASH_LENGTH);
  } else {
    // Block length keys are used as is
    memcpy(sha1_keyBuffer, key, keyLength);
  }
  // Start inner hash
  sha1_init();
  for (i=0; i<BLOCK_LENGTH; i++) {
    sha1_write(keyBuffer[i] ^ HMAC_IPAD);
  }
}

uint8_t* sha1_resultHmac(void) {
  uint8_t i;
  // Complete inner hash
  memcpy(sha1_innerHash, sha1_result(), HASH_LENGTH);

  // Calculate outer hash
  sha1_init();

  for (i=0; i<BLOCK_LENGTH; i++)
     sha1_write(keyBuffer[i] ^ HMAC_OPAD);

  for (i=0; i<HASH_LENGTH; i++)
     sha1_write(innerHash[i]);

  return sha1_result();
}
