#include <stdio.h>
#include <stdint.h>

#define IMAGE_WIDTH 400
#define IMAGE_HEIGHT 400

FILE* generate_image_with_bytes(int img_width, int img_height, uint8_t content[img_height][img_width], char *filename)
{
	FILE *img_file = fopen(filename, "wb");

	fprintf(img_file, "P6\n%d %d\n255\n", img_width, img_height);

	for (int i = 0; i < img_height; ++i) {
		fwrite(content[i], sizeof(uint32_t), img_width, img_file);
	}
	return img_file;
}

int main(void)
{
	char *output_file = "output.ppm";
	uint8_t image_content[IMAGE_HEIGHT][IMAGE_WIDTH];
	for (int i = 0; i < IMAGE_HEIGHT; ++i) {
		for (int j = 0; j < IMAGE_WIDTH; ++j) {
			uint8_t color = 0x00;
			image_content[i][j] = color;
		}
	}
	FILE *img_file = generate_image_with_bytes(IMAGE_WIDTH, IMAGE_HEIGHT, image_content, output_file);
	fclose(img_file);
	return 0;
}
