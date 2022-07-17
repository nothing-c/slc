/*
  Simple Lisp Compiler Control Flow
*/

void slcbreak (void) {
  match('b');
  emitln("jmp break");
}

void loop (void) {
  match('l');
  match(' ');
  /* Format: (l (expression)) */
  emitln("\nloop:");
  match('(');
  expression();
  match(')');
  /* Jump back to the loop label, it's a goto on stilts */
  emitln("jmp loop");
  emitln("\nbreak:");
}
