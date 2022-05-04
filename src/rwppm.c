/*
The MIT License (MIT)

Copyright © 2022 Kasyanov Nikolay Alexeyevich (Unbewohnte <me@unbewohnte.xyz>)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef RWPPM
#define RWPPM

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct rgb8 {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} rgb8;

// A very minimal ppm image
typedef struct ppm_image {
    unsigned int width;
    unsigned int height;
    rgb8* pixels;
} ppm_image;

// Read ppm image from file on the disk. Returns NULL if something went wrong
ppm_image* read_ppm(const char* path) {
    ppm_image* ppm;
    FILE* file;
    char magic_number[2];
    int read_bytes;
    int character;
    unsigned int max_rgb_value;
    rgb8* pixels;

    // open file
    file = fopen(path, "r");
    if (file == NULL) {
        return NULL;
    }

    // check if the magic number is in place
    read_bytes = fread(magic_number, 1, 2, file);
    if (read_bytes != 2 || magic_number[0] != 'P' || magic_number[1] != '6') {
        return NULL;
    }

    // skip until width, height and color information
    character = getc(file);
    while (character == '#') {
        while (getc(file) != '\n') {
            character = getc(file);
        }
    }
    ungetc(character, file);

    ppm = (ppm_image*) malloc(sizeof(ppm_image));
    if (!ppm) {
        free(ppm);
        return NULL;
    }

    // scan width, height, max color value
    if (fscanf(file, "%d %d %d", &ppm->width, &ppm->height, &max_rgb_value) != 3) {
        return NULL;
    }

    if (max_rgb_value > 255 || max_rgb_value == 0) {
        // can't read that one !
        return NULL;
    }

    // skip last newline before matrix of pixels
    getc(file);

    // read pixels
    pixels = (rgb8*) malloc(sizeof(rgb8) * ppm->width * ppm->height);
    if (!pixels) {
        free(pixels);
        return NULL;
    }

    read_bytes = fread(pixels, sizeof(rgb8) * ppm->width, ppm->height, file);
    if (read_bytes != ppm->height) {
        // here read_bytes is not actually bytes read, but
        // rather rows read 
        return NULL;
    }
    ppm->pixels = pixels;

    // close file
    fclose(file);

    return ppm;
}

// Write ppm image to the disk. Returns 0 if everything is alright, 1 otherwise
int write_ppm(const ppm_image* ppm, const char* path) {
    FILE* file;
    
    file = fopen(path, "w");
    if (!file) {
        return 1;
    }

    fprintf(file, "P6\n");
    fprintf(file, "%d %d %d\n", ppm->width, ppm->height, 255);
    fwrite(ppm->pixels, sizeof(rgb8) * ppm->width , ppm->height, file);

    fclose(file);

    return 0;
}

// Put pixel with specified rgb8 color at x,y coordinates. Returns 0 if pixel has been replaced, 1 if 
// coordinates are out of bounds
int put_pixel_at(const unsigned int x, const unsigned int y, const rgb8 color, ppm_image* ppm) {
    unsigned int index = ppm->width * y + x;
    if (index >= ppm->width*ppm->height) {
        // out of bounds !
        return 1;
    }

    ppm->pixels[index] = color;

    return 0;
}

// Get pixel color at specified coordinates. Returns a const pointer to that pixel color if 
// it is present, NULL if coordinates are out of bounds
const rgb8* get_pixel_at(const unsigned int x, const unsigned int y, ppm_image* ppm) {
    unsigned int index = ppm->width * y + x;
    if (index >= ppm->width*ppm->height) {
        // out of bounds !
        return NULL;
    }

    const rgb8* pixel_color = &ppm->pixels[index];

    return pixel_color;
}

// Create a new ppm image with specified dimensions. All pixels are {0, 0, 0} (black) by default
ppm_image* new_ppm_image(const unsigned int width, const unsigned int height) {
    ppm_image* ppm;
    rgb8* pixels;
    const rgb8 default_pixel_color = {0, 0, 0};
    
    ppm = (ppm_image*) malloc(sizeof(ppm_image));
    pixels = (rgb8*) malloc(sizeof(rgb8) * width * height);
    
    ppm->width = width;
    ppm->height = height;


    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            pixels[width * y + x] = default_pixel_color;
        }
    }
    ppm->pixels = pixels;

    return ppm;
}

#endif