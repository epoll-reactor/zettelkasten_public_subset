%{
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define MAX_DEPTH 72

int nesting = 0 ;
unsigned int indent_stack[MAX_DEPTH] ;
unsigned int level = 0 ;

unsigned int first = 1 ;

void process_indent(char* line) ;

%}

PUNCT [-+*/=<>:]*
ID  [A-Za-z_][A-Za-z0-9_]*
INT [0-9]+

%option noyywrap

%%


^[ ]*\n       {/* Ignore blank lines. */}
^[ ]*[^ \n]+  {int last = yyleng - 1;
               process_indent(yytext) ;
               while ((last >= 0) &&
                      (yytext[last] != ' ')) {
                unput(yytext[last]);
                last-- ;}}


"("  {printf(" "); ECHO; nesting++ ; }
")"  {printf(" "); ECHO; nesting-- ; }

{ID}|{INT}|{PUNCT} {printf(" ") ; ECHO;}

[ \r] {}
\n    {}

<<EOF>>  { process_indent("") ; return 0 ; }

%%

unsigned int white_count(char* line) {
  unsigned int count = 0 ;
  while (*line == ' ')
    count++, line++ ;
  return count ;
}

void process_indent(char* line) {
  if (nesting)
    /* Ignore indents while nested. */
    return ;

  unsigned int indent = white_count(line) ;

  if (indent == indent_stack[level]) {
    if (!first) printf(" ;") ;
    first = 0 ;
    return ;
  }

  if (indent > indent_stack[level]) {
    printf(" {") ;
    assert(level+1 < MAX_DEPTH) ;
    indent_stack[++level] = indent ;
    return ;
  }

  while (indent < indent_stack[level]) {
    --level ;
    printf(" }") ;
  }

  assert(level >= 0) ;
}

int main(int argc, char* argv[]) {
  indent_stack[0] = 0 ;
  yylex() ;
  printf("\n") ;
}
