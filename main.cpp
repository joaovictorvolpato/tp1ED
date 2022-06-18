#include <iostream>
#include <fstream>
#include <string>
#include "queue.h"

void run(char *arquivo);

int main() {
    char xmlfilename[100];

    std::cin >> xmlfilename;

    run(xmlfilename);

    return 0;
}