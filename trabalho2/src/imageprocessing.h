
#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H


typedef struct {
  unsigned int width, height;
  float *r, *g, *b;
} imagem;

imagem abrir_imagem(char *nome_do_arquivo);
void salvar_imagem(char *nome_do_arquivo, imagem *I);
void liberar_imagem(imagem *i);
void multiplicaBrilho(float brilho,imagem*I);
void divideBrilho(float brilho,imagem*I);
void maiorPixel(imagem *I);

#endif
