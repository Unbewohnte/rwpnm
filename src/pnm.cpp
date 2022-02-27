/*
The MIT License (MIT)

Copyright © 2022 Kasyanov Nikolay Alexeyevich (Unbewohnte <me@unbewohnte.xyz>)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#ifndef RWPNM
#define RWPNM

/*
RWPNM v0.1.0

A drop-in library to work with PNM images
*/

#include <vector>
#include <fstream>

namespace pnm {

class Color {
public:
    Color() {}
    ~Color() {}

    bool operator==(const Color& other);
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


    bool operator==(const RGB& other) {
        if (R != other.R || G != other.G || B != other.B) {
            return false;
        }

        return true;
    }
};

class Grayscale : public Color {
public:
    Grayscale(uint8_t value = 0) {
        intensity = value;
    }
    ~Grayscale() {}

    uint8_t intensity;
};


// PPM image file format reader/writer
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
    PPM() {
        // set defaults (these can and will be changed when reading/coloring the image)
        m_width = 0;
        m_height = 0;
        m_max_color = 255;
    }
    ~PPM() {}

    // Color a pixel at coordinates (x, y)
    void put_pixel(uint32_t x, uint32_t y, RGB color) {
        uint64_t i = index_at(x, y);
        if (i >= m_pixel_data.size()) {
            // the image is not big enough !
            m_pixel_data.resize(i + 1);

            if (x > m_width) {
                m_width = x + 1;
            }

            if (y > m_height) {
                m_height = y + 1;
            }
        }
        

        m_pixel_data.at(i) = color;
    }

    // Read PPM image from disk
    void read(std::string path) {
        // try to open file
        std::ifstream ppm_image_file;
        ppm_image_file.open(path, std::ios::in);
        if (!ppm_image_file.is_open()) {
            throw std::runtime_error("Could not open an image");
        }

        // check for magic number
        std::string entity;
        ppm_image_file >> entity;
        if (entity != m_MAGIC_NUMBER) {
            throw std::runtime_error("Does not have a magic number");
        }

        // width, height, max color
        ppm_image_file >> entity;
        m_width = std::stoi(entity);

        ppm_image_file >> entity;
        m_height = std::stoi(entity);

        ppm_image_file >> entity;
        m_max_color = std::stoi(entity);

        m_pixel_data.resize(m_width * m_height);

        // skip one byte
        char one[1];
        ppm_image_file.read(one, 1);

        // retrieve pixels
        char pixel_color[3];
        for (uint64_t i = 0; i < m_width * m_height; i++) {
            ppm_image_file.read(pixel_color, 3);            
            RGB color(pixel_color[0], pixel_color[1], pixel_color[2]);
            if (color == RGB(255, 0, 0)) {
                break;
            }
            m_pixel_data.at(i) = color;
        }


        ppm_image_file.close();
    }

    // Write PPM image to disk
    void save(std::string path) {
        // check if there are pixels at all
        if (m_pixel_data.size() == 0) {
            throw std::runtime_error("No pixels were assigned");
        }

        // try to create file
        std::ofstream ppm_image_file(path, std::ostream::out);
        if (!ppm_image_file.is_open()) {
            throw std::runtime_error("Could not create an image");
        }

        // write to the file on the disk and close it

        // magic number
        ppm_image_file << m_MAGIC_NUMBER;

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

}

#endif