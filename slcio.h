void slcputc (void) {
  char * num;
  /* Format is (putc num) */
  match('p');
  match(' ');
  /* putchar() uses %ecx for its storage location! */
  num = getnum();
  initreg(num);
  emitln("mov %rax, %ecx");
  emitln("call putchar");
  free(num);
}

void slcgetc (void) {
  match('g');
  /*
    Annoyingly, I couldn't get a good list of Windows syscalls, so
    I'm having to call C functions for I/O
    Pfui
  */
  emitln("call getchar");
  /*
    Getchar sends its stuff to %eax, so we use that
  */
  emitln("mov %eax, %rax");
}
