#ifndef STREAM_H_
#define STREAM_H_
#include <bits/types/FILE.h>
#include <stdint.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>
#include <stdio.h>

typedef uint8_t byte;
typedef uint16_t hword;
typedef uint32_t word;
typedef uint64_t dword;

#define BUFFER_SIZE 4096

// Base64 encoding table
static const byte base64_table[] = {
    /* 'A' */ 65,  /* 'B' */ 66,  /* 'C' */ 67,  /* 'D' */ 68,
    /* 'E' */ 69,  /* 'F' */ 70,
    /* 'G' */ 71,  /* 'H' */ 72,  /* 'I' */ 73,  /* 'J' */ 74,
    /* 'K' */ 75,  /* 'L' */ 76,
    /* 'M' */ 77,  /* 'N' */ 78,  /* 'O' */ 79,  /* 'P' */ 80,
    /* 'Q' */ 81,  /* 'R' */ 82,
    /* 'S' */ 83,  /* 'T' */ 84,  /* 'U' */ 85,  /* 'V' */ 86,
    /* 'W' */ 87,  /* 'X' */ 88,
    /* 'Y' */ 89,  /* 'Z' */ 90,  /* 'a' */ 97,  /* 'b' */ 98,
    /* 'c' */ 99,  /* 'd' */ 100,
    /* 'e' */ 101, /* 'f' */ 102, /* 'g' */ 103, /* 'h' */ 104,
    /* 'i' */ 105, /* 'j' */ 106,
    /* 'k' */ 107, /* 'l' */ 108, /* 'm' */ 109, /* 'n' */ 110,
    /* 'o' */ 111, /* 'p' */ 112,
    /* 'q' */ 113, /* 'r' */ 114, /* 's' */ 115, /* 't' */ 116,
    /* 'u' */ 117, /* 'v' */ 118,
    /* 'w' */ 119, /* 'x' */ 120, /* 'y' */ 121, /* 'z' */ 122,
    /* '0' */ 48,  /* '1' */ 49,
    /* '2' */ 50,  /* '3' */ 51,  /* '4' */ 52,  /* '5' */ 53,
    /* '6' */ 54,  /* '7' */ 55,
    /* '8' */ 56,  /* '9' */ 57,  /* '+' */ 43,  /* '/' */ 47};

typedef struct Stream_ {
    uint64_t ptr;
    uint64_t capacity;
    uint64_t size;
    bool has_closed;
    byte *buffer;
} Stream, IStream, OStream;

#define Stream(__s)                                                            \
  {                                                                            \
      .ptr = 0,                                                                \
      .capacity = BUFFER_SIZE,                                                 \
      .size = 0,                                                               \
                                                                               \
      .has_closed = false,.buffer = NULL,                                       \
  };                                                                           \
  __s.buffer = (byte *)malloc(BUFFER_SIZE * sizeof(byte));                     \
  if (__s.buffer == NULL) {                                                    \
    exit(1);                                                                   \
  }

void Stream_toBase64(Stream *stream);
void IStream_fromFileStream(IStream* istream, FILE* fp);
uint64_t IStream_availableBytes(const IStream* istream);
uint64_t IStream_dilate(IStream* istream, uint64_t new_capacity);
uint64_t IStream_readByte(IStream* istream, byte* buffer);
uint64_t IStream_readBytes(IStream* istream, byte* buffer, uint64_t num_bytes);
uint64_t IStream_readHWord(IStream* istream, hword* buffer);
uint64_t IStream_readWord(IStream* istream, word* buffer);
uint64_t IStream_readDWord(IStream* istream, dword* buffer);
void IStream_close(IStream* istream);

void OStream_toFileStream(OStream* ostream, FILE* fp);
uint64_t OStream_availableBytes(const OStream *ostream);
uint64_t OStream_writeBytes(OStream* ostream, const byte* bytes, uint64_t num_bytes);
uint64_t OStream_dilate(OStream* ostream, uint64_t new_capacity);
uint64_t OStream_writeByte(OStream* ostream, const byte byte);
uint64_t OStream_writeHWord(OStream* ostream, const hword hword);
uint64_t OStream_writeWord(OStream* ostream, const word word);
uint64_t OStream_writeDWord(OStream* ostream, const dword dword);
void OStream_close(OStream *ostream);

#endif