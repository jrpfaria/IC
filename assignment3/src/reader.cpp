#include <iostream>
#include "yuv_reader.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <file_name>" << std::endl;
        return 1;
    }

    yuv_reader image = yuv_reader(argv[1]);
    std::cout << "ok" << std::endl;

    return 0;
}