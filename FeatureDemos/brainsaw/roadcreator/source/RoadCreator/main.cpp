  #include <irrlicht.h>
  #include <CRunner.h>

int main(int argc, char** argv) {
  CRunner theRunner(video::EDT_OPENGL,core::dimension2d<u32>(1024,768),16,false,false,false);
  theRunner.run();
  return 0;
} 
