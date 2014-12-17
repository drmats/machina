/**
 *  machina
 *
 *  Copyright (c) 2014, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#ifndef __MAIN_CPP_
#define __MAIN_CPP_ 1

#include "main.h"




/**
 *  Program entry-point.
 */
int main (int argc, char *argv[]) {
    auto machina = std::make_shared<machina::Machina>(argc, argv);
    machina->run();
    exit(EXIT_SUCCESS);
}




#endif
