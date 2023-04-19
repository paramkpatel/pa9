/**
 * @AUTHOR: Param Patel
 * @FILE: 3d.c
 * @Instructor: Ben Dicken
 * @ASSIGNMENT: 3D - Programming Assignment 8
 * @COURSE: CSc 352; Spring 2023
 * @Purpose: This purpose of this file is to write 4/5 functions that make
 * using 3D objects that calculate the dimensions and show them accordingly.
 * Usage - run make
 * Usage - ./3d
 */

#include "3d.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

void Scene3D_add_triangle(Scene3D *pD, Triangle3D trtriangle3D);

void Scene3D_add_quadrilateral(Scene3D *scene, Coordinate3D a, Coordinate3D b, Coordinate3D c, Coordinate3D d);

void sphereHelper(Scene3D *scene, Coordinate3D origin, double radius, double theta, double phi, Coordinate3D *out);

double to_degrees(double in);

double round_double(double x, int digits);

/**
 * Helps create space and make new 3D Scene.
 * @return
 */
Scene3D *Scene3D_create()
{
    Scene3D *temp = calloc(1, (sizeof(Scene3D)));
    if (temp == NULL)
    {
        exit(0);
    }
    temp->count = 0;
    return temp;
}

/**
 * This function helps with freeing the memory on the heap.
 * @param scene
 */
void Scene3D_destroy(Scene3D *scene)
{
    for (Triangle3DNode *i = scene->root; i != NULL;)
    {
        Triangle3DNode *temp = i->next;
        free(i);
        i = temp;
    }
    free(scene);
}

/**
 * Writes the shapes to a .stl file format.
 * @param scene
 * @param file_name
 */
void Scene3D_write_stl_text(Scene3D *scene, char *file_name)
{
    FILE *fp = fopen(file_name, "w");
    if (fp == NULL)
    {
        fclose(fp);
        exit(1);
    }
    fprintf(fp, "solid scene\n");
    for (Triangle3DNode *i = scene->root; i != NULL; i = i->next)
    {
        fprintf(fp, "  facet normal 0.0 0.0 0.0\n");
        fprintf(fp, "    outer loop\n");
        fprintf(fp, "      vertex %.5f %.5f %.5f\n", i->triangle.a.x, i->triangle.a.y, i->triangle.a.z);
        fprintf(fp, "      vertex %.5f %.5f %.5f\n", i->triangle.b.x, i->triangle.b.y, i->triangle.b.z);
        fprintf(fp, "      vertex %.5f %.5f %.5f\n", i->triangle.c.x, i->triangle.c.y, i->triangle.c.z);
        fprintf(fp, "    endloop\n");
        fprintf(fp, "  endfacet\n");
    }
    fprintf(fp, "endsolid scene");
    fflush(fp);
    fclose(fp);
}

/**
 * This is a helper for Scene3D_add_pyramid() for its different direction
 * @param scene scene
 * @param origin origin
 * @param width width
 * @param height height
 * @param flag flag to know what direction
 */
void up_down(Scene3D *scene, Coordinate3D origin, double width, double height, bool flag)
{
    Coordinate3D angleA = {origin.x + width / 2, origin.y + width / 2, origin.z};
    Coordinate3D angleB = {origin.x + width / 2, origin.y - width / 2, origin.z};
    Coordinate3D angleC = {origin.x - width / 2, origin.y - width / 2, origin.z};
    Coordinate3D angleD = {origin.x - width / 2, origin.y + width / 2, origin.z};
    Coordinate3D angleE = {0, 0, 0};
    if (flag == true)
    {
        angleE = (Coordinate3D){origin.x, origin.y, origin.z + height};
    }
    else
    {
        angleE = (Coordinate3D){origin.x, origin.y, origin.z - height};
    }
    Triangle3D triangle_1 = (Triangle3D){angleA, angleB, angleE};
    Triangle3D triangle_2 = (Triangle3D){angleB, angleC, angleE};
    Triangle3D triangle_3 = (Triangle3D){angleC, angleD, angleE};
    Triangle3D triangle_4 = (Triangle3D){angleD, angleA, angleE};
    Scene3D_add_triangle(scene, triangle_1);
    Scene3D_add_triangle(scene, triangle_2);
    Scene3D_add_triangle(scene, triangle_3);
    Scene3D_add_triangle(scene, triangle_4);
    Scene3D_add_quadrilateral(scene, angleA, angleB, angleC, angleD);
}

/**
 * This is a helper for Scene3D_add_pyramid() for its different direction
 * @param scene scene
 * @param origin origin
 * @param width width
 * @param height height
 * @param flag flag to know what direction
 */
void right_left(Scene3D *scene, Coordinate3D origin, double width, double height, bool flag)
{
    Coordinate3D angleA = {origin.x, origin.y + width / 2, origin.z + width / 2};
    Coordinate3D angleB = {origin.x, origin.y - width / 2, origin.z + width / 2};
    Coordinate3D angleC = {origin.x, origin.y - width / 2, origin.z - width / 2};
    Coordinate3D angleD = {origin.x, origin.y + width / 2, origin.z - width / 2};
    Coordinate3D angleE = {0, 0, 0};
    if (flag == true)
    {
        angleE = (Coordinate3D){origin.x + height, origin.y, origin.z};
    }
    else
    {
        angleE = (Coordinate3D){origin.x - height, origin.y, origin.z};
    }
    Triangle3D triangle_1 = (Triangle3D){angleA, angleB, angleE};
    Triangle3D triangle_2 = (Triangle3D){angleB, angleC, angleE};
    Triangle3D triangle_3 = (Triangle3D){angleC, angleD, angleE};
    Triangle3D triangle_4 = (Triangle3D){angleD, angleA, angleE};
    Scene3D_add_triangle(scene, triangle_1);
    Scene3D_add_triangle(scene, triangle_2);
    Scene3D_add_triangle(scene, triangle_3);
    Scene3D_add_triangle(scene, triangle_4);
    Scene3D_add_quadrilateral(scene, angleA, angleB, angleC, angleD);
}

/**
 * This is a helper for Scene3D_add_pyramid() for it's different direction.
 * @param scene scene
 * @param origin origin
 * @param width width
 * @param height height
 * @param flag flag to know what direction
 */
void forward_backward(Scene3D *scene, Coordinate3D origin, double width, double height, bool flag)
{
    Coordinate3D angleA = {origin.x + width / 2, origin.y, origin.z + width / 2};
    Coordinate3D angleB = {origin.x - width / 2, origin.y, origin.z + width / 2};
    Coordinate3D angleC = {origin.x - width / 2, origin.y, origin.z - width / 2};
    Coordinate3D angleD = {origin.x + width / 2, origin.y, origin.z - width / 2};

    Coordinate3D angleE = {0, 0, 0};
    if (flag == true)
    {
        angleE = (Coordinate3D){origin.x, origin.y + height, origin.z};
    }
    else
    {
        angleE = (Coordinate3D){origin.x, origin.y - height, origin.z};
    }
    Triangle3D triangle_1 = (Triangle3D){angleA, angleB, angleE};
    Triangle3D triangle_2 = (Triangle3D){angleB, angleC, angleE};
    Triangle3D triangle_3 = (Triangle3D){angleC, angleD, angleE};
    Triangle3D triangle_4 = (Triangle3D){angleD, angleA, angleE};
    Scene3D_add_triangle(scene, triangle_1);
    Scene3D_add_triangle(scene, triangle_2);
    Scene3D_add_triangle(scene, triangle_3);
    Scene3D_add_triangle(scene, triangle_4);
    Scene3D_add_quadrilateral(scene, angleA, angleB, angleC, angleD);
}

/**
 * This is calls the helpers in all different directions.
 * @param scene scene
 * @param origin origin
 * @param width width
 * @param height height
 * @param flag flag to know what direction
 */
void Scene3D_add_pyramid(Scene3D *scene, Coordinate3D origin, double width, double height, char *orientation)
{
    if (strcmp(orientation, "up") == 0)
    {
        up_down(scene, origin, width, height, true);
    }
    else if (strcmp(orientation, "down") == 0)
    {
        up_down(scene, origin, width, height, false);
    }
    else if (strcmp(orientation, "right") == 0)
    {
        right_left(scene, origin, width, height, true);
    }
    else if (strcmp(orientation, "left") == 0)
    {
        right_left(scene, origin, width, height, false);
    }
    else if (strcmp(orientation, "forward") == 0)
    {
        forward_backward(scene, origin, width, height, true);
    }
    else if (strcmp(orientation, "backward") == 0)
    {
        forward_backward(scene, origin, width, height, false);
    }
}

/**
 * This is a helper for Scene3D_add_pyramid() for its different direction.
 * @param scene scene
 * @param origin origin
 * @param width width
 * @param height height
 * @param flag flag to know what direction
 */
void Scene3D_add_cuboid(Scene3D *scene, Coordinate3D origin, double width, double height, double depth)
{
    Coordinate3D a = {origin.x + width / 2, origin.y + height / 2, origin.z - depth / 2};
    Coordinate3D b = {origin.x + width / 2, origin.y - height / 2, origin.z - depth / 2};
    Coordinate3D c = {origin.x - width / 2, origin.y - height / 2, origin.z - depth / 2};
    Coordinate3D d = {origin.x - width / 2, origin.y + height / 2, origin.z - depth / 2};
    Coordinate3D e = {origin.x + width / 2, origin.y + height / 2, origin.z + depth / 2};
    Coordinate3D f = {origin.x + width / 2, origin.y - height / 2, origin.z + depth / 2};
    Coordinate3D g = {origin.x - width / 2, origin.y - height / 2, origin.z + depth / 2};
    Coordinate3D h = {origin.x - width / 2, origin.y + height / 2, origin.z + depth / 2};
    Scene3D_add_quadrilateral(scene, a, b, c, d);
    Scene3D_add_quadrilateral(scene, e, f, g, h);
    Scene3D_add_quadrilateral(scene, a, b, f, e);
    Scene3D_add_quadrilateral(scene, d, c, g, h);
    Scene3D_add_quadrilateral(scene, b, c, g, f);
    Scene3D_add_quadrilateral(scene, a, d, h, e);
}

/**
 * This was a given method in the Spec.
 * @param scene scene
 * @param a
 * @param b
 * @param c
 * @param d
 */
void Scene3D_add_quadrilateral(Scene3D *scene, Coordinate3D a, Coordinate3D b, Coordinate3D c, Coordinate3D d)
{
    Triangle3D triangle_1 = (Triangle3D){a, b, c};
    Triangle3D triangle_2 = (Triangle3D){b, c, d};
    Triangle3D triangle_3 = (Triangle3D){a, c, d};
    Triangle3D triangle_4 = (Triangle3D){a, b, d};
    Scene3D_add_triangle(scene, triangle_1);
    Scene3D_add_triangle(scene, triangle_2);
    Scene3D_add_triangle(scene, triangle_3);
    Scene3D_add_triangle(scene, triangle_4);
}

/**
 * Creates the Triangle object.
 * @param pD
 * @param triangle3D
 */
void Scene3D_add_triangle(Scene3D *pD, Triangle3D triangle3D)
{
    Triangle3DNode *new_node = (Triangle3DNode *)malloc(sizeof(Triangle3DNode));
    new_node->triangle = triangle3D;
    new_node->next = NULL;
    if (pD->root == NULL)
    {
        pD->root = new_node;
        pD->count = 1;
        return;
    }
    Triangle3DNode *current_node = pD->root;
    while (current_node->next != NULL)
    {
        current_node = current_node->next;
    }
    current_node->next = new_node;
    pD->count++;
}

void Scene3D_write_stl_binary(Scene3D *scene, char *file_name)
{
    FILE *fp = fopen(file_name, "wb");
    if (fp == NULL)
    {
        exit(0);
    }

    fflush(fp);
    fclose(fp);
}

void Scene3D_add_sphere(Scene3D *scene, Coordinate3D origin, double radius, double increment)
{
    Scene3D *sphere = malloc(1 * (sizeof(Scene3D)));
    sphere->count = 0;
    sphere->root = NULL;
    Coordinate3D a;
    Coordinate3D b;
    Coordinate3D c;
    Coordinate3D d;
    for (int phi = increment; phi <= 180; phi = phi + increment)
    {
        for (int theta = 0; theta < 360; theta = theta + increment)
        {
            sphereHelper(scene, origin, radius, theta, phi, &a);
            sphereHelper(scene, origin, radius, theta, phi - increment, &b);
            sphereHelper(scene, origin, radius, theta - increment, phi, &c);
            sphereHelper(scene, origin, radius, theta - increment, phi - increment, &d);
        }
    }
}

void sphereHelper(Scene3D *scene, Coordinate3D origin, double radius, double theta, double phi, Coordinate3D *out)
{
    theta = theta * (PI / 180);
    phi = phi * (PI / 180);
    (*out).x = round_double((radius * sin(theta) * cos(phi)) + origin.x, 4);
    (*out).y = round_double((radius * sin(theta) * sin(phi)) + origin.y, 4);
    (*out).z = round_double((radius * cos(theta)) + origin.z, 4);
}

double round_double(double x, int digits)
{
    double fac = pow(10, digits);
    double res = roundf(roundf(x * fac));
    res = res / fac;
    if (res == -0.0f)
    {
        res = 0.0f;
    }
    return res;
}

void Scene3D_add_fractal(Scene3D *scene, Coordinate3D origin, double size, int levels)
{
}
