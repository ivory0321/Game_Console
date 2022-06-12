#include <iostream>
#include "backend.h"
using namespace std;


//бсбр
void title() {
    //col = 50, col = 12
    cout<<"брбс"<<endl;
}


void loading_screen() {


}

int difficulty_screen() {

    return 0;
}


void start_game(int difficulty) {
    Backend backend;
    backend.do_this();
}


int main() {

    int difficulty;


    title();
    loading_screen();
    difficulty = difficulty_screen();
    start_game(difficulty);
    return 0;
}