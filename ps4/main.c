#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"
#include "transformations.h"

// | base64 -d > file.bmp

// int main(int argc, char* argv[]){
//     FILE* input = fopen(argv[1], "rb");
//     FILE* output = fopen(argv[2], "wb");

//     struct bmp_image* image = read_bmp(input);
//     struct bmp_image* edited = crop(image, 1, 1, 1, 2);
    
//     printf("Input file size: %d width %d, height %d, imgsize %d\n", edited->header->size, edited->header->width, edited->header->height, edited->header->image_size);
//     write_bmp(output, edited);
//     //printf("Output file size: %d\n", edited->header->size);
//     fclose(input);
//     fclose(output);
// }

                    
//ДЛЯ КОМИТОВ
int main(int argc, char* argv[]){
    FILE* input = fopen(argv[1], "rb");
    FILE* output = fopen(argv[2], "wb");

    struct bmp_image* img = read_bmp(input);
    struct bmp_image* rotated = rotate_right(img);
    free_bmp_image(img);
    struct bmp_image* rotatedleft = rotate_left(rotated);
    free_bmp_image(rotated);
    struct bmp_image* flip1 = flip_horizontally(rotatedleft);
    free_bmp_image(rotatedleft);
    struct bmp_image* flip2 = flip_vertically(flip1);
    free_bmp_image(flip1);
    struct bmp_image* scaled = scale(flip2, 1);
    free_bmp_image(flip2);
    struct bmp_image* cropped = crop(scaled, 1, 1, 2, 2);
    free_bmp_image(scaled);
    struct bmp_image* extracted = extract(cropped, "rg");
    free_bmp_image(cropped);

    write_bmp(output, extracted);
    free_bmp_image(extracted);
    fclose(input);
    fclose(output);
}
