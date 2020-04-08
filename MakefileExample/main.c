#include <stdio.h>

main ()
{ int cmd;

  do
  { printf ("command> ");
    scanf ("%d", &cmd);
    if (cmd == 1) solve_function ();
    else if (cmd == 2) find_numbers ();
    else if (cmd !=0) printf ("Invalid command\n");
  }
  while (cmd !=0);
}
