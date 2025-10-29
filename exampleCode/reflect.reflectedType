#include <fs/inodemeta.h> // prospective inode metadata reading library, assumedly using new syscall types.

struct simpleImage {
    int width;
    int height;
    int bytesPerPixel;
    int colorFormat;
};

REFLECTIONSTATUS parseFile(struct simpleImage* imageData) {
    char headerFormat[1024];
    int fd = open("myfile.bmp");
    readheaderformat(fd, headerFormat, 1024); // comes from `fs/inodemeta.h`, file should have a C-compatible declaration of its header info in inode metadata.
    reflected <headerFormat> header;
    read(fd, &header, sizeof header); // might have some weeds with alignment and padding.
    
    imageData->width = header.width;
    imageData->height = header.height;
    imageData->bytesPerPixel = header.bytesPerPixel;
    imageData->colorFormat = header.colorFormat;

    cleanup: // needs some way to make sure your cleanup happens before the function returns REFLECTIONSTATUS on failure. Maybe `cleanup` is a reserved label that reflected-using function always goes to when it fails?
    close(fd);
    // implicitly returns an "OK STATUS" REFLECTIONSTATUS
}

reflect struct someRandomStuff {
    char message[10];
    int timeOfDay;
    float hungerLevel;
    enum color favoriteColor;
};

reflect char*;

reflect int; // reflect can be applied to existing types

struct polyData {
    type t; // `type` is reserved, refers to the comptime-built enum for all `reflect` qualified types.
    void* data; // union is better here, but void* cause I'm lazy.
    // maybe there should be some way to tag 
}

int main() {
    struct polyData myArray[5];
    // this function will assumedly load myArray with some data of any of the above reflected types.
    // it will also put the type of each value in the `t` field of the struct.
    // type is gotten via `typeof`, which returns the automatically enumerated ID of the registered type.
    generateAbunchOfRandomData(&myArray, 5);
    for (int i = 0; i < 5; i++) {
        // layoutof returns a Datasegment-allocated string that represents the canonical C representation of that data.
        printf("The layout of this struct is: %s", layoutof(myArray[i].t)); 
        // rprintf understands how to map special format strings returned by `formatof` to pointers, assumedly pointing to that type. The format returned by `formatof` is mostly congruent with C printf formatting, but with some necessary extensions.
        rprintf(formatof(myArray[i].t), myArray[i].data); 
    }
}

// you definitely shouldn't do this, but maybe the pattern is valid when it can be extrapolated to other cases?
// it essentially auto-generates the checking code you would need to do to make a polymorphic `add` with type punning safe.
// some macros could let you write close-to-python syntax here.
REFLECTIONSTATUS add(struct polyData op1, struct polyData op2, int* out) {
    reflected <layoutof(op1.t)> i1;
    reflected <layoutof(op2.t)> i2;
    // assuming the reflected types became int, the interpreter will allow this. If they didn't, it'll return a REFLECTIONSTATUS that tells you why it couldn't complete the operation, with both an int status and a char* message.
    *out = i1 + i2;
}
