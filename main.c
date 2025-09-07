#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define IMAGE_WIDTH 800
#define IMAGE_HEIGHT 600

// 0xAABBGGRR
#define COLOR_RED   0xFF0000FF
#define COLOR_BLACK 0xFF000000
#define COLOR_GREEN 0xFF00FF00
#define COLOR_BLUE  0xFFFF0000
#define COLOR_WHITE 0xFFFFFFFF

typedef uint32_t Color32;

typedef struct point_t {
	int x;
	int y;
} point_t;

void fill_image(int img_height, int img_width, Color32 image[img_height][img_width], Color32 color)
{
	for (int i = 0; i < IMAGE_HEIGHT; ++i) {
		for (int j = 0; j < IMAGE_WIDTH; ++j) {
			image[i][j] = color;
		}
	}
}

void save_image_as_ppm(int img_width, int img_height, Color32 image[img_height][img_width], char *filename)
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
	fclose(img_file);
}

// `point` is the center of the circle
void plot_point(int img_height, int img_width, Color32 image[img_height][img_width],
				point_t point, int radius, Color32 color)
{
	int start_y = point.y - radius;
	int start_x = point.x - radius;
	int end_y = point.y + radius;
	int end_x = point.x + radius;
	for (int x = start_x; x <= end_x; ++x) {
		if (0 <= x && x < IMAGE_WIDTH) {
			for (int y = start_y; y <= end_y; ++y) {
				if (0 <= y && y < IMAGE_HEIGHT) {
					int dx = point.x - x;
					int dy = point.y - y;
					if (dx*dx + dy*dy <= radius*radius) {
						image[y][x] = color;
					}
				}
			}
		}
	}
}

int main(void)
{
	char *output_file = "output.ppm";
	Color32 image[IMAGE_HEIGHT][IMAGE_WIDTH];
	fill_image(IMAGE_HEIGHT, IMAGE_WIDTH, image, COLOR_WHITE);
	plot_point(IMAGE_HEIGHT, IMAGE_WIDTH, image, (point_t){10, 10}, 5, COLOR_BLACK);
	save_image_as_ppm(IMAGE_WIDTH, IMAGE_HEIGHT, image, output_file);
	return 0;
}
