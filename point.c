#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct color {
  unsigned char r,g,b;
}color;

void clear(int width, int height, int clrear_color, color *output)
{
    memset(output, clrear_color, 3*width*height);
}


void render(color *output)
{
    clear(1024, 768, 120, output);
}


int out_result(int width, int height, color *color)
{
    int length = width * height;
    FILE *f = fopen("image.ppm", "w");
    fprintf( f, "P3\n%d %d\n%d\n", width, height, 255 );
    for (int i = 0; i < length; i++) {
        fprintf( f, "%d %d %d ", color[i].r, color[i].g, color[i].b );
    }

    return  fclose(f);
}


int main(int argc, char **argv)
{
  printf("----------POINT RENDER----------\n");

  const short w = 1024,h = 768;

  color out_color[w*h] = {0};

  render(out_color);

  return out_result(w, h, out_color);

}