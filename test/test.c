#include <stdio.h>
#include "../home_easy.h"
#include "../buffer.h"

#define TEST_START printf("\n=== %s ===\n", __FUNCTION__)


void testByteBuffer()
{
    BYTE_BUFFER buffer;
    char bytes[] = {0xcd, 0xef, 0x12};
    TEST_START;

    buffer = createByteBuffer();

    pushByte(&buffer, 0xab);
    pushBytes(&buffer, bytes, 3);
    pushWord(&buffer, 0x3456);

    printf("Should: AB CD EF 12 34 56\nGet: ");
    printfByteBuffer(buffer);

    destroyByteBuffer(buffer);
}

void testPrintBits()
{
    BYTE_BUFFER buffer;
    char bytes[] = {0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0};
    TEST_START;

    buffer = createByteBuffer();
    pushBytes(&buffer, bytes, 8);

    printfBitBuffer(buffer);

    destroyByteBuffer(buffer);
}

void testGetId()
{
    unsigned long int id;
    unsigned long int frame = 0x2829019a;
    TEST_START;

    id =  getHomeEasyId(frame);
    printf("Should 00A0A406\nGet %08X\n", id);
}

void testEncode()
{
    unsigned char srcByte = 0x28;
    unsigned short int encWord;
    TEST_START;

    encWord = encodeByte(srcByte);
    printf("Input: %02X\nShould: 0x5995\nGet: %04X\n", srcByte, encWord);
}


void testDecode()
{
    unsigned short int srcWord = 0x5995;
    unsigned char decByte;
    TEST_START;

    decByte = decodeByte(srcWord);
    printf("Input: %04X\nShould: 0x28\nGet: %02X\n", srcWord, decByte);
}

void testHomeEasyEncode()
{
    BYTE_BUFFER source;
    BYTE_BUFFER encoded;
    unsigned long int frame = 0x2829019a;
    TEST_START;

    printf("Input: %08X\n", frame);
    printf("Should: 59 95 59 95 55 56 96 99\nGet:    ");

    encoded = homeEasyEncode(frame);
    printfByteBuffer(encoded);

    destroyByteBuffer(encoded);
}

void testHomeEasyDecode()
{
    BYTE_BUFFER source;
    unsigned long int decoded;
    unsigned char bytes[] = {0x59, 0x95, 0x59, 0x96, 0x55, 0x56, 0x9A, 0x55};
    TEST_START;

    source = createByteBuffer();
    pushBytes(&source, bytes, 8);
    printf("Input: ");
    printfByteBuffer(source);
    printf("Should: 282901B0\nGet:    %08X\n", homeEasyDecode(&source));

    destroyByteBuffer(source);
}


void testHomeEasyCommand(char section, unsigned char num, unsigned char onOff)
{
    unsigned long int command;
    BYTE_BUFFER encoded;
    unsigned long int id = getHomeEasyId(0x2829019a);
    TEST_START;

    command = createHomeEasyCommand(id, section, num, onOff);
    printf("%08X\n", command);

    encoded = homeEasyEncode(command);
    printfByteBuffer(encoded);

    destroyByteBuffer(encoded);
}

int main()
{
    printf("Test\n");

    testByteBuffer();

    testPrintBits();
    
    testGetId();

    testEncode();

    testDecode();

    testHomeEasyEncode();

    testHomeEasyDecode();

    testHomeEasyCommand('D', 4, OFF);

    return 0;
}
