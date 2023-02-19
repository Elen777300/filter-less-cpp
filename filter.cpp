#include <iostream>
#include <fstream>
#include <unistd.h>

#include "helpers.hpp"

int main(int argc, char *argv[]) {
    const char *filters = "bgrs";

    // Get filter flag and check validity
    int opt;
    char filter = '\0';
    while ((opt = getopt(argc, argv, filters)) != -1) {
        if (opt == '?') {
            std::cerr << "Invalid filter." << std::endl;
            return 1;
        }
        filter = static_cast<char>(opt);
    }

    // Ensure only one filter
    if (getopt(argc, argv, filters) != -1) {
        std::cerr << "Only one filter allowed." << std::endl;
        return 2;
    }

    // Ensure proper usage
    if (argc != (optind + 2)) {
        std::cerr << "Usage: ./filter [flag] infile outfile" << std::endl;
        return 3;
    }

    // Remember filenames
    char *infile = argv[optind];
    char *outfile = argv[optind + 1];

    std::ifstream inptr(infile, std::ios::binary);
    if (!inptr) {
        std::cerr << "Could not open " << argv[2] << "." << std::endl;
        return 4;
    }

    // Open output file
    std::ofstream outptr(outfile, std::ios::binary);
    if (!outptr) {
        std::cerr << "Could not create " << argv[3] << "." << std::endl;
        return 5;
    }

    // Read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    inptr.read(reinterpret_cast<char *>(&bf), sizeof(BITMAPFILEHEADER));

    // Read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    inptr.read(reinterpret_cast<char *>(&bi), sizeof(BITMAPINFOHEADER));

    // Check if infile is a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0) {
        std::cerr << "Unsupported file format." << std::endl;
        return 6;
    }

    // Get image's dimensions
    int height = abs(bi.biHeight);
    int width = bi.biWidth;

    // Allocate memory for image
    auto image = std::make_unique<RGBTRIPLE[]>(height * width);
    if (image == nullptr) {
        std::cout << "Not enough memory to store image." << std::endl;
        outptr.close();
        inptr.close();
        return 7;
    }

    // Determine padding for scanlines
    int padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;

    // Iterate over infile's scanlines
    for (int i = 0; i < height; i++) {
        // Read row into pixel array
        inptr.read(reinterpret_cast<char *>(image.get() + i * width), width * sizeof(RGBTRIPLE));

        // Skip over padding
        inptr.seekg(padding, std::ios_base::cur);
    }

    // Filter image
    switch (filter) {
        // Blur
        case 'b':
            blur(height, width, image);
            break;
            // Grayscale
        case 'g':
            grayscale(height, width, image);
            break;
            // Reflection
        case 'r':
            reflect(height, width, image);
            break;
            // Sepia
        case 's':
            sepia(height, width, image);
            break;
        default:
            std::cerr << "Unsupported argument" << std::endl;
            return 8;
    }

    // Write outfile's BITMAPFILEHEADER
    outptr.write(reinterpret_cast<const char *>(&bf), sizeof(BITMAPFILEHEADER));

    // Write outfile's BITMAPINFOHEADER
    outptr.write(reinterpret_cast<const char *>(&bi), sizeof(BITMAPINFOHEADER));

    // Write new pixels to outfile
    for (int i = 0; i < height; i++) {
        // Write row to outfile
        outptr.write(reinterpret_cast<const char *>(&image[i * width]), sizeof(RGBTRIPLE) * width);

        // Write padding at end of row
        for (int k = 0; k < padding; k++) {
            outptr.put(0x00);
        }
    }

    // Close files
    inptr.close();
    outptr.close();
    return 0;
}
