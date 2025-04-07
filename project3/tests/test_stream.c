#include "../inc/stream.h"
#include "../inc/test.h"

TEST(basic_io)
{
    FILE* fp = fopen("test_1.txt", "wb");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }
    OStream ostream = Stream(ostream);
    const word w = 0x7f36140b;
    OStream_writeWord(&ostream, w);
    OStream_toFileStream(&ostream, fp);
    OStream_close(&ostream);
    fclose(fp);

    fp = fopen("test_1.txt", "rb");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }
    IStream istream = Stream(istream);
    IStream_fromFileStream(&istream, fp);
    word rw = 0;
    uint64_t n = IStream_readWord(&istream, &rw);
    fprintf(stderr, "%ld\n", n);
    // assert(n == 4);
    IStream_close(&istream);
    printf("%x", rw);
    assert(rw == w);

    fclose(fp);
}

TEST(multiple_io)
{
    FILE* fp = fopen("test_2.txt", "wb");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }
    OStream ostream = Stream(ostream);
    const word w = 0x7f36140b;
    const dword dw = 0x0036140b7f36140b;
    OStream_writeWord(&ostream, w);
    OStream_writeDWord(&ostream, dw);
    OStream_writeWord(&ostream, w);
    OStream_toFileStream(&ostream, fp);
    OStream_close(&ostream);
    fclose(fp);

    fp = fopen("test_1.txt", "rb");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }
    IStream istream = Stream(istream);
    IStream_fromFileStream(&istream, fp);
    word rw = 0;
    uint64_t n = IStream_readWord(&istream, &rw);
    assert(rw == w);
    byte b = 0;
    uint64_t n1 = IStream_readByte(&istream, &b);
    assert(b = 0x0b);
    fprintf(stderr, "%ld\n", n);
    // assert(n == 4);
    IStream_close(&istream);
    printf("%x", rw);
    

    fclose(fp);
}

int main(void)
{
    run_all_tests();
    return 0;
}