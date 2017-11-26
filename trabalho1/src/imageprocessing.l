
%{
#include <stdlib.h>
#include <string.h>
void yyerror(char *);

#include "y.tab.h"
%}



%%

[a-z]  {
        yylval.ival = yytext[0] - 'a';
        return VAR;
       }


\" { return ASPA; }

[0-9]+(.[0-9]+)* {
        yylval.fval = atof(yytext);
       // printf("achei nome num:%s\n", yylval.strval);
        return NUM;    
}

[a-zA-Z0-9\.]+  {
        for (int i = 0; i < yyleng; i++ ){
          yylval.strval[i] = yytext[i];
        }
        yylval.strval[yyleng] = '\0';
        //printf ("Achei nome string: %s\n", yylval.strval);
        return STRING;
       }

[=]     {
          return IGUAL;
        }
    

        
[*] {
    //printf("achei asterisco \n");
    return AST;
}        
    
[/] {
    return BARRA;
}        
        
        
(\[[a-zA-Z0-9\.]+\])  {
        for (int i = 1; i +1 < yyleng; i++ ){
          yylval.strval[i] = yytext[i];
        }
        yylval.strval[yyleng] = '\0';
        printf ("Achei nome aqui: %s\n", yylval.strval);
        return BRILHO;
       }
        
        
        
        
\n   {
         return EOL;
       }

[ \t]+ ;

.     {printf("**%s**\n", yytext);
yyerror("Token nao reconhecido!\n");}

%%

int  yywrap() {
  return 1;
}
