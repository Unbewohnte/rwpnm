#include "../src/rwppm.c"
#include <stdio.h>
#include <stdlib.h>

int main() {
    ppm_image* ppm = read_ppm("test_img512x512.ppm");
    if (ppm == NULL) {
        perror("could not read ppm image");
        return 1;
    }

    if (write_ppm(ppm, "result_image.ppm") != 0) {
        perror("could not write new ppm image");
        return 1;
    }

    free(ppm);

    ppm_image* new_ppm = new_ppm_image(200, 200);
    for(int y = 0; y < 200; y++) {
        for(int x = 0; x < 200; x++) {
            rgb8 pixel = {10 + x, 20 + x, 50 + y};
            put_pixel_at(x, y, pixel, new_ppm);
        } 
    }

    if (write_ppm(new_ppm, "result_image.ppm") != 0) {
        perror("could not write new ppm image");
        return 1;
    }

    free(new_ppm);

    return 0;
}