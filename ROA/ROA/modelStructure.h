#ifndef MODEL_STR
#define MODEL_STR

typedef struct 
  {
       float x,y,z;
  }vertex;
  
typedef struct 
  {
       float u,v;
  }texcoord;
  
typedef struct
  {
       short a,b,c;
  }triangles;
  
typedef struct
   {
   short d,e,f;
   }textures;

typedef struct
    {
    vertex *v;  
    }frame;
  
typedef struct 
  {
  frame *f;
  texcoord *texcoor;
  triangles *tri;
  textures *tex;
  int numtri;
  int numframe;
  }ModelData;


#endif