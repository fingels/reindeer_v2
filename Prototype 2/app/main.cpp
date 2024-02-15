#include <iostream>

#include <reindeer2/strings.hpp>
#include <reindeer2/utils.hpp>


using namespace std;

int main() {


    // Setup

    Color::Modifier blue(Color::FG_BLUE);
    Color::Modifier red(Color::FG_RED);
    Color::Modifier def(Color::FG_DEFAULT);

    SpaceSeparator facet(1); // 1 - don't delete when done
    cout.imbue(std::locale(std::locale(), &facet));
    
    // Main

    cout << blue << "Hello " << red << "World!" << def << endl;
    return 0;
}
