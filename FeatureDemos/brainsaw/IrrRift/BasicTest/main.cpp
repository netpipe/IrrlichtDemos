  #include <stdio.h>

  #include <Win32Project1.h>

int main() {
  printf("IrrRift_install: %i\n", (int)IrrRift_install());

  while (true) {
    float x, y, z;
    IrrRift_poll(x, y, z);
    printf("%.2f, %.2f, %.2f\n", x, y, z);
  }
  return 0;
}
