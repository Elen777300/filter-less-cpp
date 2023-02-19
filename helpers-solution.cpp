#include <cmath>
#include "helpers.hpp"

// Convert image to grayscale
void grayscale(int height, int width, std::unique_ptr<RGBTRIPLE[]> &image) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            RGBTRIPLE &pixel = image[i * width + j];
            BYTE average = (pixel.rgbtRed + pixel.rgbtGreen + pixel.rgbtBlue) / 3;
            pixel.rgbtRed = average;
            pixel.rgbtGreen = average;
            pixel.rgbtBlue = average;
        }
    }
}


// Convert image to sepia
void sepia(int height, int width, std::unique_ptr<RGBTRIPLE[]> &image) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            RGBTRIPLE &pixel = image[i * width + j];

            // Convert to sepia
            int sepiaRed = round(0.393 * pixel.rgbtRed + 0.769 * pixel.rgbtGreen + 0.189 * pixel.rgbtBlue);
            int sepiaGreen = round(0.349 * pixel.rgbtRed + 0.686 * pixel.rgbtGreen + 0.168 * pixel.rgbtBlue);
            int sepiaBlue = round(0.272 * pixel.rgbtRed + 0.534 * pixel.rgbtGreen + 0.131 * pixel.rgbtBlue);

            // Cap values at 255
            if (sepiaRed > 255) {
                sepiaRed = 255;
            }

            if (sepiaGreen > 255) {
                sepiaGreen = 255;
            }

            if (sepiaBlue > 255) {
                sepiaBlue = 255;
            }

            // Update pixel
            pixel.rgbtRed = sepiaRed;
            pixel.rgbtGreen = sepiaGreen;
            pixel.rgbtBlue = sepiaBlue;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, std::unique_ptr<RGBTRIPLE[]> &image) {
    // Iterate over image rows
    for (int i = 0; i < height; i++) {
        // Swap pixels across the horizontal center line
        for (int j = 0; j < width / 2; j++) {
            RGBTRIPLE temp = image[i * width + j];
            image[i * width + j] = image[i * width + (width - j - 1)];
            image[i * width + (width - j - 1)] = temp;
        }
    }
}

// Blur image
void blur(int height, int width, std::unique_ptr<RGBTRIPLE[]> &image) {
    // Define the kernel
    const float kernel[3][3] = {
            {1.0 / 9, 1.0 / 9, 1.0 / 9},
            {1.0 / 9, 1.0 / 9, 1.0 / 9},
            {1.0 / 9, 1.0 / 9, 1.0 / 9}
    };

    // Create a copy of the original image
    auto imageCopy = std::make_unique<RGBTRIPLE[]>(height * width);
    memcpy(imageCopy.get(), image.get(), height * width * sizeof(RGBTRIPLE));

    // Apply the kernel to each pixel in the image
    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            float sumBlue = 0.0, sumGreen = 0.0, sumRed = 0.0;
            for (int k = -1; k <= 1; k++) {
                for (int l = -1; l <= 1; l++) {
                    int index = (i + k) * width + (j + l);
                    sumBlue += kernel[k + 1][l + 1] * imageCopy[index].rgbtBlue;
                    sumGreen += kernel[k + 1][l + 1] * imageCopy[index].rgbtGreen;
                    sumRed += kernel[k + 1][l + 1] * imageCopy[index].rgbtRed;
                }
            }
            int index = i * width + j;
            image[index].rgbtBlue = round(sumBlue);
            image[index].rgbtGreen = round(sumGreen);
            image[index].rgbtRed = round(sumRed);
        }
    }
}
