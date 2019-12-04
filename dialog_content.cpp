#include "dialog.h"

/*
the actual contents of the dialog windows are going to go here
*/

int dialog_height = 6;
int dialog_width = 40;

dialog talk_to_rose = dialog(
  dialog_height, dialog_width,
  {"Hello there. My name is Rose. I don't",
  "have much to say right now.",
  "*Leave*",
  "Why are you here?"}, 4,
  {3,4}, 2
);

dialog ask_rose_a = dialog(
  dialog_height, dialog_width,
  {"Well I wanted to adventure.",
  "Too long sitting at home waiting",
  "for adventure to come to me...",
  "<continue>"}, 4,
  {4}, 1
);

dialog ask_rose_b = dialog(
  dialog_height, dialog_width,
  {"But it's awfully scary down there.",
  "Could you bring back the treasure",
  "for me? Please?",
  "<exit>"}, 4,
  {4}, 1
);

dialog rose_success = dialog(
  dialog_height, dialog_width,
  {"Oh wow, you did it!",
  "You're my hero <3",
  "",
  "<exit>"}, 4,
  {4}, 1
);
