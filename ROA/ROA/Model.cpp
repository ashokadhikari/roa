#include "Model.h"

Model* LnR::LoadModel(const char* model_name)
{
	 int i;
	 Model *M = new Model;
	 //GLuint *Text = new GLuint;
     int temp,textwidth,textheight,numvertices,numtexcoord,numtriangles,numframes;
     int textureoffset,coordoffset,triangleoffset,frameoffset;
     
	 ifstream file;
	 file.open(model_name,ios::binary);
     
     file.read((char*)&temp,sizeof(int));  //IPD2
     file.read((char*)&temp,4);            //8 
     file.read((char*)&textwidth,4);        //texture width
     file.read((char*)&textheight,4);       //texture height
     file.read((char*)&temp,4);            //no fo bytes per frame
     file.read((char*)&temp,4);            //no of texture
     file.read((char*)&numvertices,4);     //no of vertices
     file.read((char*)&numtexcoord,4);     //no of texture coordinate
     file.read((char*)&numtriangles,4);    //no of triangles
     file.read((char*)&temp,4);            //openGL commands
     file.read((char*)&numframes,4);            //no fo frames
     file.read((char*)&textureoffset,4);   //texture offset
     file.read((char*)&coordoffset,4);     //texture coordinate offset
     file.read((char*)&triangleoffset,4);  //triangle offset
     file.read((char*)&frameoffset,4);     //frame offset
     file.read((char*)&temp,4);            //offset of openGL command
     file.read((char*)&temp,4);            //offset to the end of file
     
     
     
     
     M->TexC = new TextureCord[numtexcoord];
	 
	 file.seekg(coordoffset,ios::beg);
     for (int i=0;i<numtexcoord;i++)
        {
        short temp1;
        file.read ((char*)&temp1,sizeof(short));
        M->TexC[i].u=(float)temp1/textwidth;
        file.read ((char*)&temp1,sizeof(short));
        M->TexC[i].v=1-(float)temp1/textheight;
        }
     
     file.seekg(triangleoffset,ios::beg);
	 
     M->numTri=numtriangles;
	 M->Tri = new Triangle[numtriangles];
	 M->Tex = new Texture[numtriangles];
     for (i=0;i<numtriangles;i++)
        {
         file.read((char*)&M->Tri[i].a,sizeof(short));
         file.read((char*)&M->Tri[i].b,sizeof(short));
         file.read((char*)&M->Tri[i].c,sizeof(short));
         file.read((char*)&M->Tex[i].d,sizeof(short));
         file.read((char*)&M->Tex[i].e,sizeof(short));
         file.read((char*)&M->Tex[i].f,sizeof(short));                        
        }
        
     file.seekg(frameoffset,ios::beg);
	 
     M->numFrames=numframes;
	 M->F = new Frame[numframes];

for (int k=0;k<numframes;k++)
     {    
     float sx,sy,sz,tx,ty,tz;
     file.read((char*)&sx,sizeof(float));
     file.read((char*)&sy,sizeof(float));
     file.read((char*)&sz,sizeof(float));
     file.read((char*)&tx,sizeof(float));
     file.read((char*)&ty,sizeof(float));
     file.read((char*)&tz,sizeof(float));
     char temp5[20];
     file.read(temp5,16);
     
     unsigned char a;
	 M->F[k].V = new Vertex[numvertices];
     for (i=0;i<numvertices;i++)
        {
        file.read((char*)&a,sizeof(unsigned char));
        M->F[k].V[i].x=(tx+(sx*(float)a));
        file.read((char*)&a,sizeof(unsigned char));
        M->F[k].V[i].y=(ty+(sy*a));
        file.read((char*)&a,sizeof(unsigned char));
        M->F[k].V[i].z=(tz+(sz*a));
        file.read((char*)&a,sizeof(unsigned char));
        }
     }
file.close(); 
return M;
   
}

GLuint* LnR::LoadTexture(const char *filename)
{
	 glEnable(GL_TEXTURE_2D);
	 GLuint *texture = new GLuint;

	  AUX_RGBImageRec *TextureImage[1];
      memset(TextureImage,0,sizeof(void *)*1);
      TextureImage[0]=auxDIBImageLoad(L"tex.BMP");

	  glGenTextures(1, texture);
      glBindTexture(GL_TEXTURE_2D, *texture);
      glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
            
      if (TextureImage[0])							// If Texture Exists
	        {
		     if (TextureImage[0]->data)					// If Texture Image Exists
		        {
    			free(TextureImage[0]->data);				// Free The Texture Image Memory
		        }

              free(TextureImage[0]);						// Free The Image Structure
            }
	  return texture;
}

void LnR::RenderModel(Model* M, int frame, GLuint *T)
{
	float currx,curry,currz;
	 glBindTexture(GL_TEXTURE_2D,T[0]);
	glBegin(GL_TRIANGLES);

    for (int i=0;i<M->numTri;i++)
	{
        currx=(M->F[frame].V[M->Tri[i].a].x);//*(1.0-framebuffer)+(M->F[k].V[M->Tri[i].a].x)*(framebuffer);
		curry=(M->F[frame].V[M->Tri[i].a].y);//*(1.0-framebuffer)+(M->F[k].V[M->Tri[i].a].y)*(framebuffer);
		currz=(M->F[frame].V[M->Tri[i].a].z);//*(1.0-framebuffer)+(M->F[k].V[M->Tri[i].a].z)*(framebuffer);
        glTexCoord2f(M->TexC[M->Tex[i].d].u,M->TexC[M->Tex[i].d].v);
        glVertex3f(currx,curry,currz);

		currx=(M->F[frame].V[M->Tri[i].b].x);//*(1.0-framebuffer)+(M->F[k].V[M->Tri[i].b].x)*(framebuffer);
		curry=(M->F[frame].V[M->Tri[i].b].y);//*(1.0-framebuffer)+(M->F[k].V[M->Tri[i].b].y)*(framebuffer);
		currz=(M->F[frame].V[M->Tri[i].b].z);//*(1.0-framebuffer)+(M->F[k].V[M->Tri[i].b].z)*(framebuffer);
        glTexCoord2f(M->TexC[M->Tex[i].e].u,M->TexC[M->Tex[i].e].v);
        glVertex3f(currx,curry,currz);
        
		currx=(M->F[frame].V[M->Tri[i].c].x);//*(1.0-framebuffer)+(M->F[k].V[M->Tri[i].c].x)*(framebuffer);
		curry=(M->F[frame].V[M->Tri[i].c].y);//*(1.0-framebuffer)+(M->F[k].V[M->Tri[i].c].y)*(framebuffer);
		currz=(M->F[frame].V[M->Tri[i].c].z);//*(1.0-framebuffer)+(M->F[k].V[M->Tri[i].c].z)*(framebuffer);
		glTexCoord2f(M->TexC[M->Tex[i].f].u,M->TexC[M->Tex[i].f].v);
        glVertex3f(currx,curry,currz);
    }
    glEnd();

}