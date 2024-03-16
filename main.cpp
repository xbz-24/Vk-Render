#include "app.hpp"

int main(){
    App* application = new App(640, 
                               480,
                               true);
    application -> run();
    delete application;
}
