#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "transformations.h"

void calculate_bmp_file_size(const struct bmp_image* image);

struct bmp_image* flip_horizontally(const struct bmp_image* image) {
    if (image == NULL) {
        return NULL;
    }
    struct bmp_image* flipped_image = malloc(sizeof(struct bmp_image));
    if (flipped_image == NULL) {
        return NULL;
    }

    flipped_image->header = malloc(sizeof(struct bmp_header));
    if (flipped_image->header == NULL) {
        free(flipped_image);
        return NULL;
    }
    memcpy(flipped_image->header, image->header, sizeof(struct bmp_header));

    size_t numPixels = image->header->width * image->header->height;

    flipped_image->data = malloc(numPixels * sizeof(struct pixel));
    if (flipped_image->data == NULL) {
        free(flipped_image->header);
        free(flipped_image);
        return NULL;
    }

    size_t row_width = image->header->width;
    for (int j = 0; j < image->header->height; j++) {
        for (int i = 0; i < row_width; i++) {
            flipped_image->data[j * row_width + i] = image->data[j * row_width + (row_width - 1 - i)];
        }
    }

    return flipped_image;
}

struct bmp_image* flip_vertically(const struct bmp_image* image){
    if(image == NULL){
        return NULL;
    }
    struct bmp_image* flipped_image = malloc(sizeof(struct bmp_image));
    if (flipped_image == NULL) {
        return NULL;
    }

    flipped_image->header = malloc(sizeof(struct bmp_header));
    if (flipped_image->header == NULL) {
        free(flipped_image);
        return NULL;
    }
    memcpy(flipped_image->header, image->header, sizeof(struct bmp_header));

    size_t numPixels = image->header->width * image->header->height;

    flipped_image->data = malloc(numPixels * sizeof(struct pixel));
    if (flipped_image->data == NULL) {
        free(flipped_image->header);
        free(flipped_image);
        return NULL;
    }

    size_t offset = 0;
    size_t offsetvert = 1;
    for(int j = 0; j < image->header->height; j++){
        for(int i = 0; i < image->header->width; i++){
            flipped_image->data[i + offset] = image->data[numPixels - (image->header->width * offsetvert) + i];
        }
        offset += image->header->width;
        offsetvert++;
    }
    return flipped_image;
}

struct bmp_image* rotate_left(const struct bmp_image* image){
    if(image == NULL){
        return NULL;
    }
    struct bmp_image* rotated_image = malloc(sizeof(struct bmp_image));
    if (rotated_image == NULL) {
        return NULL;
    }

    rotated_image->header = malloc(sizeof(struct bmp_header));
    if (rotated_image->header == NULL) {
        free(rotated_image);
        return NULL;
    }
    memcpy(rotated_image->header, image->header, sizeof(struct bmp_header));
    size_t numPixels = image->header->width * image->header->height;

    rotated_image->data = malloc(numPixels * sizeof(struct pixel));
    if (rotated_image->data == NULL) {
        free(rotated_image->header);
        free(rotated_image);
        return NULL;
    }

    size_t buffer = rotated_image->header->width;
    rotated_image->header->width = rotated_image->header->height;
    rotated_image->header->height = buffer;
    size_t source = 0;
    size_t offset = 0;
    for (unsigned int i = 0; i < rotated_image->header->width; i++) {
        for (unsigned int j = 0; j < rotated_image->header->height; j++) {
            size_t destIndex = rotated_image->header->width - 1 - i + offset;
            rotated_image->data[destIndex] = image->data[source];
            source++;
            offset += rotated_image->header->width;
        }
        offset = 0;
    }
    calculate_bmp_file_size(rotated_image);
    return rotated_image;
}

struct bmp_image* rotate_right(const struct bmp_image* image){
    if(image == NULL){
        return NULL;
    }
    struct bmp_image* rotated_image = malloc(sizeof(struct bmp_image));
    if (rotated_image == NULL) {
        return NULL;
    }

    rotated_image->header = malloc(sizeof(struct bmp_header));
    if (rotated_image->header == NULL) {
        free(rotated_image);
        return NULL;
    }
    memcpy(rotated_image->header, image->header, sizeof(struct bmp_header));
    size_t numPixels = image->header->width * image->header->height;

    rotated_image->data = malloc(numPixels * sizeof(struct pixel));
    if (rotated_image->data == NULL) {
        free(rotated_image->header);
        free(rotated_image);
        return NULL;
    }
    rotated_image->header->width = image->header->height;
    rotated_image->header->height = image->header->width;

    size_t source = numPixels - 1;
    size_t offset = 0;
    for (unsigned int i = 0; i < rotated_image->header->width; i++) {
        for (unsigned int j = 0; j < rotated_image->header->height; j++) {
            size_t destIndex = rotated_image->header->width - 1 - i + offset;
            rotated_image->data[destIndex] = image->data[source];
            source--;
            offset += rotated_image->header->width;
        }
        offset = 0;
    }
        
    calculate_bmp_file_size(rotated_image);
    return rotated_image;
}



// start_y - y-ová pozícia ľavého horného bodu na vystrihnutie
// start_x - x-ová pozícia ľavého horného bodu na vystrihnutie
// height - výška vystrihovaného obrázka v pixloch
// width - šírka vystrihovaného obrázka v pixloch

struct bmp_image* crop(const struct bmp_image* image, const uint32_t start_y, const uint32_t start_x, const uint32_t height, const uint32_t width){
    if (image == NULL || start_x < 0 || start_y < 0 || start_x + width > image->header->width || start_y + height > image->header->height) {
        return NULL;
    }

    struct bmp_image* cropped_image = malloc(sizeof(struct bmp_image));
    if (cropped_image == NULL) {
        free_bmp_image(cropped_image);
        return NULL;
    }
    cropped_image->header = malloc(sizeof(struct bmp_header));
    memcpy(cropped_image->header, image->header, sizeof(struct bmp_header));

    cropped_image->header->width = width;
    cropped_image->header->height = height;
    cropped_image->data = malloc(width * height * sizeof(struct pixel));

    if (cropped_image->data == NULL) {
        free_bmp_image(cropped_image);
        return NULL;
    }
int adjusted_start_y = image->header->height - start_y - height;

    // Copy the data from the original image to the cropped image
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int orig_idx = (adjusted_start_y + y) * image->header->width + (start_x + x);
            int crop_idx = y * width + x;
            cropped_image->data[crop_idx] = image->data[orig_idx];
        }
    }
    calculate_bmp_file_size(cropped_image);
    return cropped_image;
}



struct bmp_image* scale(const struct bmp_image* image, float factor){
    if(image == NULL){
        return NULL;
    }
    if(factor <= 0){
        return NULL;
    }
    size_t newWidth = round(image->header->width * factor);
    size_t newHeight = round(image->header->height * factor);

    struct bmp_image* scaled_image = malloc(sizeof(struct bmp_image));
    if (scaled_image == NULL){
        return NULL;
    }

    scaled_image->header = malloc(sizeof(struct bmp_header));
    if (scaled_image->header == NULL){
        free(scaled_image);
        return NULL;
    }
    size_t numPixels = newWidth * newHeight;
    scaled_image->data = malloc(numPixels * sizeof(struct pixel));
    if (scaled_image->data == NULL){
        free(scaled_image->header);
        free(scaled_image);
        return NULL;
    }
    memcpy(scaled_image->header, image->header, sizeof(struct bmp_header));

    scaled_image->header->height = newHeight;
    scaled_image->header->width = newWidth;
    calculate_bmp_file_size(scaled_image);
    for (int i = 0; i < newHeight; i++){
        for (int j = 0; j < newWidth; j++){
            int x = floor((float)j / newWidth * image->header->width);
            int y = floor((float)i / newHeight * image->header->height);
            scaled_image->data[i * newWidth + j] = image->data[y * image->header->width + x];
        }
    }
    return scaled_image;
}
bool isInside(char color, const char* colors_to_keep){
    for(int i = 0; i < strlen(colors_to_keep); i++){
        if(color == colors_to_keep[i]){
            return true;
        }
    }
    return false;
}
bool noLetters(const char* colors_to_keep){
    // if(!strcmp(colors_to_keep, "rgb")) return false;
    // if(!strcmp(colors_to_keep, "rbg")) return false;
    // if(!strcmp(colors_to_keep, "bgr")) return false;
    // if(!strcmp(colors_to_keep, "brg")) return false;
    // if(!strcmp(colors_to_keep, "gbr")) return false;
    // if(!strcmp(colors_to_keep, "grb")) return false;

    // if(!strcmp(colors_to_keep, "r")) return false;
    // if(!strcmp(colors_to_keep, "g")) return false;
    // if(!strcmp(colors_to_keep, "b")) return false;
    for(int i = 0; i < strlen(colors_to_keep); i++){
        if(colors_to_keep[i] == 'r' || colors_to_keep[i] == 'g' || colors_to_keep[i] == 'b'){
            return false;
        }
    }
    return true;
}

struct bmp_image* extract(const struct bmp_image* image, const char* colors_to_keep){
    if(image == NULL){
        return NULL;
    }

    if (colors_to_keep == NULL || noLetters(colors_to_keep)){
        return NULL;
    }
    struct bmp_image* extracted = malloc(sizeof(struct bmp_image));
    extracted->header = malloc(sizeof(struct bmp_header));
    if (extracted->header == NULL){
        free(extracted);
        return NULL;
    }
    size_t dataSize = image->header->width * image->header->height;
    memcpy(extracted->header, image->header, sizeof(struct bmp_header));
    extracted->data = (struct pixel*)malloc(dataSize * sizeof(struct pixel));
    for(int i = 0; i < dataSize; i++){
        extracted->data[i] = image->data[i];
    }
    //printf("%d %d %d\n", extracted->data[1].red, extracted->data[1].green,    extracted->data[1].blue);
    if(!isInside('r', colors_to_keep)){
        for(int i = 0; i < dataSize; i++){
            extracted->data[i].red = 0;
        }
    }
    if(!isInside('g', colors_to_keep)){
        for(int i = 0; i < dataSize; i++){
            extracted->data[i].green = 0;
        }
    }
    if(!isInside('b', colors_to_keep)){
        for(int i = 0; i < dataSize; i++){
            extracted->data[i].blue = 0;
        }
    }
    return extracted;
}
