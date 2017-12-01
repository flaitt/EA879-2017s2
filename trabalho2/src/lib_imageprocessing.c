
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

#include <sys/wait.h>
#include <unistd.h>

#include "imageprocessing.h"

#include <FreeImage.h>
#define numThreads 1
#define numProcesso 1

clock_t Ticks[2];

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

void multiplicaBrilhoprocesso(float brilho,imagem*I){
    int i,j;
    pid_t processo[numProcesso];
    double Tempo;
    Ticks[0] = clock();
    
    
    void prodprocesso(int args){        
        for (i=(args); i<I->width; i++) {
            for (j=0; j <I->height; j++) {
                int idx;
                

                idx = i + (j*I->width);

                I->r[idx] = I->r[idx] * brilho;
                I->g[idx] = I->g[idx] * brilho;
                I->b[idx] = I->b[idx] * brilho;
                
                if(I->r[idx]>255){ /*Nao pode ser maior que 255 o valor de um pixel*/
                    I->r[idx]=255; 
                    /*printf("estouro red\n");*/
                }
                if(I->g[idx]>255){
                    I->g[idx]=255;
                    /*printf("estouro green\n");*/
                }
                if(I->b[idx]>255){
                    I->b[idx]=255;  
                    /*printf("estouro blue\n");*/
                }
            }
        }
        
        for(i=0;i<numProcesso;i++){
            processo[i] = fork();
            if(processo[i]==0){
                    prodprocesso(processo[i]);
                    wait(NULL);
            }
        }
    }
   Ticks[1] = clock();
   Tempo = (Ticks[1] - Ticks[0]) * 1000.0 / CLOCKS_PER_SEC;
   printf("Tempo gasto com processo: %g ms.\n", Tempo);
}


void multiplicaBrilhothread(float brilho,imagem*I){
    double Tempo;
    int i,vetor[numThreads],j;
    pthread_t tarefa[numThreads];
    
    Ticks[0] = clock();
    
    
    void* mul(void* args){
        
        int* aux=(int*)args;
        for (i=(*aux); i<I->width; i++) {
            for (j=0; j <I->height; j++) {
                int idx;
                

                idx = i + (j*I->width);

                I->r[idx] = I->r[idx] * brilho;
                I->g[idx] = I->g[idx] * brilho;
                I->b[idx] = I->b[idx] * brilho;
                
                if(I->r[idx]>255){ /*Nao pode ser maior que 255 o valor de um pixel*/
                    I->r[idx]=255; 
                    /*printf("estouro red\n");*/
                }
                if(I->g[idx]>255){
                    I->g[idx]=255;
                    /*printf("estouro green\n");*/
                }
                if(I->b[idx]>255){
                    I->b[idx]=255;  
                    /*printf("estouro blue\n");*/
                }
            }
        }
    }
    for(i=0;i<numThreads;i++){
        vetor[numThreads]=i;
    }
    
    for(i=0 ; i < numThreads ; i++){
        pthread_create(&tarefa[i],NULL,mul,&vetor[i]);
    }
    
    for(i=0 ; i< numThreads ; i++){
        pthread_join(tarefa[i],NULL);
    }
   
   Ticks[1] = clock();
   Tempo = (Ticks[1] - Ticks[0]) * 1000.0 / CLOCKS_PER_SEC;
   printf("Tempo gasto com thread: %g ms.\n", Tempo);
   multiplicaBrilhoprocesso(brilho/1.5,I);
    
}


void multiplicaBrilhocoluna(float brilho,imagem*I){
    /*if(first==0){
        imagem k = abrir_imagem("demo.jpg");
        salvar_imagem("teste.jpg", &k);
        first=1;
    }
    */
    
    double Tempo;
    imagem *AUX;
    imagem AUX2;
    
    AUX = &AUX2;
    
    AUX->width = I->width;
    AUX->height = I->height;
    
    Ticks[0] = clock();
    for (int i=0; i<I->width; i++) {
     for (int j=0; j <I->height; j++) {
      int idx;
    

      idx = i + (j*I->width);

      
      
      I->r[idx] = I->r[idx] * brilho;
      I->g[idx] = I->g[idx] * brilho;
      I->b[idx] = I->b[idx] * brilho;

      
      if(I->r[idx]>255){ /*Nao pode ser maior que 255 o valor de um pixel*/
        I->r[idx]=255; 
        /*printf("estouro red\n");*/
      }
      if(I->g[idx]>255){
        I->g[idx]=255;
        /*printf("estouro green\n");*/
      }
      if(I->b[idx]>255){
        I->b[idx]=255;  
        /*printf("estouro blue\n");*/
      }
    }
   }
   Ticks[1] = clock();
   Tempo = (Ticks[1] - Ticks[0]) * 1000.0 / CLOCKS_PER_SEC;
   printf("Tempo gasto sem thread por colunas: %g ms.\n", Tempo);
   multiplicaBrilhothread(brilho/1.5,I);
}




void multiplicaBrilho(float brilho,imagem*I){
    /*if(first==0){
        imagem k = abrir_imagem("demo.jpg");
        salvar_imagem("teste.jpg", &k);
        first=1;
    }
    */
    
    double Tempo;
    imagem *AUX;
    imagem AUX2;
    
    AUX = &AUX2;
    
    AUX->width = I->width;
    AUX->height = I->height;
    
    Ticks[0] = clock();
    for (int i=0; i<I->width; i++) {
     for (int j=0; j <I->height; j++) {
      int idx;
    

      idx = i + (j*I->width);

      
      AUX->r[idx] = I->r[idx];
      AUX->g[idx] = I->g[idx];
      AUX->b[idx] = I->b[idx];
      
      I->r[idx] = I->r[idx] * brilho;
      I->g[idx] = I->g[idx] * brilho;
      I->b[idx] = I->b[idx] * brilho;

      
      if(I->r[idx]>255){ /*Nao pode ser maior que 255 o valor de um pixel*/
        I->r[idx]=255; 
        /*printf("estouro red\n");*/
      }
      if(I->g[idx]>255){
        I->g[idx]=255;
        /*printf("estouro green\n");*/
      }
      if(I->b[idx]>255){
        I->b[idx]=255;  
        /*printf("estouro blue\n");*/
      }
    }
   }
   Ticks[1] = clock();
   Tempo = (Ticks[1] - Ticks[0]) * 1000.0 / CLOCKS_PER_SEC;
   printf("Tempo gasto sem thread por linhas: %g ms.\n", Tempo);
   multiplicaBrilhocoluna(brilho,&AUX2);
}



void divideBrilho(float brilho,imagem*I){
    multiplicaBrilho((1/brilho),I);
}

void maiorPixel(imagem *I){
    float max=0,rmax=0,gmax=0,bmax=0;
    for(int i=0;i < I->width ; i++){
        for(int j=0 ; j<I->height ; j++){
            if(I->r[(i + (j* I->width))] + I->g[(i + (j* I->width))] + I->b[(i + (j* I->width))] > max){
                max = I->r[(i + (j* I->width))]+ I->g[(i + (j* I->width))]+ I->b[(i + (j* I->width))];
                rmax=I->r[(i + (j* I->width))];
                gmax=I->g[(i + (j* I->width))];
                bmax=I->b[(i + (j* I->width))];
            }
        }
    }
    printf("O valor do maior pixel eh: %.2f\nrmax=%f \n gmax=%f\n bmax=%f \n",max,rmax,gmax,bmax);
}