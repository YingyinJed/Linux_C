#include "test.h"

int test_(int x ,int y)
{
   x = (x + y) * 2;
   y = (x + y) * 3;
   printf("%d---test----%d\n",x,y);
   return x;
}