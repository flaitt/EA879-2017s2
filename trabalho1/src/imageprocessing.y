%{
#include <stdio.h>
#include "imageprocessing.h"
#include <FreeImage.h>

void yyerror(char *c);
int yylex(void);

%}
%union {
  char    strval[50];
  int     ival;
  float   fval;
}

%token <strval> STRING BRILHO
%token <ival> VAR IGUAL EOL ASPA AST BARRA
%token <fval> NUM
%left SOMA 

%%

PROGRAMA:
        PROGRAMA EXPRESSAO EOL
        |
        ;

EXPRESSAO:
    | STRING IGUAL STRING {
        printf("Copiando %s para %s\n", $3, $1);
        imagem I = abrir_imagem($3);
        printf("Li imagem %d por %d\n", I.width, I.height);
        salvar_imagem($1, &I);
        liberar_imagem(&I);
                          }

    | STRING IGUAL STRING AST NUM{
       /* printf("antes abrir \n");*/
        imagem I = abrir_imagem($3);
       /* printf("antes de multiplicar\n");*/
        multiplicaBrilho($5,&I);
       /* printf("ta multiplicando \n");*/
        salvar_imagem($1,&I);
    }
    
    | STRING IGUAL STRING BARRA NUM{
        imagem I = abrir_imagem($3);
        divideBrilho($5,&I);
        salvar_imagem($1,&I);
    }
    
    | BRILHO{
        printf("antes imagem\n");
        imagem I = abrir_imagem($1);
        printf("antes imagem\n");
        maiorPixel(&I);
    }
    
    
                          
    ;

%%

void yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
}

int main() {
  FreeImage_Initialise(0);
  yyparse();
  return 0;

}
