#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define IMAGE_WIDTH 800
#define IMAGE_HEIGHT 600

// 0xAABBGGRR
#define COLOR_RED 0xFF0000FF
#define COLOR_BLACK 0xFF000000
#define COLOR_GREEN 0xFF00FF00
#define COLOR_BLUE 0xFFFF0000
#define COLOR_WHITE 0xFFFFFFFF

#define SEEDS_COUNT 10
#define SEEDS_RADIUS 5
#define SEEDS_COLOR COLOR_BLACK

typedef uint32_t Color32;

#define SKY_BLUE    0xFFE9C64B
#define TEAL        0xFFA3B32B
#define MINT_GREEN  0xFFA6D67E
#define SOFT_LIME   0xFF65E2C9
#define LIGHT_CORAL 0xFF848AF0
#define LAVENDER    0xFFE29EB5
#define INDIGO      0xFFD75F4C
#define ROSE_PINK       0xFFAA6BEB
#define AQUA_BLUE       0xFFC9D558
#define PERIWINKLE      0xFFF2B9A3

static Color32 palette[] = {
	SKY_BLUE,
	TEAL,
	MINT_GREEN,
	SOFT_LIME,
	LIGHT_CORAL,
	LAVENDER,
	INDIGO,
	ROSE_PINK,
	AQUA_BLUE,
	PERIWINKLE,
};

#define PALETTE_COUNT (sizeof(palette) / sizeof(palette[0]))

typedef struct point_t {
  int x;
  int y;
} point_t;

void fill_image(int img_height, int img_width,
                Color32 image[img_height][img_width], Color32 color) {
  for (int i = 0; i < IMAGE_HEIGHT; ++i) {
    for (int j = 0; j < IMAGE_WIDTH; ++j) {
      image[i][j] = color;
    }
  }
}

void save_image_as_ppm(int img_width, int img_height,
                       Color32 image[img_height][img_width], char *filename) {
  FILE *img_file = fopen(filename, "wb");
  if (img_file == NULL) {
    fprintf(stderr, "ERROR: Could not open file named %s: %s\n", filename,
            strerror(errno));
  }
  fprintf(img_file, "P6\n%d %d 255\n", img_width, img_height);
  for (int i = 0; i < img_height; ++i) {
    for (int j = 0; j < img_width; ++j) {
      // 0xAABBGGRR
      Color32 pixel = image[i][j];
      uint8_t bytes[3] = {
          (pixel & 0x0000FF) >> 8 * 0,
          (pixel & 0x00FF00) >> 8 * 1,
          (pixel & 0xFF0000) >> 8 * 2,
      };
      fwrite(bytes, sizeof(bytes), 1, img_file);
      assert(!ferror(img_file));
    }
  }
  fclose(img_file);
}

// `point` is the center of the circle
void plot_point(int img_height, int img_width,
                Color32 image[img_height][img_width], point_t point, int radius,
                Color32 color) {
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
          if (dx * dx + dy * dy <= radius * radius) {
            image[y][x] = color;
          }
        }
      }
    }
  }
}

void generate_random_seeds(int seed_count, point_t random_seeds[seed_count]) {
  for (int i = 0; i < seed_count; ++i) {
    random_seeds[i].x = rand() % IMAGE_WIDTH;
    random_seeds[i].y = rand() % IMAGE_HEIGHT;
  }
}

// Return non sqrt result.
int point_sqr_dist(point_t a, point_t b) {
  int dx = a.x - b.x;
  int dy = a.y - b.y;
  return dx * dx + dy * dy;
}

void fill_voronoi_cell(int img_height, int img_width,
                       Color32 image[img_height][img_width], int seed_count,
                       point_t seeds[seed_count]) {
  for (int y = 0; y < img_height; ++y) {
    for (int x = 0; x < img_width; ++x) {
      // Calculate the distance between the pixel and the points
      // Figure out which one has the most less distance
      // Color the pixel
      int prev_seed = 0;
      for (int seed = 1; seed < seed_count; ++seed) {
        if (point_sqr_dist(seeds[seed], (point_t){x, y}) <
            point_sqr_dist(seeds[prev_seed], (point_t){x, y})) {
          prev_seed = seed;
        }
      }
      image[y][x] = palette[prev_seed%PALETTE_COUNT];
    }
  }
}

int main(void) {
  srand(time(NULL));
  char *output_file = "output.ppm";
  Color32 image[IMAGE_HEIGHT][IMAGE_WIDTH] = {0};
  point_t random_seeds[SEEDS_COUNT] = {0};
  generate_random_seeds(SEEDS_COUNT, random_seeds);
  fill_image(IMAGE_HEIGHT, IMAGE_WIDTH, image, COLOR_BLACK);
  fill_voronoi_cell(IMAGE_HEIGHT, IMAGE_WIDTH, image, SEEDS_COUNT,
                    random_seeds);
  for (int seed = 0; seed < SEEDS_COUNT; ++seed) {
    plot_point(IMAGE_HEIGHT, IMAGE_WIDTH, image, random_seeds[seed],
               SEEDS_RADIUS, SEEDS_COLOR);
  }
  save_image_as_ppm(IMAGE_WIDTH, IMAGE_HEIGHT, image, output_file);
  return 0;
}
