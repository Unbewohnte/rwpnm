/*
The MIT License (MIT)

Copyright © 2022 Kasyanov Nikolay Alexeyevich (Unbewohnte <me@unbewohnte.xyz>)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include<iostream>
#include<vector>
#include<fstream>


class Color {
public:
    Color() {}
    ~Color() {}
};

class RGB : public Color {
public:
    RGB(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0) {
        R = r;
        G = g;
        B = b;
    }
    ~RGB() {}

    uint8_t R;
    uint8_t G;
    uint8_t B;
};

class Grayscale : public Color {
public:
    Grayscale(uint8_t value = 0) {
        intensity = value;
    }
    ~Grayscale() {}

    uint8_t intensity;
};


// PPM file format reader/writer base class
class PPM {
protected:
    // PPM colored image magic number
    const std::string m_MAGIC_NUMBER = "P6";

    // image width and height
    uint32_t m_width;
    uint32_t m_height;

    // maximum value for all color channels
    uint16_t m_max_color;

    // actual pixel data
    std::vector<RGB> m_pixel_data;

    // returns 1-dimensional array index as if it was 2-dimensional array
    uint64_t index_at(uint32_t x, uint32_t y) {
        return m_width * y + x;
    }

public:
    PPM() {}
    ~PPM() {}

    // Color a pixel at coordinates (x, y)
    void put_pixel(uint32_t x, uint32_t y, RGB color) {
        uint64_t i = index_at(x, y);

        if (i >= (m_width * m_height))  {
            throw std::runtime_error("Pixel coordinates are out of bounds");
        }

        m_pixel_data.at(i) = color;
    }
};

// PPM file format image writer class
class PPM_writer : public PPM {
public:
    PPM_writer(uint32_t width, uint32_t height, uint16_t maxcolor = 255) {
        m_width = width;
        m_height = height;
        m_max_color = maxcolor;
        m_pixel_data.resize(width*height);
    }
    ~PPM_writer() {}

    // Write PPM image to disk
    void save(std::string path) {
        // check if there are less/more pixels than needed
        if (m_pixel_data.size() > (m_width * m_height) || m_pixel_data.size() < (m_width * m_height)) {
            std::cout << m_pixel_data.size();
            throw std::runtime_error("Invalid amount of pixels. There must be width*height pixels");
        }

        // try to create file
        std::ofstream ppm_image_file(path, std::ostream::out);
        if (!ppm_image_file.is_open()) {
            throw std::runtime_error("Could not create an image");
        }

        // write to the file on the disk and close it

        // magic number
        ppm_image_file << "P6";

        // newline
        ppm_image_file << "\n";

        // width, height, max color value
        ppm_image_file << m_width << " " << m_height << " " << m_max_color << "\n";

        // pixels
        for (const RGB& pixel_color : m_pixel_data) {
            ppm_image_file << (char) pixel_color.R << (char) pixel_color.G << pixel_color.B;
        }

        ppm_image_file.close();
    }
};


class PPM_reader : public PPM {
public:
    PPM_reader() {}
    ~PPM_reader() {}

    // Read PPM image from disk
    void read(std::string path) {
        // try to open file
        std::ifstream ppm_image_file;
        ppm_image_file.open(path, std::ios::in);
        if (!ppm_image_file.is_open()) {
            throw std::runtime_error("Could not open an image");
        }

        while(true) {
            std::string data;
            ppm_image_file >> data;
            if (data.length() == 0) {
                break;
            }
            std::cout << data;
        }
    }
};

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
    }

    return 0;
}
