

void tcolor_demo(){

    fputs("Hello, colorful world\n", text_red(stdout));
    reset_colors(stdout);

    fputs("Hello, colorful world\n", text_red(stderr));
    reset_colors(stderr);

  fprintf(stderr, 
    "<%d> %s"
    "\n",
    getpid(),
    "TCOLOR"
  );
}
