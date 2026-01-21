#include <iostream>

class test {
private:
    int superProtectedValueThatsImpossibleToChange;

public:

    test(int inner) {
        superProtectedValueThatsImpossibleToChange = inner;
    }
    virtual void setInner(int nVal) final {
        superProtectedValueThatsImpossibleToChange = nVal;
    }
    virtual int getInner() final {
        return superProtectedValueThatsImpossibleToChange;
    }

};

struct trojanHorse {
    int garbage1;
    int garbage2;
    int inner;
};

class test2 : public test {
    public:

    test2(int inner): test(inner) {}

    /*
    virtual int getInner() {
        return superProtectedValueThatsImpossibleToChange;
    }
    */
    void badBehavior(int inner) {
        ((trojanHorse*) this)->inner = inner;
    }
};


int main() {
    test2 t(10);
    std::cout << t.getInner() << std::endl;
    t.badBehavior(1);
    std::cout << t.getInner() << std::endl;
}
