#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct color { unsigned char r,g,b; };
struct point { int x,y; };
struct paint { int width; int height; struct color *canvas; };
struct vector3f { float x, y, z; };

#define MAX(a, b) ((a)>(b) ? (a) : (b))
#define MIN(a, b) ((a)<(b) ? (a) : (b))

/* utility */
void swap(int *a, int *b)
{
  int t = *a;
  *a = *b;
  *b = t;
}

void swap_point(struct point *a, struct point *b)
{
  struct point *t = a;
  a = b;
  b = t;
}

void cross3f(struct vector3f *a, struct vector3f *b, struct vector3f *result)
{
  result->x = a->y * b->z - a->z * b->y;
  result->y = a->z * b->x - a->x * b->z;
  result->z = a->x * b->y - a->y * b->x;
}

void barycentric(struct point a, struct point b, struct point c, struct point p, struct vector3f *result)
{
  struct vector3f u;
  struct vector3f vec_x = { c.x-a.x, b.x-a.x, a.x-p.x };
  struct vector3f vec_y = { c.y-a.y, b.y-a.y, a.y-p.y };
  cross3f(&vec_x, &vec_y, &u);
  if (abs(u.z) < 1){
    result->x = -1;
    result->y = 1;
    result->z = 1;
  }
  else
  {
    result->x = 1.f - (u.x+u.y)/u.z;
    result->y = u.y/u.z;
    result->z = u.x/u.z;
  }
}


void pixel(struct paint *painter, int x, int y, struct color pixel_color)
{
  if(x >= painter->width || y >= painter->height){
    /* clip replace later */
    return;
  }
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

  float x = (float)start.x + 0.5f;
  float y = (float)start.y + 0.5f;
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

void triangle_wireframe(struct paint *painter, struct point p1, struct point p2, struct point p3, struct color line_color)
{
  line(painter, p1, p2, line_color);
  line(painter, p2, p3, line_color);
  line(painter, p3, p1, line_color);
}

void triangle_scan(struct paint *painter, struct point p1, struct point p2, struct point p3, struct color painter_color)
{
  /* edge walking */
  if(p1.y == p2.y && p1.y == p3.y) return;
  if(p1.y > p2.y) swap_point(&p1, &p2);
  if(p1.y > p3.y) swap_point(&p1, &p3);
  if(p2.y > p3.y) swap_point(&p2, &p3);

  struct point vec12 = {p2.x-p1.x, p2.y-p1.y};
  struct point vec13 = {p3.x-p1.x, p3.y-p1.y};
  struct point vec23 = {p3.x-p2.x, p3.y-p2.y};

  int total_height = p3.y - p1.y;
  int top_height = p2.y - p1.y;
  int bottom_height = p3.y - p2.y;
  struct point a, b;
  for(int i = 0; i < total_height; i++){
    bool is_bottom = i > top_height || top_height == 0;
    int base_height = is_bottom ? bottom_height : top_height;
    float alpha = (float)i / total_height;
    float beta = (float)(i - (is_bottom ? top_height : 0)) / base_height;
    a.x = p1.x+vec13.x*alpha;
    a.y = p1.y+vec13.y*alpha;
    if(is_bottom){
      b.x = p2.x+vec23.x*beta;
      b.y = p2.y+vec23.y*beta;
    }
    else{
      b.x = p1.x+vec12.x*beta;
      b.y = p1.y+vec12.y*beta;
    }
    int step = a.x < b.x ? 1 : -1;
    int y = p1.y + i;
    while(a.x != b.x){
      pixel(painter, a.x, y, painter_color);
      a.x += step;
    }
  }
}

void triangle_equa(struct paint *painter, struct point p1, struct point p2, struct point p3, struct color painter_color)
{
  /* edge equations: barycentric coordinates*/
  struct point bbox_min = {MIN(p1.x, MIN(p2.x, p3.x)), MIN(p1.y, MIN(p2.y, p3.y))};
  struct point bbox_max = {MAX(p1.x, MAX(p2.x, p3.x)), MAX(p1.y, MAX(p2.y, p3.y))};

  struct point p;
  struct vector3f bary_coord;
  for (p.y = bbox_min.y; p.y < bbox_max.y; p.y++){
    for (p.x = bbox_min.x; p.x< bbox_max.x; p.x++){
      barycentric(p1, p2, p3, p, &bary_coord);
      if (bary_coord.x < 0 || bary_coord.y < 0 || bary_coord.z < 0)
        continue;
      /* bary_coord could be used to interpolate color normal and other attribute. */
      pixel(painter, p.x, p.y, painter_color);
    }
  }
}

void triangle(struct paint *painter, struct point p1, struct point p2, struct point p3, struct color painter_color)
{
  // triangle_wireframe(painter, p1, p2, p3, painter_color);
  // triangle_scan(painter, p1, p2, p3, painter_color);
  triangle_equa(painter, p1, p2, p3, painter_color);
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
