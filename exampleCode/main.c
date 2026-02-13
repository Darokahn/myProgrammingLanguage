#include <stdint.h>
#include <stdio.h>

uint8_t prefixCodes[] = {
    0x0,
    0xc0,
    0xe0,
    0xf0
};

typedef struct {
    char* base;
    int length;
} utf8_t;

int addContinuation(uint8_t cont, int accumTotal) {
    accumTotal <<= 6;
    accumTotal += cont & 0x3F;
    return accumTotal;
}

int getInitial(uint8_t start, int length) {
    uint8_t code = prefixCodes[length-1];
    int initialVal = start & ~code;
    return initialVal;
}

int getByteCount(uint8_t prefix) {
    if (prefix == 0) return 0;
    uint8_t leading;
    int byteCount = 0;
    for (leading = prefix; (leading << byteCount) & 0x80; byteCount++);
    if (byteCount == 0) byteCount = 1;
    return byteCount;
}

utf8_t decodeGlyph_string(utf8_t* base) {
    utf8_t newString = *base;
    newString.length = getByteCount(newString.base[0]);
    return newString;
}
int decodeGlyph_int(utf8_t* base) {
    utf8_t newGlyph = decodeGlyph_string(base);
    int codePoint = getInitial(newGlyph.base[0], newGlyph.length);
    for (int i = 0; i < newGlyph.length - 1; i++) {
        codePoint = addContinuation(newGlyph.base[i + 1], codePoint);
    }
    return codePoint;
}

int main() {
    utf8_t myString = {"😎", 4};
    printf("0x%x\n", decodeGlyph_int(&myString));
}
