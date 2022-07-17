/*
  Simple Lisp Compiler Logical Ops
*/

/* Yes, I know that this is a lot of boilerplate and could be streamlined */

void lessthan (void) {
  char * num;
  char * num1;
  match('<');
  match(' ');
  num = getnum();
  initreg(num);
  emitln("push %rax");
  match(' ');
  num1 = getnum();
  initreg(num);
  /* This *should* take care of nested functions, but those are already borked */
  emitln("pop %rdx");
  /*
    This works this way because the opcodes (for AT&T, which I'm using)
    do the opcode as "cmp src2, src1" which translates to src2 [operator] src1
    The reasoning for this is a mystery, but w/e
  */
  emitln("cmp %rax, %rdx");
  /*
    Because of the way this is structure, I can just set an unconditional
    jmp to the false label and at the end of both labels set an unconditional
    jump to the rest of the program
  */
  emitln("jl true\n\tjmp false\ntrue:");
  match(' ');
  match('(');
  expression();
  match(')');
  emitln("jmp resume");
  emitln("\nfalse:");
  match(' ');
  match('(');
  expression();
  emitln("\nresume:");
  free(num);
  free(num1);
}

void greaterthan (void) {
  /*
    This bit is just boilerplate, but I'm too lazy to put it in
    a function by itself
  */
  char * num;
  char * num1;
  match('>');
  match(' ');
  num = getnum();
  initreg(num);
  emitln("push %rax");
  match(' ');
  num1 = getnum();
  initreg(num1);
  emitln("pop %rdx");
  emitln("cmp %rax, %rdx");
  emitln("jg true\njmp false\ntrue:");
  match(' ');
  match('(');
  expression();
  match(')');
  emitln("jmp resume");
  emitln("\nfalse:");
  match(' ');
  match('(');
  expression();
  emitln("\nresume:");
  free(num);
  free(num1);
}

void equalto (void) {
  char * num;
  char * num1;
  match('=');
  match(' ');
  num = getnum();
  initreg(num);
  emitln("push %rax");
  match(' ');
  num1 = getnum();
  initreg(num1);
  emitln("pop %rdx");
  emitln("cmp %rax, %rdx");
  emitln("je true\njmp false\ntrue:");
  match(' ');
  match('(');
  expression();
  match(')');
  emitln("jmp resume");
  emitln("\nfalse:");
  match(' ');
  match('(');
  expression();
  emitln("\nresume:");
  free(num);
  free(num1);
}

void nequalto (void) {
  char * num;
  char * num1;
  match('!');
  match(' ');
  num = getnum();
  initreg(num);
  emitln("push %rax");
  match(' ');
  num1 = getnum();
  initreg(num1);
  emitln("pop %rdx");
  emitln("cmp %rax, %rdx");
  emitln("jne true\njmp false\ntrue:");
  match(' ');
  match('(');
  expression();
  match(')');
  emitln("jmp resume");
  emitln("\nfalse:");
  match(' ');
  match('(');
  expression();
  emitln("\nresume:");
  free(num);
  free(num1);
}

void slcif (void) {
  /* The form is (i [><=!] num num (expression) (expression to do if false)) */
  match('i');
  /* Gobble whitespace */
  match(' ');
  switch (LOOK) {
  case '<':
    lessthan();
    break;
  case '>':
    greaterthan();
    break;
  case '=':
    equalto();
    break;
  case '!':
    nequalto();
    break;
  default:
    error("Comparison operator");
  }
}
