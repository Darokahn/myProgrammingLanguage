#include <fs/inodemeta.h>

struct simpleImage {
    int width;
    int height;
    int bytesPerPixel;
    int colorFormat;
};

REFLECTIONSTATUS parseFile(struct simpleImage* imageData) {
    char headerFormat[1024];
    int fd = open("myfile.bmp");
    readheaderformat(fd, headerFormat, 1024);
    reflected <headerFormat> header;
    read(fd, &header, sizeof header);
    
    imageData->width = header.width;
    imageData->height = header.height;
    imageData->bytesPerPixel = header.bytesPerPixel;
    imageData->colorFormat = header.colorFormat;

    cleanup:
    close(fd);
}

reflect struct someRandomStuff {
    char message[10];
    int timeOfDay;
    float hungerLevel;
    enum color favoriteColor;
};

reflect char*;

reflect int;

struct polyData {
    type t;
    void* data;
};

int main() {
    struct polyData myArray[5];
    generateAbunchOfRandomData(&myArray, 5);
    for (int i = 0; i < 5; i++) {
        printf("The layout of this struct is: %s", layoutof(myArray[i].t));
        rprintf(formatof(myArray[i].t), myArray[i].data);
    }
}

REFLECTIONSTATUS add(struct polyData op1, struct polyData op2, int* out) {
    reflected <layoutof(op1.t)> i1;
    reflected <layoutof(op2.t)> i2;
    *out = i1 + i2;
}
