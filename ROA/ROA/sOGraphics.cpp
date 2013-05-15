#include "sOGraphics.h"
#include "globals.h"
#include <iostream>
#include <fstream>

using namespace std;
//initialise graphics

SDL_OpenGL_Graphics::~SDL_OpenGL_Graphics() {
    SDL_Quit();
}

void SDL_OpenGL_Graphics::InitializeVideoMode(int w, int h, int bpp, Uint32 flags)
{

	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
	    fprintf( stderr, "Video initialization failed: %s\n",
		     SDL_GetError( ) );
	    exit(1);
	}
	/* Fetch the video info */
    const SDL_VideoInfo *videoInfo = SDL_GetVideoInfo( );

    if ( !videoInfo )
	{
	    fprintf( stderr, "Video query failed: %s\n",
		     SDL_GetError( ) );
		exit(1);
	}
	/* This checks to see if surfaces can be stored in memory */
    if ( videoInfo->hw_available )
	flags |= SDL_HWSURFACE;
    else
	flags |= SDL_SWSURFACE;

    /* This checks if hardware blits can be done */
    if ( videoInfo->blit_hw )
	flags |= SDL_HWACCEL;
	/* Sets up OpenGL double buffering */
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    /* get a SDL surface */
    screen = SDL_SetVideoMode(w, h, bpp,flags);

	/* Verify there is a surface */
    if ( !screen )
	{
	    fprintf( stderr,  "Video mode set failed: %s\n", SDL_GetError( ) );
	    exit(1);
	}

	if(!resizeWindow(w,h)) exit(2);
	if(!InitGL()) exit(1);
	
}

bool SDL_OpenGL_Graphics::InitGL(){
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	return true;
}
bool SDL_OpenGL_Graphics::resizeWindow( int width, int height )
{
    /* Height / width ration */
    GLfloat ratio;

    /* Protect against a divide by zero */
    if ( height == 0 )
	height = 1;

    ratio = ( GLfloat )width / ( GLfloat )height;

    /* Setup our viewport. */
    glViewport( 0, 0, ( GLint )width, ( GLint )height );

    /* change to the projection matrix and set our viewing volume. */
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );

    /* Set our perspective */
    gluPerspective( 45.0f, ratio, 0.1f, 100.0f );

    /* Make sure we're chaning the model view and not the projection */
    glMatrixMode( GL_MODELVIEW );

    /* Reset The View */
    glLoadIdentity( );

    return true;
}

void SDL_OpenGL_Graphics::SetBgColor(int r, int g, int b) {
    SDL_MapRGB(screen -> format, r,g,b);
}

void SDL_OpenGL_Graphics::ShowText(const char* txt, int x, int y) {
    TTF_SetFontStyle(font, TTF_STYLE_BOLD);//|TTF_STYLE_ITALIC);
    textSurface=TTF_RenderText_Solid(font,txt,textColor);
    PutImage(textSurface, x, y);
    SDL_FreeSurface(textSurface);
}

void SDL_OpenGL_Graphics::SetTextColor(int r, int g, int b) {
    textColor.r = r;
    textColor.g = g;
    textColor.b = b;
}

SDL_Surface* SDL_OpenGL_Graphics::LoadImage(const char *filename) {
    SDL_Surface *loadedImage = NULL;
    SDL_Surface *optimised = NULL;

    if((loadedImage = IMG_Load(filename)) == NULL) printf("%d", IMG_GetError());

    if(loadedImage != NULL) {
        optimised = SDL_DisplayFormat(loadedImage);
        SDL_FreeSurface(loadedImage);

    }
    return optimised;
}

SDL_Surface* SDL_OpenGL_Graphics::LoadImageTrans(const char *filename, int transR, int transG, int transB) {
    SDL_Surface *loadedImage = NULL;
    SDL_Surface *optimised = NULL;

    if((loadedImage = IMG_Load(filename)) == NULL) printf("%d", IMG_GetError());

    if(loadedImage != NULL) {
        Uint32 colorkey;
        colorkey = SDL_MapRGB(screen -> format, transR, transG, transB);
        SDL_SetColorKey(loadedImage, SDL_SRCCOLORKEY, colorkey);
        optimised = SDL_DisplayFormat(loadedImage);
        SDL_FreeSurface(loadedImage);
    }
    return optimised;
}

SDL_Surface* SDL_OpenGL_Graphics::LoadImageAlpha(const char *filename) {
    SDL_Surface *loadedImage = NULL;
    SDL_Surface *optimised = NULL;

    if((loadedImage = IMG_Load(filename)) == NULL) printf("%d", IMG_GetError());

    if(loadedImage != NULL) {
        optimised = SDL_DisplayFormatAlpha(loadedImage);
        SDL_FreeSurface(loadedImage);
    }
    return optimised;
}

void SDL_OpenGL_Graphics::PutImage(SDL_Surface *src, SDL_Surface *dst,SDL_Rect *clip, int x, int y) {
    static SDL_Rect dstrect;
    dstrect.x = x;
    dstrect.y = y;

    SDL_BlitSurface(src, clip, dst, &dstrect);
}

void SDL_OpenGL_Graphics::PutImage(SDL_Surface *src, int x, int y) {
    static SDL_Rect dstrect;
    dstrect.x = x;
    dstrect.y = y;

    SDL_BlitSurface(src, NULL, screen, &dstrect);
}

void SDL_OpenGL_Graphics::ClearScreen() {
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen -> format, 0,0,0));
}

void SDL_OpenGL_Graphics::ClearScreen(int x, int y,int w, int h) {
    SDL_Rect bclip;
    bclip.x = x;
    bclip.y = y;
    bclip.w = w;
    bclip.h = h;
    SDL_FillRect(screen, &bclip, SDL_MapRGB(screen -> format, 0,0,0));
}

void SDL_OpenGL_Graphics::FlipDrawing() {
    //SDL_Flip(screen);
	SDL_GL_SwapBuffers( );


}

///3d object manipulation

ModelData* SDL_OpenGL_Graphics::LoadModel(const char *filename){
     ModelData *object = new ModelData;
	 int i;
     int temp,textwidth,textheight,numvertices,numtexcoord,numtriangles,numframes;
     int textureoffset,coordoffset,triangleoffset,frameoffset;
     
	 ifstream file;
	 file.open(filename,ios::binary);
	 
	 if(!file)
	 {
		 //fstream file("error_log.mk");
		 //file.write("Model File not found", 20);
		 exit(FILE_NOT_FOUND);	 
	 }
     
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
     
     
     
     object->texcoor=new texcoord[numtexcoord];
     file.seekg(coordoffset,ios::beg);
     for (int i=0;i<numtexcoord;i++)
        {
        short temp1;
        file.read ((char*)&temp1,sizeof(short));
        object->texcoor[i].u=(float)temp1/textwidth;
        file.read ((char*)&temp1,sizeof(short));
        object->texcoor[i].v=1-(float)temp1/textheight;
        }
     
     file.seekg(triangleoffset,ios::beg);
	 
     object->tri=new triangles[numtriangles];
	 object->tex=new textures[numtriangles];
	 object->numtri=numtriangles;
     for (i=0;i<numtriangles;i++)
        {
         file.read((char*)&object->tri[i].a,sizeof(short));
         file.read((char*)&object->tri[i].b,sizeof(short));
         file.read((char*)&object->tri[i].c,sizeof(short));
         file.read((char*)&object->tex[i].d,sizeof(short));
         file.read((char*)&object->tex[i].e,sizeof(short));
         file.read((char*)&object->tex[i].f,sizeof(short));                        
        }
        
     file.seekg(frameoffset,ios::beg);
	 
     object->f=new frame[numframes];    
	 object->numframe=numframes;
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
     
     object->f[k].v=new vertex[numvertices]; 
	 unsigned char a;
     for (i=0;i<numvertices;i++)
        {
        file.read((char*)&a,sizeof(unsigned char));
        object->f[k].v[i].x=(tx+(sx*(float)a));
        file.read((char*)&a,sizeof(unsigned char));
        object->f[k].v[i].y=(ty+(sy*a));
        file.read((char*)&a,sizeof(unsigned char));
        object->f[k].v[i].z=(tz+(sz*a));
        file.read((char*)&a,sizeof(unsigned char));
        }
     }   
file.close();    
return object; 
}


GLuint* SDL_OpenGL_Graphics::LoadTexture(const char* filename)
{
	  GLuint *texture = new GLuint; 
	  glEnable(GL_TEXTURE_2D);
	  AUX_RGBImageRec *TextureImage[1];
      memset(TextureImage,0,sizeof(void *)*1);
      TextureImage[0]=auxDIBImageLoad(filename);
	  if(!TextureImage[0])
	  {
		 //fstream file("error_log.mk");
		 //file.write("Texture File not found", 20);
		 exit(FILE_NOT_FOUND);
	  }

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

void SDL_OpenGL_Graphics::DrawModel(ModelData *objectp,GLuint *texture,int j,int k,GLfloat framebuffer)
{
	glColor3f(1.0f ,1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[0]);
    GLfloat currx,curry,currz;


	glBegin(GL_TRIANGLES);
    for (int i=0;i<objectp->numtri;i++)
        {
        //glTranslatef(0.0f,0.0f,5.0f);
        currx=(GLfloat)((objectp->f[j].v[objectp->tri[i].a].x)*(1.0-framebuffer)+(objectp->f[k].v[objectp->tri[i].a].x)*(framebuffer));
		curry=(GLfloat)((objectp->f[j].v[objectp->tri[i].a].y)*(1.0-framebuffer)+(objectp->f[k].v[objectp->tri[i].a].y)*(framebuffer));
		currz=(GLfloat)((objectp->f[j].v[objectp->tri[i].a].z)*(1.0-framebuffer)+(objectp->f[k].v[objectp->tri[i].a].z)*(framebuffer));
        glTexCoord2f(objectp->texcoor[objectp->tex[i].d].u,objectp->texcoor[objectp->tex[i].d].v);
        //glTexCoord2f(object.t[i].u,object.t[i].v);
        glVertex3f(currx,curry,currz);

		currx=(GLfloat)((objectp->f[j].v[objectp->tri[i].b].x)*(1.0-framebuffer)+(objectp->f[k].v[objectp->tri[i].b].x)*(framebuffer));
		curry=(GLfloat)((objectp->f[j].v[objectp->tri[i].b].y)*(1.0-framebuffer)+(objectp->f[k].v[objectp->tri[i].b].y)*(framebuffer));
		currz=(GLfloat)((objectp->f[j].v[objectp->tri[i].b].z)*(1.0-framebuffer)+(objectp->f[k].v[objectp->tri[i].b].z)*(framebuffer));
        glTexCoord2f(objectp->texcoor[objectp->tex[i].e].u,objectp->texcoor[objectp->tex[i].e].v);
         //glTexCoord2f(1.0,0.0);
        glVertex3f(currx,curry,currz);
        
		currx=(GLfloat)((objectp->f[j].v[objectp->tri[i].c].x)*(1.0-framebuffer)+(objectp->f[k].v[objectp->tri[i].c].x)*(framebuffer));
		curry=(GLfloat)((objectp->f[j].v[objectp->tri[i].c].y)*(1.0-framebuffer)+(objectp->f[k].v[objectp->tri[i].c].y)*(framebuffer));
		currz=(GLfloat)((objectp->f[j].v[objectp->tri[i].c].z)*(1.0-framebuffer)+(objectp->f[k].v[objectp->tri[i].c].z)*(framebuffer));
		glTexCoord2f(objectp->texcoor[objectp->tex[i].f].u,objectp->texcoor[objectp->tex[i].f].v);
        // glTexCoord2f(0.5,1.0);
        glVertex3f(currx,curry,currz);
        }
    glEnd();

}

void SDL_OpenGL_Graphics::Translate_Rotate_and_Scale (Vector3D v1,double angle, Vector3D v2)
{
		glPopMatrix();
		glPushMatrix();
		glTranslatef((GLfloat)v1.x, (GLfloat)v1.y, (GLfloat)v1.z);
		glRotatef((GLfloat)angle,0.0,1.0,0.0);
		glScalef((GLfloat)v2.x, (GLfloat)v2.y, (GLfloat)v2.z);
}

