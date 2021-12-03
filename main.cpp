#include "program.h"

int main(int argc, char *argv[])
{
    Program devices(argc, argv);
    return devices.exec();
}
