
#include "Application.h"
#undef main

int main(int argc, char* argv[])
{

    Application app(800, 600);
    app.run();
    return 0;
}