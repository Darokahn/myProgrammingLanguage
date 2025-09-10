struct mystruct {
    int a;
    char* b;
    void(struct mystruct* s) printMe;
};

void (int a, char* b, struct mystruct* s) initMystruct = ${
    s->a = a;
    s->b = b;
    s->printMe = ${
        printf("%s, %d", s->b, s->a);
    };
}

int(void) main = ${
    struct mystruct s;
    s.printMe(&s);
}

// =========


struct mystruct {
    int a;
    char* b;
    void (*printMe)(struct mystruct* s);
};

void printMe(struct mystruct* s) {
    printf("%s, %d", s->b, s->a);
}

void initMystruct(int a, char* b, struct mystruct* s) {
    s->a = a;
    s->b = b;
    s->printMe = &printMe;
}

int main() {
    struct mystruct s;
    initMystruct(1, "hello", &s);
    s.printMe(&s);
}
