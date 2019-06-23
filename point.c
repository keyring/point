#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct color { unsigned char r,g,b; };
struct point { int x,y; };
struct paint { int width; int height; struct color *canvas; };

/* utility */
void swap(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

void pixel(struct paint *painter, int x, int y, struct color pixel_color)
{
    /* origin is left-bottom */
    y = painter->height - y;
    int index = y * painter->height + x;
    painter->canvas[index].r = pixel_color.r;
    painter->canvas[index].g = pixel_color.g;
    painter->canvas[index].b = pixel_color.b;
}

void line_dda(struct paint *painter, struct point start, struct point end, struct color line_color)
{
    int dx = end.x - start.x;
    int dy = end.y - start.y;

    int steps = abs(dx) >= abs(dy) ? abs(dx) : abs(dy);

    float xi = (float)dx / steps;
    float yi = (float)dy / steps;

    float x = (float)start.x + 0.5;
    float y = (float)start.y + 0.5;
    for(int i = 0; i <= steps; i++){
        pixel(painter, (int)x, (int)y, line_color);
        x += xi;
        y += yi;
    }
}

void line_midpoint(struct paint *painter, struct point start ,struct point end, struct color line_color)
{
    int dx, dy, d, incry, incre, incrne, slope=0;
    int x1 = start.x;
    int y1 = start.y;
    int x2 = end.x;
    int y2 = end.y;

    dx = abs(x1-x2);
    dy = abs(y1-y2);
    if(dy>dx){
        swap(&x1, &y1);
        swap(&x2, &y2);
        swap(&dx, &dy);
        slope = 1;
    }
    if(x1 > x2){
        swap(&x1, &x2);
        swap(&y1, &y2);
    }
    if(y1 > y2){ incry = -1; }
    else{ incry = 1; }

    d = 2*dy-dx;
    incre = 2*dy;
    incrne = 2*(dy-dx);

    while(x1 <= x2){
        if(slope){ pixel(painter, y1, x1, line_color); }
        else{ pixel(painter, x1, y1, line_color); }

        x1++;
        if(d < 0){ d+= incre; }
        else{ y1 += incry; d += incrne; }
    }
}

void line_bresenham(struct paint *painter, struct point start, struct point end, struct color line_color)
{
    int x1 = start.x;
    int y1 = start.y;
    int x2 = end.x;
    int y2 = end.y;

    int dx = abs(x1-x2);
    int dy = abs(y1-y2);
    int steps = dx>dy ? dx : dy;

    int xi = x1 < x2 ? 1 : -1;
    int yi = y1 < y2 ? 1 : -1;

    int err = (dx>dy ? dx : -dy)>>1, e2;
    for(int i = 0; i <= steps; i++){
        pixel(painter, x1, y1, line_color);
        e2 = err;
        if(e2 > -dx){ err -= dy; x1 += xi; }
        if(e2 < dy){ err += dx; y1 += yi; }
    }
}

void line(struct paint *painter, struct point start, struct point end, struct color line_color)
{
    /* dda */
   // line_dda(painter, start, end, line_color);

    /* mid point  */
   // line_midpoint(painter, start, end, line_color);

    /* bresenham   */
    line_bresenham(painter, start, end, line_color);
}

void triangle(struct paint *painter, struct point pos1, struct point pos2, struct point pos3, struct color line_color)
{
    line(painter, pos1, pos2, line_color);
    line(painter, pos2, pos3, line_color);
    line(painter, pos3, pos1, line_color);
}


void plot(struct paint *painter, int cx, int cy, int x, int y, struct color pixel_color)
{
    pixel(painter, cx+x, cy+y, pixel_color); pixel(painter, cx+y, cy+x, pixel_color);
    pixel(painter, cx-x, cy+y, pixel_color); pixel(painter, cx-y, cy+x, pixel_color);
    pixel(painter, cx+x, cy-y, pixel_color); pixel(painter, cx+y, cy-x, pixel_color);
    pixel(painter, cx-x, cy-y, pixel_color); pixel(painter, cx-y, cy-x, pixel_color);
}

void circle(struct paint *painter, int cx, int cy, int radius, struct color circle_color)
{
    int x = 0;
    int y = radius;
    int d = 1 - radius;         /* 1.25 - r */

    plot(painter, cx, cy, x, y, circle_color);

    while(x <= y){
        if(d<0){
            d += 2*x+3;
        }else{
            d += 2*(x-y)+5;
            --y;
        }
        ++x;
        plot(painter, cx, cy, x, y, circle_color);
    }
}

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
    struct point p1 = {int(w/4), int(h/4)};
    struct point p2 = {int(w/2), int(h/4*3)};
    struct point p3 = {int(w/4*3), int(h/4)};
    triangle(painter, p1, p2, p3, triangle_color);

    /* paint a circle */
    struct color circle_color = {124, 22, 57};
    int circle_radius = 20;
    circle(painter, 50, 50, circle_radius, circle_color);
}

int main(int argc, char **argv)
{
  printf("----------POINT RENDER----------\n");

  const int w = 800;
  const int h = 100;

  struct color canvas[w*h] = {0};
  struct paint painter = {.width = w,
                          .height = h,
                          .canvas = canvas };
  render(&painter);

  if (present(&painter)) {
      return -1;
  }

  return 0;

}
