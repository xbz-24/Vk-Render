#include "app.hpp"

int main()
{
    App* app = new App(640, 480, true);
    app -> run();
    delete app;
}
