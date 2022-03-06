#include <iostream>
#include "../src/pnm.cpp"


void make_test_img() {
    try {
        pnm::PPM image;
        
        const uint32_t width = 512;
        const uint32_t height = 512;

        for (uint32_t y = 0; y < height; y++) {
            for (uint32_t x = 0; x < width; x++) {                
                image.put_pixel(x, y, pnm::RGB(x / 2, y / 2, x / 2 + y / 2));
            }
        }

        image.save("test_img512x512.ppm");
    } catch(const std::exception& e) {
        std::string error_message("make_test_img: ", e.what());
        throw std::runtime_error(error_message);
    }
}

void green_rectangle_on_top_of_existing_image() {
    try {
        pnm::PPM image;
        image.read("test_img512x512.ppm");
        // image.read("img.ppm");

        uint32_t rec_width = 100;
        uint32_t rec_height = 100;
        
        for (uint32_t y = 0; y < rec_height; y++) {
            for (uint32_t x = 0; x < rec_width; x++) {
                image.put_pixel(x, y, pnm::RGB(0, 255, 0));
            }
        }

        image.save("result_image.ppm");
    } catch(const std::exception& e) {
        std::string error_message("green_rectangle_on_top_of_existing_image: ", e.what());
        throw std::runtime_error(error_message);
    }
}

void no_pixel_assign() {
    try {
        pnm::PPM image;
        image.save("result_image.ppm");

        std::string error_message("no_pixel_assign: should`ve gotten an error, but there weren`t any thrown");
        throw std::runtime_error(error_message);
    } catch(const std::exception& e) {}
}

void write_comment() {
    try {
        pnm::PPM image;
        
        const uint32_t width = 512;
        const uint32_t height = 512;

        for (uint32_t y = 0; y < height; y++) {
            for (uint32_t x = 0; x < width; x++) {                
                image.put_pixel(x, y, pnm::RGB(x / 2, y / 2, x / 2 + y / 2));
            }
        }

        image.add_comment("comment 1");
        image.add_comment("comment 2");
        image.add_comment("comment 3\n\n\n");

        image.save("result_image.ppm");

    } catch(const std::exception& e) {
        std::string error_message("write_comment:", e.what());
        throw std::runtime_error(error_message);
    }
}

void remove_comments() {
    try {
        pnm::PPM image;
        image.read("result_image.ppm");
        
        image.remove_last_comment();
        image.remove_all_comments();
        
        image.save("result_image.ppm");

    } catch(const std::exception& e) {
        std::string error_message("remove_comment: ", e.what());
        throw std::runtime_error(error_message);
    }
}

void read_comment(bool print) {
    try {
        write_comment();
    
        pnm::PPM image;
        image.read("result_image.ppm");

        std::vector<std::string> comments = image.get_comments();
        if (comments.size() == 0) {
            throw std::runtime_error("No comments found, though there should be");
        }

        if (print) {
            for (const std::string& comment : comments) {
                std::cout << comment << "\n";
            }
        }

    } catch(const std::exception& e) {
        std::string error_message("read_comment:", e.what());
        throw std::runtime_error(error_message);
    }
}

int main() {
    try {
        make_test_img();
        green_rectangle_on_top_of_existing_image();
        no_pixel_assign();
        write_comment();
        read_comment(false);
        remove_comments();

    } catch(const std::exception& e) {
        std::cout << "[ERROR] " << e.what() << "\n";
        return 1;
    }

    std::cout << "[ALL TESTS WERE COMPLETED SUCCESSFULLY]\n";

    return 0;
}
