
#include <stdlib.h>
#include <stdio.h>

#include "imageprocessing.h"

#include <FreeImage.h>

/*
imagem abrir_imagem(char *nome_do_arquivo);
void salvar_imagem(char *nome_do_arquivo);
void liberar_imagem(imagem *i);
 */

imagem abrir_imagem(char *nome_do_arquivo) {
  FIBITMAP *bitmapIn;
  int x, y;
  RGBQUAD color;
  imagem I;

  bitmapIn = FreeImage_Load(FIF_JPEG, nome_do_arquivo, 0);

  if (bitmapIn == 0) {
    printf("Erro! Nao achei arquivo - %s\n", nome_do_arquivo);
  } else {
    printf("Arquivo lido corretamente!\n");
   }

  x = FreeImage_GetWidth(bitmapIn);
  y = FreeImage_GetHeight(bitmapIn);

  I.width = x;
  I.height = y;
  I.r = malloc(sizeof(float) * x * y);
  I.g = malloc(sizeof(float) * x * y);
  I.b = malloc(sizeof(float) * x * y);

   for (int i=0; i<x; i++) {
     for (int j=0; j <y; j++) {
      int idx;
      FreeImage_GetPixelColor(bitmapIn, i, j, &color);

      idx = i + (j*x);

      I.r[idx] = color.rgbRed;
      I.g[idx] = color.rgbGreen;
      I.b[idx] = color.rgbBlue;
    }
   }
  return I;

}

void liberar_imagem(imagem *I) {
  free(I->r);
  free(I->g);
  free(I->b);
}

void salvar_imagem(char *nome_do_arquivo, imagem *I) {
  FIBITMAP *bitmapOut;
  RGBQUAD color;

  printf("Salvando imagem %d por %d...\n", I->width, I->height);
  bitmapOut = FreeImage_Allocate(I->width, I->height, 24, 0, 0, 0);
  printf("Salvo!\n");
   for (int i=0; i<I->width; i++) {
     for (int j=0; j<I->height; j++) {
      int idx;

      idx = i + (j*I->width);
      color.rgbRed = I->r[idx]; 
      color.rgbGreen = I->g[idx];
      color.rgbBlue = I->b[idx];

      FreeImage_SetPixelColor(bitmapOut, i, j, &color);
    }
  }

  FreeImage_Save(FIF_JPEG, bitmapOut, nome_do_arquivo, JPEG_DEFAULT);
}

void multiplicaBrilho(float brilho,imagem*I){
    for (int i=0; i<I->width; i++) {
     for (int j=0; j <I->height; j++) {
      int idx;
    

      idx = i + (j*I->width);

      I->r[idx] = I->r[idx] * brilho;
      I->g[idx] = I->g[idx] * brilho;
      I->b[idx] = I->b[idx] * brilho;
      
      if(I->r[idx]>255){ /*Nao pode ser maior que 255 o valor de um pixel*/
        I->r[idx]=255;  
      }
      if(I->g[idx]>255){
        I->g[idx]=255;  
      }
      if(I->b[idx]>255){
        I->b[idx]=255;  
      }
    }
   }
}
void divideBrilho(float brilho,imagem*I){
    multiplicaBrilho((1/brilho),I);
}

void maiorPixel(imagem *I){
    float max=0;
    for(int i=0;i<I->width;i++){
        for(int j=0;j<I->height;j++){
            if(I->r[i + (j* I->width)]+I->g[i + (j* I->width)]+I->b[i + (j* I->width)]){
                max = I->r[i + (j* I->width)]+I->g[i + (j* I->width)]+I->b[i + (j * I->width)];
            }
        }
    }
}