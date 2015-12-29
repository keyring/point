#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct color { unsigned char r,g,b; };
struct point { int x,y; };
struct paint { int width; int height; struct color *canvas; };

void pixel(int x, int y, struct color pixel_color, struct paint *painter)
{
    y = painter->height - y;
    int index = y * painter->height + x;
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

    float x = (float)start.x + 0.5;
    float y = (float)start.y + 0.5;
    for(int i = 0; i <= steps; i++){
        pixel((int)x, (int)y, line_color, painter);
        x += xi;
        y += yi;
    }
}

void line_midpoint(struct point start ,struct point end, struct color line_color, struct paint *painter)
{
    int dx, dy, d, d1, d2, x, y;
    int a = start.y - end.y;
    int b = end.x - start.x;
    x = start.x;
    y = start.y;

    dx = end.x - start.x;
    dy = end.y - start.y;
    int xi = dx < 0 ? -1 : 1;
    int yi = dy < 0 ? -1 : 1;
    int steps = abs(dx) >= abs(dy) ? abs(dx) : abs(dy);

    if(abs(dx) > abs(dy)){
        if(dy >= 0){
            d =  2 * a + b;
            d1 = 2 * (a);
            d2 = 2 * (a) + 2 * b;
            while(steps-- >= 0){
                printf("%d,%d\n",x,y);
                pixel(x, y, line_color, painter);
                x+=xi;
                if(d < 0){ y+=yi; d+=d2; }
                else{ d+=d1; }
            }
        }else{
            d = 2 * (xi*a) - b;
            d1 = 2 * (xi*a) - 2*b;
            d2 = 2 * (xi*a);
            while(steps-- >= 0){
                pixel(x, y, line_color, painter);
                x+=xi;
                if(d < 0){ d+=d2; }
                else{ y+=yi; d+=d1; }
            }
        }

    }else{
        if(dx >= 0){
            d = 2 * (yi*b) + a;
            d1 = 2 * (yi*b);
            d2 = 2 * (yi*b) + 2*a;
            while(steps-- >= 0){
                pixel(x, y, line_color, painter);
                y+=yi;
                if(d < 0){ x+=xi; d+=d2; }
                else{ d+=d1; }
            }
        }else{
            d = 2 * (yi*b) - a;
            d1 = 2 * (yi*b) - 2*a;
            d2 = 2 * (yi*b);
            while(steps-- >= 0){
                pixel(x, y, line_color, painter);
                y+=yi;
                if(d < 0){ d+=d2; }
                else{ x+=xi; d+=d1; }
            }
        }
    }

    /* todo */
}

void line(struct point start, struct point end, struct color line_color, struct paint *painter)
{
    /* dda */
//    line_dda(start, end, line_color, painter);

    /* mid point  */
    line_midpoint(start, end, line_color, painter);
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
    pixel(40,40, color, painter);

    /* paint a line */
    struct color line_color = {0, 0, 0};
    struct point start = {50,50};

    for(int i = 0; i < 1; i++){
        int x = 20;//rand()%100;
        int y = 60;//rand()%100;
        struct point end = {x, y};
        line(start, end, line_color, painter);
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