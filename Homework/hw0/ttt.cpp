#include <iostream>
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

using namespace std;
 
int main()
{
  //char str[]="00:22:33:4B:55:5A";
  string str;
  char *delim = ":";
  char * pch;
  cin >> str;
  printf ("Splitting string \"%s\" into tokens:\n",str.c_str());
  pch = strtok((char *)str.c_str(),delim);
  while (pch != NULL)
  {
    printf ("%s\n",pch);
    pch = strtok (NULL, delim);
  }      
  return 0;
}
