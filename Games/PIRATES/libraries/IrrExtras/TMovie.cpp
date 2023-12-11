
    #include <dshow.h>
    #include <mmstream.h>
    #include <amstream.h>
    #include <ddstream.h>


    static GUID MY_CLSID_AMMultiMediaStream={0x49C47CE5,0x9BA4,0x11D0,0x82,0x12,0x00,0xC0,0x4F,0xC3,0x2C,0x45};
    static GUID MY_IID_IAMMultiMediaStream={0xBEBE595C,0x9A6F,0x11D0,0x8F,0xDE,0x00,0xC0,0x4F,0xD9,0x18,0x9D};
    static GUID MY_MSPID_PrimaryVideo={0xA35FF56A,0x9FDA,0x11D0,0x8F,0xDF,0x00,0xC0,0x4F,0xD9,0x18,0x9D};
    static GUID MY_IID_IDirectDrawMediaStream={0xF4104FCE,0x9A70,0x11D0,0x8F,0xDE,0x00,0xC0,0x4F,0xD9,0x18,0x9D};
    static GUID MY_MSPID_PrimaryAudio={0xA35FF56B,0x9FDA,0x11D0,0x8F,0xDF,0x00,0xC0,0x4F,0xD9,0x18,0x9D};

    class TMovie
     {
             IAMMultiMediaStream*     pAMStream;
             IMediaStream*            pPrimaryVidStream;
             IDirectDrawMediaStream*  pDDStream;
             IDirectDrawStreamSample* pSample;
             IDirectDrawSurface*      pSurface;
             RECT                     Movie_rect;
             LONG                     MoviePitch;
             void*                    MovieBuffer;
             DWORD                    time;
             DWORD                    oldtick;
             BOOL                     flg;

         public:
               TMovie()
                {
                   CoInitialize(0);
                   pAMStream         = 0;
                   pPrimaryVidStream = 0;
                   pDDStream         = 0;
                   pSample           = 0;
                   pSurface          = 0;
                   time              = 0;
                }

               ~TMovie()
                {
                    pPrimaryVidStream->Release();
                    pDDStream->Release();
                    pSample->Release();
                    pSurface->Release();
                    pAMStream->Release();
                    CoUninitialize();
                }

               void LoadMovie(char* filename)
                {
                    WCHAR buf[512];
                    MultiByteToWideChar(CP_ACP,0,filename,-1,buf,512);
                    CoCreateInstance(MY_CLSID_AMMultiMediaStream,0,1,MY_IID_IAMMultiMediaStream,(void**)&pAMStream);
                    pAMStream->Initialize((STREAM_TYPE) 0, 0, NULL);
                    pAMStream->AddMediaStream( 0, &MY_MSPID_PrimaryVideo, 0, NULL);
                    pAMStream->OpenFile(buf,4);
                    pAMStream->GetMediaStream( MY_MSPID_PrimaryVideo, &pPrimaryVidStream);
                    pPrimaryVidStream->QueryInterface(MY_IID_IDirectDrawMediaStream,(void**)&pDDStream);
                    pDDStream->CreateSample(0,0,0,&pSample);
                    pSample->GetSurface(&pSurface,&Movie_rect);
                    pAMStream->SetState((STREAM_STATE)1);
                }

               void NextMovieFrame()
                {
                   if(GetTickCount()-oldtick < time){flg = false ;return;}
                   oldtick = GetTickCount();
                   flg = true;
                   pSample->Update( 0, NULL, NULL, 0);
                }

               int MovieWidth() { return (Movie_rect.right - Movie_rect.left);}

               int MovieHeight() { return (Movie_rect.bottom - Movie_rect.top);}

               void DrawMovie(int x,int y,ITexture* Buf)
                {
                    void* pBits = Buf->lock();
                    LONG  Pitch = Buf->getPitch();
                    DDSURFACEDESC  ddsd;
                    ddsd.dwSize=sizeof(DDSURFACEDESC);
                    pSurface->Lock( NULL,&ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT , NULL);
                    int wmin=(Pitch<ddsd.lPitch)?Pitch:ddsd.lPitch;
                    for(int h=0; h<ddsd.dwHeight; h++)
                         memcpy((BYTE*)pBits+((y+h)*Pitch)+x*4,(BYTE*)ddsd.lpSurface+h*ddsd.lPitch,wmin);
                    pSurface->Unlock(NULL);
                    Buf->unlock();
               }

              void SetMovieFPS(int fps)
               {
                    time = fps;
               }

              void MovieLock()
               {
                    DDSURFACEDESC  ddsd;
                    ddsd.dwSize=sizeof(DDSURFACEDESC);
                    pSurface->Lock( NULL,&ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL);
                    MoviePitch  = ddsd.lPitch;
                    MovieBuffer = ddsd.lpSurface;
               }

              void MovieUnlock()
               {
                    pSurface->Unlock( NULL);
               }

              void Gray(int x1,int y1,int x2,int y2)
               {
                   if(!flg) return;
                   DWORD  clr;
                   LPBYTE buff;
                   for (int h=y1; h<y2; h++)
                       for (int w=x1; w<x2; w++)
                        {
                           buff = (LPBYTE)MovieBuffer + w*4 + h*MoviePitch;
                           clr = (buff[0] + buff[1] + buff[2]) /3;
                           *(DWORD*)buff = RGB((BYTE)clr,(BYTE)clr,(BYTE)clr);
                        }
               }

              void Negative(int x1,int y1,int x2,int y2)
               {
                   if(!flg) return;
                   DWORD  clr;
                   LPBYTE buff;
                   for (int h=y1; h<y2; h++)
                       for (int w=x1; w<x2; w++)
                        {
                           buff = (LPBYTE)MovieBuffer + w*4 + h*MoviePitch;
                           *(DWORD*)buff = RGB((BYTE)255-buff[0],(BYTE)255-buff[1],(BYTE)255-buff[2]);
                        }
               }

              void Intensity(float intnsty,int x1,int y1,int x2,int y2)
               {
                   if(!flg) return;
                   DWORD  clr;
                   LPBYTE buff;
                   for (int h=y1; h<y2; h++)
                       for (int w=x1; w<x2; w++)
                        {
                           buff = (LPBYTE)MovieBuffer + w*4 + h*MoviePitch;
                           *(DWORD*)buff = RGB((BYTE)buff[0]/intnsty,(BYTE)buff[1]/intnsty,(BYTE)buff[2]/intnsty);
                        }
               }
     };



/*
while(irrDevice->run())
  {
     irrVideo->beginScene(true, true, SColor(0,200,200,200));
                  movie->NextMovieFrame() ;
                  movie->MovieLock();
                  movie->Gray(0,0,320,240) ;
                 // movie->Negative(0,0,320,240);
                 // movie->Intensity(2.5,0,0,320,100);
                  movie->MovieUnlock();
                  movie->DrawMovie(0,0,movTxtr);
                  irrSceneMgr->drawAll()
      irrVideo->endScene()
  }
*/
