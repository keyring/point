#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct color { unsigned char r,g,b; };
struct point { int x,y; };
struct paint { int width; int height; struct color *canvas; };

void pixel(struct point pos, struct color pixel_color, struct paint *painter)
{
    int index = pos.y * painter->height + pos.x;
    painter->canvas[index].r = pixel_color.r;
    painter->canvas[index].g = pixel_color.g;
    painter->canvas[index].b = pixel_color.b;
}

void clear(int clrear_color, struct paint *painter)
{
    memset(painter->canvas, clrear_color, 3*painter->width*painter->height);
}


void render(struct paint *painter)
{
    clear(255, painter);
    struct color color  = {255, 0, 0};
    struct point pos = {40, 40};
    pixel(pos, color, painter);
}


int print(struct paint *painter)
{
    int length = painter->width * painter->height;
    FILE *f = fopen("image.ppm", "w");

    fprintf( f, "P3\n%d %d\n%d\n", painter->width, painter->height, 255 );
    for (int i = 0; i < length; i++) {
        fprintf( f, "%d %d %d ",painter->canvas[i].r, painter->canvas[i].g, painter->canvas[i].b );
    }

    printf("-----------OUTPUT SUCCESS--------\n");

    return  fclose(f);
}


int main(int argc, char **argv)
{
  printf("----------POINT RENDER----------\n");

  const int w = 100;
  const int h = 100;

  struct color canvas[w*h] = {0};
  struct paint painter = {.width = w,
                          .height = h,
                          .canvas = canvas };
  render(&painter);

  if (print(&painter)) {
      return -1;
  }

  return 0;

}