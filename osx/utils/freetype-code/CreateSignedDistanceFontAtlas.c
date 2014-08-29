#include "freetype-gl.h"
#include "FreeImage.h"
//#include <ft2build.h>
//#include <freetype.h>
#include "edtaa3func.h"

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

int PADDING = 50;



unsigned char * make_distance_map( unsigned char *img, unsigned int width, unsigned int height ) {
    short * xdist = (short *)  malloc( width * height * sizeof(short) );
    short * ydist = (short *)  malloc( width * height * sizeof(short) );
    double * gx   = (double *) calloc( width * height, sizeof(double) );
    double * gy      = (double *) calloc( width * height, sizeof(double) );
    double * data    = (double *) calloc( width * height, sizeof(double) );
    double * outside = (double *) calloc( width * height, sizeof(double) );
    double * inside  = (double *) calloc( width * height, sizeof(double) );
    int i;

    // Convert img into double (data)
    double img_min = 255, img_max = -255;
    for( i=0; i<width*height; ++i)
    {
        double v = img[i];
        data[i] = v;
        if (v > img_max) img_max = v;
        if (v < img_min) img_min = v;
    }
    // Rescale image levels between 0 and 1
    for( i=0; i<width*height; ++i)
    {
        data[i] = (img[i]-img_min)/img_max;
    }

    // Compute outside = edtaa3(bitmap); % Transform background (0's)
    computegradient( data, height, width, gx, gy);
    edtaa3(data, gx, gy, height, width, xdist, ydist, outside);
    for( i=0; i<width*height; ++i)
        if( outside[i] < 0 )
            outside[i] = 0.0;

    // Compute inside = edtaa3(1-bitmap); % Transform foreground (1's)
    memset(gx, 0, sizeof(double)*width*height );
    memset(gy, 0, sizeof(double)*width*height );
    for( i=0; i<width*height; ++i)
        data[i] = 1 - data[i];
    computegradient( data, height, width, gx, gy);
    edtaa3(data, gx, gy, height, width, xdist, ydist, inside);
    for( i=0; i<width*height; ++i)
        if( inside[i] < 0 )
            inside[i] = 0.0;

    // distmap = outside - inside; % Bipolar distance field
    unsigned char *out = (unsigned char *) malloc( width * height * sizeof(unsigned char) );
    for( i=0; i<width*height; ++i)
    {
        outside[i] -= inside[i];
        outside[i] = 128+outside[i]*16;
        if( outside[i] < 0 ) outside[i] = 0;
        if( outside[i] > 255 ) outside[i] = 255;
        out[i] = 255 - (unsigned char) outside[i];
        //out[i] = (unsigned char) outside[i];
    }

    free( xdist );
    free( ydist );
    free( gx );
    free( gy );
    free( data );
    free( outside );
    free( inside );
    return out;
}


void writeFNT(texture_font_t * font, char* fn, char* tn) {

  //open file for writing

  FILE *fp;
  fp=fopen(fn, "w");

  //font info

  char* filename = font->filename;
  float size = font->size;
  //int hinting = font->hinting;
  float height = font->height;
  //float linegap = font->linegap;
  //float ascender = font->ascender;
  float descender = font->descender;

  fprintf(fp, "info face=\"%s\" size=%f bold=0 italic=0 charset=\"\" unicode=0 stretchH=%f smooth=1 aa=1 padding=0,0,0,0 spacing=0,0\n", filename, size, height );


  //atlas info

  texture_atlas_t * atlas = font->atlas;

  int aw = (int)atlas->width;
  int ah = (int)atlas->height;

  fprintf(fp, "common lineHeight=%d base=%d scaleW=%d scaleH=%d pages=0 packed=0 padding=%d\n", (int)height, (int)descender, aw, ah, PADDING );
  fprintf(fp, "page id=0 file=\"%s\"\n", filename);
  fprintf(fp, "chars count=%d\n", (int)font->glyphs->size-1);

  //glyph info

  size_t i ;
  for ( i = 1; i < font->glyphs->size; i++) {
    texture_glyph_t* glyph = *(texture_glyph_t **) vector_get( font->glyphs, i );

    char charcode = glyph->charcode;
    int gx = (int) (glyph->s0 * aw);
    int gy = (int) (glyph->t1 * ah);
    float gs0 = (float) (glyph->s0);
    float gs1 = (float) (glyph->s1);
    float gt0 = (float) (glyph->t0);
    float gt1 = (float) (glyph->t1);

    int gw = (int)glyph->width;
    int gh = (int)glyph->height;
    int goff_x = (int)glyph->offset_x;
    int goff_y = (int)glyph->offset_y;
    int gadvance_x = (int)glyph->advance_x;
    //int gadvance_y = (int)glyph->advance_y;

    //will deal with kerning later...

    fprintf(fp, "char id=%d ", (int) charcode);
    fprintf(fp, "x=%d ", gx);
    fprintf(fp, "y=%d ", gy);
    fprintf(fp, "s0=%f ", gs0);
    fprintf(fp, "s1=%f ", gs1);
    fprintf(fp, "t0=%f ", gt0);
    fprintf(fp, "t1=%f ", gt1);

    fprintf(fp, "width=%d ", gw);
    fprintf(fp, "height=%d ", gh);
    fprintf(fp, "xoffset=%d ", goff_x);
    fprintf(fp, "yoffset=%d ", goff_y);
    fprintf(fp, "xadvance=%d ", gadvance_x);
    fprintf(fp, "letter=\"%c\" \n", charcode);
  }

  //close file
  fclose(fp);

// size_t i ;
//  for ( i = 1; i < font->glyphs->size; i++) {
//    texture_glyph_t* glyph = *(texture_glyph_t **) vector_get( font->glyphs, i );

}

void writePNG(texture_atlas_t * atlas, char* fn) {
  printf("atlas depth = %d\n", (int)atlas->depth);

  int stride = (atlas->depth - 0)*atlas->width;
  int numBits = 8*(atlas->depth-0);
  FIBITMAP* img0 = FreeImage_ConvertFromRawBits(
      atlas->data, atlas->width, atlas->height,
      stride, numBits,
      0xFF0000, 0x00FF00, 0x0000FF, 1);

  FreeImage_SetTransparent(img0, 1);
  GLubyte Transparency[] = {0xFF};
  FreeImage_SetTransparencyTable(img0, Transparency, 1);

  printf("FreeImage_IsTransparent = %d\n", FreeImage_IsTransparent(img0));
  FreeImage_Save(FIF_PNG, img0, fn, 0);
}


void char2wchar(char* in, wchar_t* out, int length) {

  int n;

  for (n = 0; n < length-1; n++) {
    mbtowc( &out[n], &in[n], MB_CUR_MAX );
  }

  out[length-1] = L'\0';
  printf("a1: wide character strdata: %ls\n", out);
}

int main( int argc, char **argv ) {

  if ( argc != 4 && argc != 5 && argc != 6)
  {
    printf( "\n\nexample usage:\t%s Vera.ttf 32 256\n\nwill make the two files \"Vera.png\" and \"Vera.fnt\" \nthat represent the font atlas with a texture size of 256x256 for all characters in the font \"Vera.ttf\" with size 32, \nand which can then be loaded via the aluminum library.\n\n", argv[0] );
    printf( "\nexample usage:\t%s Vera.ttf 24 1024 myFont \n\nwill make the two files \"myFont.png\" and \"myFont.fnt\" \nthat represent the font atlas with a texture size of 1024x1024 for all characters in the font \"Vera.ttf\" with size 24, \nand which can then be loaded via the aluminum library.\n\n", argv[0] );
    printf( "\nexample usage:\t%s Vera.ttf 24 1024 myFont \"abc123\" \n\nwill make the two files \"myFont.png\" and \"myFont.fnt\" \nthat represent the font atlas with a texture size of 1024x1024 for only the characters \"abc123\" in the font \"Vera.ttf\" with size 24, \nand which can then be loaded via the aluminum library.\n\n", argv[0] );
    exit(0);
  }

  char inFileTTF[50];
  char outFilePNG[50];
  char outFileFNT[50];

  sprintf( inFileTTF, "%s", argv[1] );
  size_t fontsize = (size_t) atoi(argv[2]);
  size_t texturesize = (size_t) atoi(argv[3]);

  char* glyphs = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";


  if (argc == 4) {
    sprintf( outFilePNG, "%s.png", argv[1] );
    sprintf( outFileFNT, "%s.fnt", argv[1] );
  } else {
    char s[50];
    strcpy(s, argv[4]);
    char* token = strtok(s, ".");
    sprintf( outFilePNG, "%s.png", token );
    sprintf( outFileFNT, "%s.fnt", token );

    if (argc == 6) {
      glyphs = argv[5];
    }
  }

  printf("\n\n%s %s %d %d %s %s\n\n", argv[0], inFileTTF, (int)fontsize, (int)texturesize, outFilePNG, outFileFNT);

  //we need to actually create the glut window in order to get the GLContext, since the texture atlas uses a gl texture as its backing store.
  glutInit( &argc, argv );
  glutInitWindowSize( 1,1 );
  glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
  glutCreateWindow( "" );

  texture_atlas_t * atlas = texture_atlas_new( texturesize, texturesize, 1 );
  const char *filename = inFileTTF;
  int length = strlen(glyphs) + 1;
  wchar_t cache[length];

  char2wchar(glyphs, cache, length);

  //printf("2: wide character strdata: %ls\n", cache);

  texture_font_t * font = texture_font_new( atlas, filename, fontsize );
  texture_font_load_glyphs( font, cache, PADDING );

   fprintf( stderr, "Generating distance map...\n" );

    unsigned char *map;

    map = make_distance_map(atlas->data, atlas->width, atlas->height);
    fprintf( stderr, "done !\n");

    memcpy( atlas->data, map, atlas->width*atlas->height*sizeof(unsigned char) );
    free(map);
    texture_atlas_upload( atlas );


  //Save .png file
  writePNG(atlas, outFilePNG);

  //Save .fnt file
  writeFNT(font, outFileFNT, outFilePNG);

  //some debug info
  printf( "Texture size               : %ldx%ld\n", atlas->width, atlas->height );
  printf( "Texture occupancy          : %.2f%%\n", 100.0*atlas->used/(float)(atlas->width*atlas->height) );

  return 0;
}
