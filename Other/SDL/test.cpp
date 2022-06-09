/***************************************
 Testes com SDL - INPUT               
 por Maycon Amaral
***************************************/
//http://irrlicht.sourceforge.net/phpBB2/viewtopic.php?p=63053&highlight=sdl#63053
#include <windows.h>

#include <SDL/SDL_getenv.h>
#include <SDL/SDL.h>
#include <SDL/SDL_syswm.h>

#include <irrlicht.h>

#pragma comment(lib,"SDL.lib")
#pragma comment(lib,"SDLmain.lib")

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
   // Iniciando SDL
   if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_NOPARACHUTE) < 0)
   {
      MessageBox((HWND)(DWORD)GetActiveWindow(),"Ocorreu erro ao iniciar SDL",SDL_GetError(),MB_OK | MB_ICONERROR | MB_TASKMODAL);
   }

   // Cria janela da SDL
   SDL_Surface *sdl_window = SDL_SetVideoMode(640,480,32,SDL_HWSURFACE|SDL_RESIZABLE);
   
   HWND sdl_hWnd;
   
   static SDL_SysWMinfo pInfo;
   SDL_VERSION(&pInfo.version);
   SDL_GetWMInfo(&pInfo);
   sdl_hWnd = pInfo.window;

   RECT r;
   //GetWindowRect(pInfo.window, &r);
   //SetWindowPos(pInfo.window, 0, r.left, r.top, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

   //SDL_ShowCursor(SDL_DISABLE);
   SDL_WM_GrabInput(SDL_GRAB_ON);
   
   SIrrlichtCreationParameters   params;
   params.AntiAlias=false;
   params.Bits=32;
   params.DriverType=EDT_OPENGL;
   params.Fullscreen=false;
   params.Stencilbuffer=false;
   params.Vsync=false;
   params.WindowSize=dimension2d<s32>(640,480);
   params.WindowId = (int)sdl_hWnd;
   
   IrrlichtDevice *device = createDeviceEx(params);
   IVideoDriver* driver = device->getVideoDriver();
   ISceneManager* smgr = device->getSceneManager();
   IGUIEnvironment* guienv = device->getGUIEnvironment();

   // Ajustando Texto da Janela
   SDL_WM_SetCaption("Irrlicht + SDL - Testando INPUT",NULL);

   // Loop Principal ( INPUT )
   SDL_Event evento;
   SDL_EnableKeyRepeat(0,0);
   
   SDL_EnableKeyRepeat(1,1);

   // Fazer teste de input
   bool sair_programa = false;
   
   // Teste
   ISceneNode* node = smgr->addTestSceneNode();
   
   node->setScale(vector3df(1,1,1));
   
   node->setMaterialFlag(EMF_LIGHTING,false);
   node->setMaterialFlag(EMF_WIREFRAME,true);
   
   node->setPosition(vector3df(0,0,100));
   
   ICameraSceneNode* camera = smgr->addCameraSceneNode(0,vector3df(0,0,0),vector3df(0,0,100));
   
   vector3df rotation;
   vector3df position = node->getPosition();
   
   Uint8 *tecla = SDL_GetKeyState(NULL);

   while(sair_programa==false)
   {
      while (SDL_PollEvent(&evento))
      {
         // Inicia Eventos do INPUT
         switch (evento.type)
         {
            // Quanado Apertar alguma tecla do Teclado
            case SDL_KEYDOWN : switch(evento.key.keysym.sym){
             
             case SDLK_UP : printf("Tecla - Seta para Cima - Foi pressionada \n"); rotation.X++; break;
             case SDLK_DOWN : printf("Tecla - Seta para Baixo - Foi pressionada \n"); rotation.X--; break;
             case SDLK_LEFT : printf("Tecla - Seta para Esquerda - Foi pressionada \n"); rotation.Y++; break;
             case SDLK_RIGHT: printf("Tecla - Seta para Direita - Foi pressionada \n"); rotation.Y--; break;
             case SDLK_ESCAPE : sair_programa = true; break;

         } break;
         
         // Quando Soltar alguma tecla do Teclado
         case SDL_KEYUP : switch(evento.key.keysym.sym){
           
           case SDLK_UP : printf("Tecla - Seta para Cima - Foi solta \n"); break;
           case SDLK_DOWN : printf("Tecla - Seta para Baixo - Foi solta \n"); break;
           case SDLK_LEFT : printf("Tecla - Seta para Esquerda - Foi solta \n"); break;
           case SDLK_RIGHT : printf("Tecla - Seta para Direita - Foi solta \n"); break;
           
      } break;
     
      // Quando Pressionar algum botão do Mouse
      case SDL_MOUSEBUTTONDOWN : switch(((SDL_MouseButtonEvent*)&evento)->button){
           
           case SDL_BUTTON_LEFT : printf("Mouse - Botao Esquerdo foi pressionado \n"); break;
           case SDL_BUTTON_MIDDLE : printf("Mouse - Botao do Meio foi pressionado \n"); break;
           case SDL_BUTTON_RIGHT : printf("Mouse - Botao Direito foi pressionado \n"); break;
           case SDL_BUTTON_WHEELUP : printf("Mouse - Scroll Para Cima foi Somado \n"); break;
           case SDL_BUTTON_WHEELDOWN : printf("Mouse - Scroll Para Baixo foi Somado \n"); break;
           
      } break;
     
      // Quando Soltar algum botão do Mouse
      case SDL_MOUSEBUTTONUP : switch(((SDL_MouseButtonEvent*)&evento)->button){
           
           case SDL_BUTTON_LEFT : printf("Mouse - Botao Esquerdo foi solto \n"); break;
           case SDL_BUTTON_MIDDLE : printf("Mouse - Botao do Meio foi solto \n"); break;
           case SDL_BUTTON_RIGHT : printf("Mouse - Botao Direito foi solto \n"); break;
           case SDL_BUTTON_WHEELUP : printf("Mouse - Scroll Para Cima foi Parado \n"); position.Z++; break;
           case SDL_BUTTON_WHEELDOWN : printf("Mouse - Scroll Para Baixo foi Parado \n"); position.Z--; break;
           
      } break;
                       
      // Quando Movimentar o Mouse
      case SDL_MOUSEMOTION : printf("Mouse - Posicao: [%d, %d] \n", evento.motion.x, evento.motion.y); break;
                 
         // Quando Encerrar SDL
         case SDL_QUIT : sair_programa = true; break;
         
         default: break;
         }
         
      // Pega o Estado das Teclas do Teclado
      tecla = SDL_GetKeyState(NULL); 
     
      // Enquanto a Tecla Estiver Pressionada
      if (tecla[SDLK_RETURN]) printf("Tecla - Enter - Foi pressionada no momento \n");
           
      node->setRotation(rotation);
      node->setPosition(position);
     
      driver->beginScene(true, true, SColor(255,0,0,0));
      smgr->drawAll();
      guienv->drawAll();
      driver->endScene();                                                             
      }
   }

   // Libera memária alocada para a Janela
   SDL_FreeSurface(sdl_window);

   // Destruindo SDL
   SDL_Quit();
   device->drop();

   return 0;
}
