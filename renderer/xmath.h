#ifndef P_XMATH_H
#define P_XMATH_H

#include <math.h>

typedef struct { float x, y; } vec2;
typedef struct { float x, y, z; } vec3;
typedef struct { float x, y, z, w; } vec4;
typedef struct { float m[4][4]; } mat4x4;

#define XMATH_MIN(a, b) ((a)<(b) ? (a) : (b))
#define XMATH_MAX(a, b) ((a)>(b) ? (a) : (b))


#define XMATH_VEC_FUNC_DEF(n) \
static void vec##n##_min(const vec##n *a, const vec##n *b, vec##n *r); \
static void vec##n##_max(const vec##n *a, const vec##n *b, vec##n *r); \
static void vec##n##_add(const vec##n *a, const vec##n *b, vec##n *r); \
static void vec##n##_sub(const vec##n *a, const vec##n *b, vec##n *r); \
static void vec##n##_mul(const vec##n *v, const float s, vec##n *r); \
static void vec##n##_div(const vec##n *v, const float d, vec##n *r); \
static float vec##n##_dot(const vec##n *a, const vec##n *b); \
static float vec##n##_length(const vec##n *v); \
static void vec##n##_normal(const vec##n *v, vec##n *r);

XMATH_VEC_FUNC_DEF(2);

XMATH_VEC_FUNC_DEF(3);
static void vec3_cross(const vec3 *a, const vec3 *b, vec3 *r);

XMATH_VEC_FUNC_DEF(4);
static void vec4_cross(const vec4 *a, const vec4 *b, vec4 *r);

static void mat4x4_identity(mat4x4 *m);
static void mat4x4_dup(mat4x4 *m, mat4x4 *n);

/* vec2 imp */

static
void vec2_min(const vec2 *a, const vec2 *b, vec2 *r)
{
  r->x = XMATH_MIN(a->x, b->x);
  r->y = XMATH_MIN(a->y, b->y);
}

static
void vec2_max(const vec2 *a, const vec2 *b, vec2 *r)
{
  r->x = XMATH_MAX(a->x, b->x);
  r->y = XMATH_MAX(a->y, b->y);
}

static
void vec2_add(const vec2 *a, const vec2 *b, vec2 *r)
{
  r->x = a->x + b->x;
  r->y = a->y + b->y;
}

static
void vec2_sub(const vec2 *a, const vec2 *b, vec2 *r)
{
  r->x = a->x - b->x;
  r->y = a->y - b->y;
}

static
void vec2_mul(const vec2 *v, const float s, vec2 *r)
{
  r->x = v->x * s;
  r->y = v->y * s;
}

static
void vec2_div(const vec2 *v, const float d, vec2 *r)
{
  vec2_mul(v, 1.f/d, r);
}

static
float vec2_dot(const vec2 *a, const vec2 *b)
{
  return a->x * b->x + a->y * b->y;
}

static
float vec2_length(const vec2 *v)
{
  return sqrtf(vec2_dot(v, v));
}

static
void vec2_normal(const vec2 *v, vec2 *r)
{
  vec2_div(v, vec2_length(v), r);
}


/* vec3 imp */

static
void vec3_min(const vec3 *a, const vec3 *b, vec3 *r)
{
  r->x = XMATH_MIN(a->x, b->x);
  r->y = XMATH_MIN(a->y, b->y);
  r->z = XMATH_MIN(a->z, b->z);
}

static
void vec3_max(const vec3 *a, const vec3 *b, vec3 *r)
{
  r->x = XMATH_MAX(a->x, b->x);
  r->y = XMATH_MAX(a->y, b->y);
  r->z = XMATH_MAX(a->z, b->z);
}

static
void vec3_add(const vec3 *a, const vec3 *b, vec3 *r)
{
  r->x = a->x + b->x;
  r->y = a->y + b->y;
  r->z = a->z + b->z;
}

static
void vec3_sub(const vec3 *a, const vec3 *b, vec3 *r)
{
  r->x = a->x - b->x;
  r->y = a->y - b->y;
  r->z = a->z - b->z;
}

static
void vec3_mul(const vec3 *v, const float s, vec3 *r)
{
  r->x = v->x * s;
  r->y = v->y * s;
  r->z = v->z * s;
}

static
void vec3_div(const vec3 *v, const float d, vec3 *r)
{
  vec3_mul(v, 1.f/d, r);
}

static
float vec3_dot(const vec3 *a, const vec3 *b)
{
  return a->x * b->x + a->y * b->y + a->z * b->z;
}

static
float vec3_length(const vec3 *v)
{
  return sqrtf(vec3_dot(v, v));
}

static
void vec3_normal(const vec3 *v, vec3 *r)
{
  vec3_div(v, vec3_length(v), r);
}

static
void vec3_cross(const vec3 *a, const vec3 *b, vec3 *r)
{
  r->x = a->y * b->z - a->z * b->y;
  r->y = a->z * b->x - a->x * b->z;
  r->z = a->x * b->y - a->y * b->x;
}


/* vec4 imp */

static
void vec4_min(const vec4 *a, const vec4 *b, vec4 *r)
{
  r->x = XMATH_MIN(a->x, b->x);
  r->y = XMATH_MIN(a->y, b->y);
  r->z = XMATH_MIN(a->z, b->z);
  r->w = XMATH_MIN(a->w, b->w);
}

static
void vec4_max(const vec4 *a, const vec4 *b, vec4 *r)
{
  r->x = XMATH_MAX(a->x, b->x);
  r->y = XMATH_MAX(a->y, b->y);
  r->z = XMATH_MAX(a->z, b->z);
  r->w = XMATH_MAX(a->w, b->w);
}

static
void vec4_add(const vec4 *a, const vec4 *b, vec4 *r)
{
  r->x = a->x + b->x;
  r->y = a->y + b->y;
  r->z = a->z + b->z;
  r->w = a->w + b->w;
}

static
void vec4_sub(const vec4 *a, const vec4 *b, vec4 *r)
{
  r->x = a->x - b->x;
  r->y = a->y - b->y;
  r->z = a->z - b->z;
  r->w = a->w - b->w;
}

static
void vec4_mul(const vec4 *v, const float s, vec4 *r)
{
  r->x = v->x * s;
  r->y = v->y * s;
  r->z = v->z * s;
  r->w = v->w * s;
}

static
void vec4_div(const vec4 *v, const float d, vec4 *r)
{
  vec4_mul(v, 1.f/d, r);
}

static
float vec4_dot(const vec4 *a, const vec4 *b)
{
  return a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w;
}

static
float vec4_length(const vec4 *v)
{
  return sqrtf(vec4_dot(v, v));
}

static
void vec4_normal(const vec4 *v, vec4 *r)
{
  vec4_div(v, vec4_length(v), r);
}

static
void vec4_cross(const vec4 *a, const vec4 *b, vec4 *r)
{
  r->x = a->y * b->z - a->z * b->y;
  r->y = a->z * b->x - a->x * b->z;
  r->z = a->x * b->y - a->y * b->x;
  r->w = 1.f;
}


/* mat4x4 imp  */
static
void mat4x4_identity(mat4x4 *m)
{
  int i, j;
  for (i = 0; i < 4; i++){
    for ( j = 0; j < 4; j++){
      m->m[i][j] = i == j ? 1.f : 0.f;
    }
  }
}

static
void mat4x4_dup(mat4x4 *m, mat4x4 *n)
{
  int i, j;
  for (i = 0; i < 4; i++){
    for (j = 0; j < 4; j++){
      m->m[i][j] = n->m[i][j];
    }
  }
}






#endif