
int term_demo(int argc, char **argv){
  term_reset();
//  term_clear();
  term_bold();
  term_underline();
  printf("bold and underlined\n");

  printf("reset\n");

  term_bright();
  term_color("green");
  printf("ok\n");

//  term_clear();
  term_reset();
  term_bold();
  term_blink();
  term_underline();
  term_color("red");
  term_background("black");
  printf("fail\n");
  term_reverse();
  printf("fail\n");
  term_reset();
//  term_clear();

//  term_move_to(50, 10);
//  printf("hey");
//  term_move_by(1, 1);
 // printf("there");
//  term_move_to(0, 15);

//  int w = 0, h = 0;
//  term_size(&w, &h);
//  printf("%dx%d\n", w, h);

  return 0;
}
