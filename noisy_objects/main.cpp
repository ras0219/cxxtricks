#include <iostream>
#include <memory>

using namespace std;

struct NoisyObject
{
    NoisyObject();
    NoisyObject(const NoisyObject& o);
    NoisyObject(NoisyObject&& o);

    ~NoisyObject();

    int x;
    std::unique_ptr<char> buf;
};

NoisyObject::NoisyObject() : x(0), buf((char*)malloc(1024)) {
    wcout << "Constructed NoisyObject()" << endl;
}

NoisyObject::NoisyObject(const NoisyObject& o) : x(o.x), buf(new char[1024]) {
    memcpy(buf.get(), o.buf.get(), 1024);
    wcout << "Copy constructed NoisyObject()" << endl;
}

NoisyObject::NoisyObject(NoisyObject&& o) : x(o.x) {
    std::swap(buf, o.buf);
    wcout << "Move constructed NoisyObject()" << endl;
}

NoisyObject::~NoisyObject() {
    wcout << "Destructed NoisyObject()" << endl;
}

void foo(NoisyObject o) {
    wcout << "Value: " << o.x << endl;
}

void foo(NoisyObject* o) {
    wcout << "Value*: " << o->x << endl;
}

void fooR(NoisyObject& o) {
    wcout << "Value&: " << o.x << endl;
}
void fooR(NoisyObject&& o) {
    wcout << "Value&&: " << o.x << endl;
}

int main()
{
    {
        NoisyObject o;
        //NoisyObject o2 = o;

        fooR(o);
        fooR(NoisyObject());
    }

    wcin.get();
}