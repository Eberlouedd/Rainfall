#include <cstring>
#include <cstdlib>

class N {
public:
    N(int32_t* arg1);
    void setAnnotation(const char* arg1);
    int32_t operator+(N& other);
    int32_t operator-(N& other);

private:
    void* _vtable_for_N;
    int32_t* _some_data;
};

N::N(int32_t* arg1) {
    _vtable_for_N = nullptr;
    _some_data = arg1 + 0x68;
}

void N::setAnnotation(const char* arg1) {
    char* annotationData;
    annotationData = new char[strlen(arg1) + 1];
    memcpy(annotationData, arg1, strlen(arg1) + 1);
}

int32_t N::operator+(N& other) {
    return *(this->_some_data) + *(other._some_data);
}

// Opérateur - : soustraction de deux objets N
int32_t N::operator-(N& other) {
    return *(this->_some_data) - *(other._some_data);
}

int32_t main(int32_t argc, char** argv, char** envp) {
    if (argc <= 1) {
        _exit(1);
    }

    int32_t* eax = new int32_t[0x6C / sizeof(int32_t)];
    N obj1(eax);

    int32_t* eax_1 = new int32_t[0x6C / sizeof(int32_t)];
    N obj2(eax_1);

    obj1.setAnnotation(argv[1]);

    int32_t result = obj1 + obj2;

    return result;
}