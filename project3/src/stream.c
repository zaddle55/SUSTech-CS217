#include "../inc/error.h"
#include "../inc/stream.h"

void IStream_fromFileStream(IStream* istream, FILE* fp)
{
    if (!istream || !fp) return;
    if (istream->has_closed) {
        // TODO: error handle
        return;
    }
    fseek(fp, 0, SEEK_END);
    int64_t file_size = ftello(fp);
    if (file_size <= 0)
    {
        // TODO: error handle
        return;
    }
    rewind(fp);

    if(istream->capacity < file_size)
    {
        uint64_t capacity = IStream_dilate(istream, 
        ((uint64_t) file_size << 1) + 1);
        if (capacity == 0ULL)
        {
            // TODO: error handle
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
            // TODO: error handle
            return 0ULL;
        }
    } else {
        istream->buffer = (byte*)malloc(new_capacity);
        if (!istream->buffer) {
            // TODO: error handle
            return 0ULL;
        }
        istream->capacity = new_capacity;
    }

    return istream->capacity;
}

inline uint64_t IStream_availableBytes(const IStream *istream)
{
    if (istream->has_closed) {
        // TODO: error handle
        return 0ULL;
    }
    return istream->size - istream->ptr;
}

uint64_t IStream_readByte(IStream *istream, byte *buffer)
{
    if (!istream || !buffer || !istream->buffer) return 0ULL;
    if (IStream_availableBytes(istream) == 0ULL)
    {
        // TODO: error handle
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
        // TODO: error handle
        return;
    }
    uint64_t wbytes = fwrite(ostream->buffer, 1, ostream->ptr, fp);
    fflush(fp);
    if (wbytes != ostream->ptr) {
        // TODO: error handle
    }
}

uint64_t OStream_availableBytes(const OStream *ostream)
{
    return ostream->capacity - ostream->ptr;
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
        // TODO: error handle
        return 0ULL;
    }
    if (OStream_availableBytes(ostream) == 0ULL)
    {
        uint64_t capacity = OStream_dilate(ostream, (ostream->ptr<<1) + 1);
        if (capacity == 0ULL)
        {
            // TODO: error handle
            return 0ULL;
        }
    }
    ostream->buffer[ostream->ptr++] = byte;
    return 1ULL;
}

uint64_t OStream_writeBytes(OStream *ostream, const byte *bytes, uint64_t num_bytes)
{
    if (!ostream || !ostream->buffer || !bytes) return 0ULL;
    if (ostream->has_closed)
    {
        // TODO: error handle
        return 0ULL;
    }
    uint64_t available_bytes = OStream_availableBytes(ostream);
    if (available_bytes < num_bytes)
    {
        uint64_t capacity = OStream_dilate(ostream, (ostream->ptr<<1) + 1);
        if (capacity == 0ULL)
        {
            return 0ULL;
        }
        available_bytes = OStream_availableBytes(ostream);
    }
    uint64_t write_bytes = (available_bytes >= num_bytes)? num_bytes : available_bytes;
    memcpy(ostream->buffer + ostream->ptr, bytes, write_bytes);
    ostream->ptr += write_bytes;
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