#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp.h"

struct bmp_image* read_bmp(FILE* stream) {
    if (stream == NULL) {
        return NULL;
    }

    struct bmp_header* header = read_bmp_header(stream);
    if(header == NULL){
        fprintf(stderr, "Error: This is not a BMP file.\n");
        return NULL;
    }
    

    struct pixel* data = read_data(stream, header);
    if(data == NULL){
        fprintf(stderr, "Error: Corrupted BMP file.\n");
        return NULL;
    }
  
    struct bmp_image* image = malloc(sizeof(struct bmp_image));

    image->header = header;

    image->data = data;

    return image;
}
void calculate_bmp_file_size(const struct bmp_image* image) {
    uint32_t header_size = 54; 
    uint32_t row_size_without_padding = image->header->width * 3;
    uint32_t padding_per_row = (4 - (row_size_without_padding % 4)) % 4;
    uint32_t total_row_size = row_size_without_padding + padding_per_row;
    uint32_t total_data_size = total_row_size * image->header->height;
    image->header->size = header_size + total_data_size;
    image->header->image_size = total_data_size;
}

bool write_bmp(FILE* stream, const struct bmp_image* image) {
    if (stream == NULL || image == NULL) return false;
    calculate_bmp_file_size(image);
    if (fwrite(image->header, sizeof(struct bmp_header), 1, stream) != 1) {
        fprintf(stderr, "Failed to write header.\n");
        return false;
    }

    fseek(stream, image->header->offset, SEEK_SET);

    int row_padded = (image->header->width * sizeof(struct pixel) + 3) & (~3);
    unsigned char* padding = calloc(1, row_padded - image->header->width * sizeof(struct pixel));
    if (!padding) {
        fprintf(stderr, "Memory allocation failed for padding.\n");
        return false;
    }

    for (int i = 0; i < image->header->height; i++) {
        if (fwrite(&image->data[i * image->header->width], sizeof(struct pixel), image->header->width, stream) != image->header->width) {
            fprintf(stderr, "Failed to write pixel data.\n");
            free(padding);
            return false;
        }
        if (fwrite(padding, 1, row_padded - image->header->width * sizeof(struct pixel), stream) != row_padded - image->header->width * sizeof(struct pixel)) {
            fprintf(stderr, "Failed to write padding.\n");
            free(padding);
            return false;
        }
    }

    free(padding);
    return true;
}


struct bmp_header* read_bmp_header(FILE* stream){
    if (stream == NULL) {
        return NULL;
    }
    fseek(stream, 0, SEEK_SET);
    struct bmp_header* header = malloc(sizeof(struct bmp_header));

    if (fread(header, sizeof(struct bmp_header), 1, stream) != 1) {
        free(header);
        return NULL;
    }
    if (header->type != 0x4D42) {
        free(header);
        return NULL;
    }
    fseek(stream, sizeof(header), SEEK_SET);
    return header;
}

struct pixel* read_data(FILE* stream, const struct bmp_header* header) {
    if (stream == NULL || header == NULL) {
        fprintf(stderr, "stroka 98\n");
        return NULL;
    }

    int numPixels = header->width * header->height;
    struct pixel* pixels = malloc(sizeof(struct pixel) * numPixels);
    if (pixels == NULL) {
        fprintf(stderr, "stroka 104\n");
        return NULL;
    }

    int padding = (4 - (header->width * sizeof(struct pixel)) % 4) % 4;
    fseek(stream, sizeof(struct bmp_header), SEEK_SET);
    for (int i = 0; i < header->height; i++) {
        size_t bytesRead = fread(&pixels[i * header->width], sizeof(struct pixel), header->width, stream);
        if (bytesRead != header->width) {
            fprintf(stderr, "stroka 113\n");
            free(pixels);
            return NULL;
        }

        if (fseek(stream, padding, SEEK_CUR) != 0) {
            fprintf(stderr, "stroka 118\n");
            free(pixels);
            return NULL;
        }
    }

    return pixels;
}


void free_bmp_image(struct bmp_image* image){
    if(image == NULL) return;
    free(image->data);
    free(image->header);
    free(image);
}
