#include "slclib.h"

/*
  Simple Lisp Compiler Math Library
*/

void subtract (void) {
  char * num;
  char * num1;
  match('-');
  match(' ');
  num = getnum();
  initreg(num);
  emitln("push %rax");
  match(' ');
  num1 = getnum();
  initreg(num1);
  emitln("pop %rdx");
  /*
    Since the input is - x y (equiv. to x - y), x will be on the stack and
    therefore in %rdx, so we do sub %rax, %rdx
    This *will* store the result in %rdx though, so we need to move it back over
  */
  emitln("sub %rax, %rdx");
  emitln("mov %rdx, %rax");
}

void add (void) {
  char * num;
  char * num1;
  match('+');
  /* Skip the space */
  match(' ');
  num = getnum();
  initreg(num);
  /* Put the first number onto stack */
  emitln("push %rax");
  match(' ');
  num1 = getnum();
  /* Get the second number in register */
  initreg(num1);
  emitln("pop %rdx");
  emitln("add %rdx, %rax");
}

void divide (void) {
  char * num;
  char * num1;
  match('/');
  match(' ');
  num = getnum();
  initreg(num);
  /* Get it onto stack */
  emitln("push %rax");
  match(' ');
  num1 = getnum();
  initreg(num1);
  /*
    Have to use %rbx instead of popping into %rdx because of
    how the x64 division op works
  */
  emitln("mov %rax, %rbx");
  emitln("pop %rax");
  /* Unsigned division */
  emitln("divq %rbx");
}

void multiply (void) {
  char * num;
  char * num1;
  match('*');
  match(' ');
  num = getnum();
  initreg(num);
  emitln("push %rax");
  match(' ');
  num1 = getnum();
  initreg(num1);
  emitln("pop %rdx");
  emitln("imul %rdx, %rax");
}
