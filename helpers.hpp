#ifndef FILTERLESSCPP_HELPERS_HPP
#define FILTERLESSCPP_HELPERS_HPP

#include "bmp.hpp"
#include <memory>


// Convert image to grayscale
void grayscale(int height, int width, std::unique_ptr<RGBTRIPLE[]> &image);

// Convert image to sepia
void sepia(int height, int width, std::unique_ptr<RGBTRIPLE[]> &image);

// Reflect image horizontally
void reflect(int height, int width, std::unique_ptr<RGBTRIPLE[]> &image);

// Blur image
void blur(int height, int width, std::unique_ptr<RGBTRIPLE[]> &image);

#endif //FILTERLESSCPP_HELPERS_HPP
