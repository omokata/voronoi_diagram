#include <stdio.h>
#include <stdint.h>

#define IMAGE_WIDTH 400
#define IMAGE_HEIGHT 400

#define COLOR_RED 0xFF0000FF

FILE* save_image_as_ppm(int img_width, int img_height, uint32_t image[img_height][img_width], char *filename)
{
	FILE *img_file = fopen(filename, "wb");

	fprintf(img_file, "P6\n%d %d 255\n", img_width, img_height);
	for (int i = 0; i < img_height; ++i) {
		for (int j = 0; j < img_width; ++j) {
			// 0xAABBGGRR
			uint32_t pixel = image[i][j];
			uint8_t bytes[3] = {
				(pixel&0x0000FF)>>8*0,
				(pixel&0x00FF00)>>8*1,
				(pixel&0xFF0000)>>8*2,
			};
			fwrite(bytes, sizeof(bytes), 1, img_file);
		}
	}
	return img_file;
}

int main(void)
{
	char *output_file = "output.ppm";
	uint32_t image[IMAGE_HEIGHT][IMAGE_WIDTH];
	for (int i = 0; i < IMAGE_HEIGHT; ++i) {
		for (int j = 0; j < IMAGE_WIDTH; ++j) {
			uint32_t color = COLOR_RED;
			image[i][j] = color;
		}
	}
	FILE *img_file = save_image_as_ppm(IMAGE_WIDTH, IMAGE_HEIGHT, image, output_file);
	fclose(img_file);
	return 0;
}
