/*
  Simple Lisp Compiler Control Flow
*/

void slcbreak (void) {
  match('b');
  emitln("jmp @f");
}

void loop (void) {
  match('l');
  match(' ');
  /* Format: (l (expression)) */
  /* 
  Anonymous label, so we don't have to come up 
  with new labels on the fly
  */
  emitln("\n@@:");
  match('(');
  expression();
  match(')');
  /* Jump back to the loop label, it's a goto on stilts */
  emitln("jmp @b");
  emitln("\n@@:");
}
