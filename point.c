#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct color {
  unsigned char r,g,b;
}color;

const int WIDTH = 1024;
const int HEIGHT = 768;

void point(int x, int y, color point_color, color *canvas)
{
    int index = y * HEIGHT + x;
    canvas[index].r = point_color.r;
    canvas[index].g = point_color.g;
    canvas[index].b = point_color.b;
}

void clear(int width, int height, int clrear_color, color *canvas)
{
    memset(canvas, clrear_color, 3*width*height);
}


void render(int width, int height, color *canvas)
{
    clear(width, height, 255, canvas);
    color color  = {255, 0, 0};
    point(40, 40, color, canvas);
}


int print(int width, int height, color *color)
{
    int length = width * height;
    FILE *f = fopen("image.ppm", "w");
    fprintf( f, "P3\n%d %d\n%d\n", width, height, 255 );
    for (int i = 0; i < length; i++) {
        fprintf( f, "%d %d %d ", color[i].r, color[i].g, color[i].b );
    }

    printf("-----------OUTPUT SUCCESS--------\n");

    return  fclose(f);
}


int main(int argc, char **argv)
{
  printf("----------POINT RENDER----------\n");

  const int w = WIDTH;
  const int h = HEIGHT;

  color canvas[w*h] = {0};

  render(w, h, canvas);

  if (print(w, h, canvas)) {
      return -1;
  }

  return 0;

}