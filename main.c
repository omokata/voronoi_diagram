#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define IMAGE_WIDTH 400
#define IMAGE_HEIGHT 400

// 0xAABBGGRR
#define COLOR_RED 0xFF0000FF
#define COLOR_GREEN 0xFF00FF00
#define COLOR_BLUE 0xFFFF0000

typedef uint32_t Color32;

void fill_image(int img_height, int img_width, Color32 image[img_height][img_width], Color32 color)
{
	for (int i = 0; i < IMAGE_HEIGHT; ++i) {
		for (int j = 0; j < IMAGE_WIDTH; ++j) {
			image[i][j] = color;
		}
	}
}

FILE* save_image_as_ppm(int img_width, int img_height, Color32 image[img_height][img_width], char *filename)
{
	FILE *img_file = fopen(filename, "wb");
	if (img_file == NULL) {
		fprintf(stderr, "ERROR: Could not open file named %s: %s\n", filename, strerror(errno));
	}
	fprintf(img_file, "P6\n%d %d 255\n", img_width, img_height);
	for (int i = 0; i < img_height; ++i) {
		for (int j = 0; j < img_width; ++j) {
			// 0xAABBGGRR
			Color32 pixel = image[i][j];
			uint8_t bytes[3] = {
				(pixel&0x0000FF)>>8*0,
				(pixel&0x00FF00)>>8*1,
				(pixel&0xFF0000)>>8*2,
			};
			fwrite(bytes, sizeof(bytes), 1, img_file);
			assert(!ferror(img_file));
		}
	}
	return img_file;
}

int main(void)
{
	char *output_file = "output.ppm";
	Color32 image[IMAGE_HEIGHT][IMAGE_WIDTH];
	fill_image(IMAGE_HEIGHT, IMAGE_WIDTH, image, COLOR_BLUE);
	FILE *img_file = save_image_as_ppm(IMAGE_WIDTH, IMAGE_HEIGHT, image, output_file);
	fclose(img_file);
	return 0;
}
