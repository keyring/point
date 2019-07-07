
#include "point.h"

void clear(struct paint *painter, int clear_color)
{
    memset(painter->canvas, clear_color, 3*painter->width*painter->height);
}

int present(struct paint *painter)
{
    int length = painter->width * painter->height;
    FILE *f = fopen("image.ppm", "w");

    fprintf( f, "P3\n%d %d\n%d\n", painter->width, painter->height, 255 );
    for (int i = 0; i < length; i++) {
        fprintf( f, "%d %d %d ",painter->canvas[i].r, painter->canvas[i].g, painter->canvas[i].b );
    }

    printf("-----------OUTPUT SUCCESS--------\n");

    return fclose(f);
}

void render(struct paint *painter)
{
    clear(painter, 255);        /* clear canvas */
    int w = painter->width;
    int h = painter->height;

    /* paint a point */
    struct color color  = {255, 0, 0};
    pixel(painter, 40, 40, color);

    /* paint a line */
    struct color line_color = {0, 0, 0};
    struct point start = {50, 50};

    for(int i = 0; i < 100; i++){
        int x = rand() % w;
        int y = rand() % h;
        struct point end = {x, y};
        line(painter, start, end, line_color);
    }

    /* paint a triangle */
    struct color triangle_color = {23, 124, 210};
    struct point p1 = {(int)(w/4), (int)(h/4)};
    struct point p2 = {(int)(w/2), (int)(h/4*3)};
    struct point p3 = {(int)(w/4*3), (int)(h/4)};
    triangle(painter, p1, p2, p3, triangle_color);

    /* paint a circle */
    struct color circle_color = {124, 22, 57};
    int circle_radius = 20;
    circle(painter, 50, 50, circle_radius, circle_color);
}

int main(int argc, char **argv)
{
  printf("----------POINT RENDER----------\n");

  const int w = 100;
  const int h = 100;

  struct color canvas[w*h];
  struct paint painter = {.width = w,
                          .height = h,
                          .canvas = canvas };
  render(&painter);

  if (present(&painter)) {
      return -1;
  }

  return 0;

}
