#include <limits.h>
#include <math.h>
#include "point.h"
#include "ply-1.1/ply.h"

typedef struct {
  float x, y, z;
  float confidence, intensity;
} ply_vertex;

typedef struct {
  unsigned char nverts;
  int *verts;
} ply_face;

typedef struct {
  PlyFile *ply;
  int nelems;
  char **elist;
  int file_type;
  int num_verts;
  ply_vertex **vlist;
  int num_faces;
  ply_face **flist;
  int num_comments;
  char **comments;
  int num_obj_info;
  char **obj_info;
  float version;
} ply_model;

PlyProperty vert_props[] = {
  {"x", PLY_FLOAT, PLY_FLOAT, offsetof(ply_vertex, x), 0, 0, 0, 0},
  {"y", PLY_FLOAT, PLY_FLOAT, offsetof(ply_vertex, y), 0, 0, 0, 0},
  {"z", PLY_FLOAT, PLY_FLOAT, offsetof(ply_vertex, z), 0, 0, 0, 0},
  {"confidence", PLY_FLOAT, PLY_FLOAT, offsetof(ply_vertex, confidence), 0, 0, 0, 0},
  {"intensity", PLY_FLOAT, PLY_FLOAT, offsetof(ply_vertex, intensity), 0, 0, 0, 0},
};

PlyProperty face_props[] = {
  {"vertex_indices", PLY_INT, PLY_INT, offsetof(ply_face, verts), 1, PLY_UCHAR, PLY_UCHAR, offsetof(ply_face, nverts)}
};

#define CANVAS_COORD(a, b) ((a)*(b)*0.5)

void read_ply(char *name, ply_model *md)
{
  md->ply = ply_open_for_reading(name, &md->nelems, &md->elist, &md->file_type, &md->version);
  int num_elems;
  char *elem_name;
  int nprops;
  PlyProperty **plist;
  for(int i = 0; i < md->nelems; i++){
    elem_name = md->elist[i];
    plist = ply_get_element_description(md->ply, elem_name, &num_elems, &nprops);
    /* read vertex elements */
    if(equal_strings("vertex", elem_name)){
      md->num_verts = num_elems;
      md->vlist = (ply_vertex **) malloc (sizeof(ply_vertex *) * num_elems);

      ply_get_property(md->ply, elem_name, &vert_props[0]);
      ply_get_property(md->ply, elem_name, &vert_props[1]);
      ply_get_property(md->ply, elem_name, &vert_props[2]);
      ply_get_property(md->ply, elem_name, &vert_props[3]);
      ply_get_property(md->ply, elem_name, &vert_props[4]);

      /* grap all vertex elements */
      for(int j = 0; j < num_elems; j++){
        md->vlist[j] = (ply_vertex *) malloc (sizeof(ply_vertex));
        ply_get_element(md->ply, (void *)md->vlist[j]);
      }
    }

    /* read face elements */
    if(equal_strings("face", elem_name)){
      md->num_faces = num_elems;
      md->flist = (ply_face **) malloc (sizeof(ply_face *) * num_elems);

      ply_get_property(md->ply, elem_name, &face_props[0]);

      /* grap all face elements */
      for(int j = 0; j < num_elems; j++){
        md->flist[j] = (ply_face *) malloc (sizeof(ply_face));
        ply_get_element(md->ply, (void *)md->flist[j]);
      }
    }

    for (int j = 0; j < nprops; j++){
      printf("property %s\n", plist[j]->name);
    }
  }

  /* grap and print out the comments in the file */
  md->comments = ply_get_comments(md->ply, &md->num_comments);
  for(int i = 0; i < md->num_comments; i++){
    printf("comments = '%s' \n", md->comments[i]);
  }

  md->obj_info = ply_get_obj_info(md->ply, &md->num_obj_info);
  for(int i = 0; i < md->num_obj_info; i++){
    printf("obj_info = '%s'\n", md->obj_info[i]);
  }

  ply_close(md->ply);
}

void clear(struct paint *painter, int clear_color)
{
    unsigned int length = 3 * painter->width * painter->height;
    memset(painter->canvas, clear_color, length);
    memset(painter->zbuffer, INT_MIN, length);
}

int present(struct paint *painter)
{
  int length = painter->width * painter->height;
  FILE *color_rt = fopen("image.ppm", "w");
  FILE *depth_rt = fopen("depth.ppm", "w");

  fprintf(color_rt, "P3\n%d %d\n%d\n", painter->width, painter->height, 255 );
  fprintf(depth_rt, "P3\n%d %d\n%d\n", painter->width, painter->height, 255 );
  for (int i = 0; i < length; i++) {
    fprintf( color_rt, "%d %d %d ",painter->canvas[i].r, painter->canvas[i].g, painter->canvas[i].b );
    int depth_color = painter->zbuffer[i] * 255;
    fprintf(depth_rt, "%d %d %d ", depth_color, depth_color, depth_color);
  }

  printf("-----------OUTPUT SUCCESS--------\n");
  fclose(depth_rt);
  return fclose(color_rt);
}

void render(struct paint *painter)
{
  clear(painter, 0);        /* clear canvas */
  int width = painter->width;
  int height = painter->height;

  ply_model md;
  read_ply("../../models/bunny/reconstruction/bun_zipper_res3", &md);

  struct color model_color = {23, 124, 210};
  int scale = 10;               /* similar to model-view transform. matrix replace later */
  int x_off = 1;
  int y_off = 0;
  ply_vertex *v0, *v1, *v2;
  vec3 light = { 0, 0, -1 };
  for(int i = 0; i < md.num_faces; i++){
    v0 = md.vlist[md.flist[i]->verts[0]];
    v1 = md.vlist[md.flist[i]->verts[1]];
    v2 = md.vlist[md.flist[i]->verts[2]];

    vec3 p0 = {CANVAS_COORD(v0->x*scale+x_off, width), CANVAS_COORD(v0->y*scale+y_off, height), v0->z*scale};
    vec3 p1 = {CANVAS_COORD(v1->x*scale+x_off, width), CANVAS_COORD(v1->y*scale+y_off, height), v1->z*scale};
    vec3 p2 = {CANVAS_COORD(v2->x*scale+x_off, width), CANVAS_COORD(v2->y*scale+y_off, height), v2->z*scale};

    vec3 first_vec, second_vec, cross, normal;
    vec3_sub(v1, v0, &first_vec);
    vec3_sub(v2, v0, &second_vec);

    vec3_cross(&first_vec, &second_vec, &cross);
    vec3_normal(&cross, &normal);

    float intensity = vec3_dot(&normal, &light);
    if (intensity < 0) {
        continue;
    }
    model_color.r = intensity * 255;
    model_color.g = intensity * 255;
    model_color.b = intensity * 255;

    rasterize(painter, &p0, &p1, &p2, model_color);
  }
}

int main(int argc, char **argv)
{
  printf("----------POINT RENDER----------\n");

  const int w = 2048;
  const int h = 2048;

  struct color *canvas = malloc(sizeof(struct color) *w*h);
  float *zbuffer = malloc(sizeof(float) * w * h);

  struct paint painter = {.width = w,
                          .height = h,
                          .zbuffer = zbuffer,
                          .canvas = canvas };
  render(&painter);

  if (present(&painter)) {
      return -1;
  }

  free(canvas);
  free(zbuffer);

  return 0;

}
