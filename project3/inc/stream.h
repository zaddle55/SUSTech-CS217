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

typedef struct Stream_ {
    uint64_t ptr;
    uint64_t capacity;
    uint64_t size;
    byte* buffer; 
    bool has_closed;   
} IStream, OStream;

#define Stream(__s) { \
    .ptr = 0, \
    .capacity = BUFFER_SIZE, \
    .size = 0, \
    .buffer = NULL, \
    .has_closed = false \
}; \
__s.buffer = (byte *)malloc(BUFFER_SIZE * sizeof(byte)); \
if (__s.buffer == NULL) { \
    exit(1); \
} \

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
void OStream_close(OStream* ostream);

#endif