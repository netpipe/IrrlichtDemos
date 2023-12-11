#ifndef _CLEVELS
  #define _CLEVELS

  #include <irrlicht.h>
//  #include <irrXml.h>

typedef struct _LVL_TOP10
{
  char  sName[20];
  irr::u32 iTime;
}
CLevelTop10;

typedef struct _LVL_INFO
{
  char sName[100];
  char sFile[100];

  CLevelTop10 cHiScore[10];
}
CLevelInfo;

class CLevels
{
  private:
    char *breakString(char *sInput);
    void loadHiScore(char *sFileName, CLevelInfo *pInfo);
    void saveHiScore(char *sFileName, CLevelInfo *pInfo);

  public:
    CLevelInfo *m_pLevels;
    int         m_iLevels;

    int getHiScore(int lvl, int pos);
    int getHiScorePos(int lvl, int iTime);
    void getHiScoreName(int lvl, int pos, wchar_t *sBuffer);
    void addHiScore(int lvl, int iTime, const wchar_t *sName);

     CLevels();
    ~CLevels();
};

#endif
