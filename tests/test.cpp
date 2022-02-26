#include <iostream>
#include "../src/pnm.cpp"

using namespace pnm;

int main() {
    try {
        PPM_writer ppm_image(800, 800);
        
        uint8_t cc = 0;
        for (uint32_t y = 0; y < 800; y++) {
            for (uint32_t x = 0; x < 800; x++) {
                ppm_image.put_pixel(x, y, RGB(cc, cc, cc));

                cc++;
            }
            cc--;
        }
        ppm_image.save("image.ppm");

    } catch(const std::exception& e) {
        std::cout << "[ERROR] " << e.what() << "\n";
        return 1;
    }

    std::cout << "[SUCCESS]\n";

    return 0;
}
