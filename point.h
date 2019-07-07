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
