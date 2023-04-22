/**
 * @AUTHOR: Param Patel
 * @FILE: generator.c
 * @Instructor: Ben Dicken
 * @ASSIGNMENT: 3D - Programming Assignment 9
 * @COURSE: CSc 352; Spring 2023
 * @Purpose:
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "3d.h"

int main()
{
    Scene3D* s = Scene3D_create();
    Coordinate3D origin = (Coordinate3D){100, 100, 0};
    Scene3D_add_fractal(s,origin, 50, 5 );
    Scene3D_write_stl_binary(s, "bin.stl");
    Scene3D_destroy(s);
    // printf("check here\n");
    return 0;
}