/**
 * @AUTHOR: Param Patel
 * @FILE: generator.c
 * @Instructor: Ben Dicken
 * @ASSIGNMENT: 3D - Programming Assignment 9
 * @COURSE: CSc 352; Spring 2023
 * @Purpose: The purpose of the file to make the 2 shapes
 * that were implemented in the 3d.c for PA9.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "3d.h"

int main()
{
    Scene3D *out = Scene3D_create();
    Coordinate3D origin = (Coordinate3D){100, 100, 0};
    Scene3D_add_fractal(out, origin, 50, 6);
    Scene3D_write_stl_binary(out, "output.stl");
    Scene3D_destroy(out);
    // printf("debug\n");

    Scene3D *sp = Scene3D_create();
    double increments[] = {15, 10, 5, 36, 30, 20, 90, 60, 45};
    for (int i = 0; i < 3; i += 1)
    {
        for (int j = 0; j < 3; j += 1)
        {
            Coordinate3D origin = (Coordinate3D){j * 100, i * 100, 0};
            Scene3D_add_sphere(sp, origin, 45, increments[(i * 3) + j]);
        }
    }
    Scene3D_write_stl_binary(sp, "sphere.stl");
    Scene3D_destroy(sp);

    return 0;
}