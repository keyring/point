#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct color { unsigned char r,g,b; };
struct point { int x,y; };
struct paint { int width; int height; struct color *canvas; };

void pixel(struct point pos, struct color pixel_color, struct paint *painter)
{
    pos.y = painter->height - pos.y;
    int index = pos.y * painter->height + pos.x;
    painter->canvas[index].r = pixel_color.r;
    painter->canvas[index].g = pixel_color.g;
    painter->canvas[index].b = pixel_color.b;
}

void line_dda(struct point start, struct point end, struct color line_color, struct paint *painter)
{
    int dx = end.x - start.x;
    int dy = end.y - start.y;

    int steps = abs(dx) >= abs(dy) ? abs(dx) : abs(dy);

    float xi = (float)dx / steps;
    float yi = (float)dy / steps;

    struct point pos;

    float x = (float)start.x;
    float y = (float)start.y;
    for(int i = 0; i <= steps; i++){
        pos.x = (int)(x + 0.5);
        pos.y = (int)(y + 0.5);
        pixel(pos, line_color, painter);
        x += xi;
        y += yi;
    }
    
}

void line_midpoint(struct point start ,struct point end, struct color line_color, struct paint *painter)
{
    
}

void line(struct point start, struct point end, struct color line_color, struct paint *painter)
{
    /* dda */
    line_dda(start, end, line_color, painter);

    /* mid point  */
    //line_midpoint(start, end, line_color, painter);
    /* bresenham   */
}

void triangle(struct point pos1, struct point pos2, struct point pos3, struct color line_color, struct paint *painter)
{}

void clear(int clear_color, struct paint *painter)
{
    memset(painter->canvas, clear_color, 3*painter->width*painter->height);
}


void render(struct paint *painter)
{
    clear(255, painter);        /* clear canvas */

    /* paint a point */
    struct color color  = {255, 0, 0};
    struct point pos = {40, 40};
    pixel(pos, color, painter);

    /* paint a line */
    struct color line_color = {255, 0, 0};
    struct point start = {50,50};

    for(int i = 0; i < 100; i++){
        int x = rand()%100;
        int y = rand()%100;
        struct point end = {x, y};
        line(end, start, line_color, painter);
    }
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