#include <cstring>
#include <cstdlib>

class N {
public:
    int value;
    char annotation[100];

    N(int v)
    {
        this->value = v;
    }

    void setAnnotation(char *str)
    {
        memcpy(this->annotation, str, strlen(str));
    }

    virtual int operator+(N &other)
    {
        return this->value + other.value;
    }

    virtual int operator-(N &other)
    {
        return this->value - other.value;
    }
};

int main(int argc, char **argv)
{
    if (argc <= 1)
        exit(1);

    N *a = new N(5);
    N *b = new N(6);

    a->setAnnotation(argv[1]);

    return *b + *a;
}