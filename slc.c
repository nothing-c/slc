#include "slcmath.h"
#include "slclogic.h"
#include "slcflow.h"
#include "slcio.h"
#include <strings.h>

/*
  Simple Lisp Compiler v0.9
*/

/* Offset from stack pointer for variables */
int OFFSET;

void operator (void);
void slcif (void);
void set (void);

/* Set variable */
void set (void) {
  /*
    4 * 8 bits (32 bits) for the offset
    (Stolen from GCC output)
  */
  OFFSET += 4;
  char varname;
  char * varval;
  /* Set a variable (only holds numbers for now) */
  /* Form is (set char value) */
  match('s');
  match(' ');
  varname = getname();
  match(' ');
  varval = getnum();
  /* Add to variable list */
  /* Make sure we don't go over bounds */
  if (VARINDEX >= VARLISTSIZE) {
    puts("\a Error: too many variables");
    exit(1);
  }
  VARIABLES[VARINDEX].name = varname;
  VARIABLES[VARINDEX].offset = OFFSET;
  VARINDEX++;
  /*
    This malloc needs to be variable, but I've given it a big set
    just to make sure it's aight
  */
  char * output = malloc(32);
  /* To deal with assigning expressions */
  if (strlen(varval) == 1) {
    sprintf(output, "mov %%rax, -0x%x(%%rbp)", OFFSET);
  } else {
    sprintf(output, "mov %s, -0x%x(%%rbp)", varval, OFFSET);
  }
  emitln(output);
  free(output);
  free(varval);
}

void operator (void) {
  switch (LOOK) {
  case '+':
    add();
    break;
  case '-':
    subtract();
    break;
  case '*':
    multiply();
    break;
  case '/':
    divide();
    break;
  case 'i':
    slcif();
    break;
  case 'l':
    loop();
    break;
  case 'b':
    slcbreak();
    break;
  case 'g':
    slcgetc();
    break;
  case 'p':
    slcputc();
    break;
  case 's':
    set();
    break;
  default:
    error("Operator");
  }
}

/* Start expression parse */
void expression (void) {
  operator();
}

int main (void) {
  /* Pull first char */
  init();
  while (LOOK != '\n') {
    match('(');
    expression();
    /* This'll throw a fit if it's the last one and you have multiple expressions */
    match(')');
    match(' ');
  }
  return 0;
}
