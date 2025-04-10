#include "../inc/error.h"
#include "../inc/stream.h"

size_t base64_size(size_t input_size) {
  return ((input_size + 2) / 3) * 4 + 1; // +1 for null terminator
}

void Stream_toBase64(Stream *stream) {
  if (!stream) {
    WARNING("Stream can't be null");
    return;
  }
  if (stream->has_closed) {
    WARNING("Stream has been closed!");
    return;
  }
  size_t i = 0, j = 0;
  byte a, b, c;

  /* allocate memory for output string */
  size_t base64_len = base64_size(stream->size);
  char *output = (char *)calloc(base64_len, sizeof(byte));
  if (!output) {
    WARNING("Fail to allocate memory for output string.");
    return;
  }

  for (; i < base64_len; i += 3, j += 4) {
    a = stream->buffer[i];
    b = (i + 1 < base64_len) ? stream->buffer[i + 1] : 0;
    c = (i + 2 < base64_len) ? stream->buffer[i + 2] : 0;

    word w = (word)a << 16 | (word)b << 8 | (word)c;

    byte mask = 0x3F;
    output[j] = base64_table[(w >> 18) & mask];
    output[j + 1] = base64_table[(w >> 12) & mask];
    output[j + 2] =
        (i + 1 < base64_len) ? base64_table[(w >> 6) & mask] : 61 /* '=' */;
    output[j + 3] = (i + 2 < base64_len) ? base64_table[w & mask] : 61 /* '=' */;
  }

  output[j] = 0;
  memcpy(stream->buffer, output, base64_len);
//   printf("%s", output);
  //   IStream_dilate(stream, base64_len);
  //   memcpy(ostream->buffer, output, base64_len);
//   ostream->size = base64_len;
//   ostream->capacity = base64_len;
}

void IStream_fromFileStream(IStream* istream, FILE* fp)
{
    if (!istream || !fp) return;
    if (istream->has_closed) {
      WARNING("Stream has been closed!");
      return;
    }
    fseek(fp, 0, SEEK_END);
    int64_t file_size = ftello(fp);
    if (file_size <= 0)
    {
        WARNING("The size of file must be positive, not %ld", file_size);
        return;
    }
    rewind(fp);

    if(istream->capacity < file_size)
    {
        uint64_t capacity = IStream_dilate(istream, 
        ((uint64_t) file_size << 1) + 1);
        if (capacity == 0ULL)
        {
            WARNING("The memory fails to reallocate!");
            return;
        }
    }

    istream->size = fread(istream->buffer, 1, file_size, fp);
    istream->ptr = 0;
    istream->has_closed = false;
}

uint64_t IStream_dilate(IStream *istream, uint64_t new_capacity)
{
    if (!istream) return 0ULL;
    if (istream->buffer) {
        byte* new_buffer = (byte*)realloc(istream->buffer, new_capacity);
        if (new_buffer) {
            istream->buffer = new_buffer;
            istream->capacity = new_capacity;
        } else {
          WARNING("The memory fails to reallocate!");
          return 0ULL;
        }
    } else {
        istream->buffer = (byte*)malloc(new_capacity);
        if (!istream->buffer) {
          WARNING("The memory fails to allocate!");
          return 0ULL;
        }
        istream->capacity = new_capacity;
    }

    return istream->capacity;
}

inline uint64_t IStream_availableBytes(const IStream *istream)
{
    if (istream->has_closed) {
      WARNING("Stream has been closed!");
      return 0ULL;
    }
    return istream->size - istream->ptr;
}

uint64_t IStream_readByte(IStream *istream, byte *buffer)
{
    if (!istream || !buffer || !istream->buffer) return 0ULL;
    if (IStream_availableBytes(istream) == 0ULL)
    {
        WARNING("No more bytes can be read from stream");
        return 0ULL;
    }
    *buffer = istream->buffer[istream->ptr++];
    return 1ULL;
}

uint64_t IStream_readBytes(IStream *istream, byte* buffer, uint64_t num_bytes)
{
    if (!istream || !buffer || !istream->buffer) return 0ULL;
    uint64_t available_bytes = IStream_availableBytes(istream);
    uint64_t read_bytes = (available_bytes >= num_bytes)? num_bytes : available_bytes;
    memcpy(buffer, istream->buffer + istream->ptr, read_bytes);
    istream->ptr += read_bytes;
    return read_bytes;
}

uint64_t IStream_readHWord(IStream *istream, hword *buffer)
{
    if (!istream || !buffer) return 0ULL;
    return IStream_readBytes(istream, (byte* )buffer, 2ULL);
}

uint64_t IStream_readWord(IStream *istream, word *buffer)
{
    if (!istream || !buffer) return 0ULL;
    return IStream_readBytes(istream, (byte* )buffer, 4ULL);
}

uint64_t IStream_readDWord(IStream *istream, dword *buffer)
{
    if (!istream || !buffer) return 0ULL;
    return IStream_readBytes(istream, (byte* )buffer, 8ULL);
}

void IStream_close(IStream *istream)
{
    if (!istream) return;
    istream->has_closed = true;
    if (istream->buffer != NULL)
    {
        free(istream->buffer);
        istream->buffer = NULL;
    }
}

void OStream_toFileStream(OStream *ostream, FILE *fp)
{
    if (!ostream || !fp) return;
    if (ostream->has_closed) {
      WARNING("Stream has been closed!");
      return;
    }
    uint64_t wbytes = fwrite(ostream->buffer, 1, ostream->size, fp);
    fflush(fp);
    if (wbytes != ostream->size) {
      WARNING("The actual bytes written in file stream is %zu, expected %zu!",
              wbytes, ostream->size);
    }
}

uint64_t OStream_availableBytes(const OStream *ostream)
{
    return ostream->capacity - ostream->size;
}

inline uint64_t OStream_dilate(OStream *ostream, uint64_t new_capacity)
{
    return IStream_dilate((IStream* ) ostream, new_capacity);
}

uint64_t OStream_writeByte(OStream *ostream, const byte byte)
{
    if (!ostream || !ostream->buffer) return 0ULL;
    if (ostream->has_closed)
    {
      WARNING("Stream has been closed!");
      return 0ULL;
    }
    if (OStream_availableBytes(ostream) == 0ULL)
    {
        uint64_t capacity = OStream_dilate(ostream, (ostream->size<<1) + 1);
        if (capacity == 0ULL)
        {
            WARNING("Failed to dilate new capacity!");
            return 0ULL;
        }
    }
    ostream->buffer[ostream->ptr++] = byte;
    ostream->size = ostream->ptr;
    return 1ULL;
}

uint64_t OStream_writeBytes(OStream *ostream, const byte *bytes, uint64_t num_bytes)
{
    if (!ostream || !ostream->buffer || !bytes) return 0ULL;
    if (ostream->has_closed)
    {
        WARNING("Stream has been closed!");
        return 0ULL;
    }
    uint64_t available_bytes = OStream_availableBytes(ostream);
    if (available_bytes < num_bytes)
    {
        uint64_t capacity = OStream_dilate(ostream, (ostream->size<<1) + 1);
        if (capacity == 0ULL)
        {
            WARNING("The memory fails to reallocate!");
            return 0ULL;
        }
        available_bytes = OStream_availableBytes(ostream);
    }
    uint64_t write_bytes = (available_bytes >= num_bytes)? num_bytes : available_bytes;
    memcpy(ostream->buffer + ostream->ptr, bytes, write_bytes);
    ostream->ptr += write_bytes;
    ostream->size = ostream->ptr;
    return write_bytes; 
}

uint64_t OStream_writeHWord(OStream *ostream, const hword hword)
{
    if (!ostream) return 0ULL;
    return OStream_writeBytes(ostream, (byte* )&hword, 2ULL);
}

uint64_t OStream_writeWord(OStream *ostream, const word word)
{
    if (!ostream) return 0ULL;
    return OStream_writeBytes(ostream, (byte* )&word, 4ULL);
}

uint64_t OStream_writeDWord(OStream *ostream, const dword dword)
{
    if (!ostream) return 0ULL;
    return OStream_writeBytes(ostream, (byte* )&dword, 8ULL);
}

void OStream_close(OStream *ostream)
{
    if (!ostream) return;
    ostream->has_closed = true;
    if (ostream->buffer)
    {
        free(ostream->buffer);
        ostream->buffer = NULL;
    }
}