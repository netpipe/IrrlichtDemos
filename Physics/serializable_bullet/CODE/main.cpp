#include "GMiniGolfApp.h"

/* Entry point
*/
int main(int argc, char* argv[])
{
  // construct app
  CGameApp* aApp = new GMiniGolfApp();

  // init
  if (!aApp->Create())
  {
    // app creation error
    delete aApp;
    return 1;
  }

  // run
  aApp->Run();
	
  delete aApp;

	return 0;
}


