  #include "CLevels.h"

CLevels::CLevels()
{
FILE *f;
int   iLevelCnt=0;
char  s1[50],*s2;
int   i;

  if ((f=fopen("levels/levels.dat","rt"))==NULL) { printf("unable to load level info.\n"); return; }

  do
  {
    fscanf(f,"%s",s1);
    s2=strchr(s1,'=');

    if (s2)
    {
      s2=breakString(s1);

      if (!strcmp(s1,"count"))
      {
        m_iLevels=atoi(s2);
        m_pLevels=(CLevelInfo *)malloc(m_iLevels*sizeof(CLevelInfo));
      }
      else
      {
        strcpy(m_pLevels[iLevelCnt].sName,s1);
        strcpy(m_pLevels[iLevelCnt].sFile,s2);

        if (strrchr(s2,'.')) *(strrchr(s2,'.'))='\0';

        sprintf(s1,"%s.hiscore",s2);

        for (i=0; i<10; i++) m_pLevels[iLevelCnt].cHiScore[i].iTime=0;

        loadHiScore(s1,&m_pLevels[iLevelCnt]);

        iLevelCnt++;
      }
    }
  }
  while (!feof(f));

  fclose(f);
}

CLevels::~CLevels()
{
  free(m_pLevels);
}

void CLevels::loadHiScore(char *sFileName, CLevelInfo *pInfo)
{
FILE *f;
int   iPos=0;
char  s1[50],*s2;

  if ((f=fopen(sFileName,"rt"))==NULL) return;

  fscanf(f,"%s",s1);

  do
  {
    s2=breakString(s1);

    if (s2)
    {
      strcpy(pInfo->cHiScore[iPos].sName,s1);
      pInfo->cHiScore[iPos].iTime=atoi(s2);

      iPos++;
    }
    fscanf(f,"%s",s1);
  }
  while (!feof(f) && iPos<10);

  fclose(f);
}

void CLevels::saveHiScore(char *sFileName, CLevelInfo *pInfo)
{
FILE *f;
int   iPos=0;
char  s1[50],*s2;

  if ((f=fopen(sFileName,"wt"))==NULL) return;

  for (iPos=0; iPos<10; iPos++)
    fprintf(f,"%s=%i\n",pInfo->cHiScore[iPos].sName,pInfo->cHiScore[iPos].iTime);

  fclose(f);
}

char *CLevels::breakString(char *sInput)
{
char *s;

  while ((s=strchr(sInput,'|'))!=NULL) *s=' ';
  s=strchr(sInput,'=');

  if (!s) return NULL;

  *s='\0';
  s++;

  return s;
}

int CLevels::getHiScore(int lvl, int pos)
{
  return m_pLevels[lvl].cHiScore[pos].iTime;
}

void CLevels::getHiScoreName(int lvl, int pos, wchar_t *sBuffer)
{
irr::core::stringw sDummy(m_pLevels[lvl].cHiScore[pos].sName);

  memcpy(sBuffer,sDummy.c_str(),sDummy.size()*sizeof(wchar_t)+1);
}

void CLevels::addHiScore(int lvl, int iTime, const wchar_t *sName)
{
int  iPos=getHiScorePos(lvl,iTime);
char sFileName[50];

  if (iPos>9) return;

  for (int i=9; i>iPos; i--)
  {
    m_pLevels[lvl].cHiScore[i].iTime=m_pLevels[lvl].cHiScore[i-1].iTime;
    strcpy(m_pLevels[lvl].cHiScore[i].sName,m_pLevels[lvl].cHiScore[i-1].sName);
  }

  m_pLevels[lvl].cHiScore[iPos].iTime=iTime;
  memset(m_pLevels[lvl].cHiScore[iPos].sName,0,20*sizeof(char));
  strcpy(m_pLevels[lvl].cHiScore[iPos].sName,irr::core::stringc(sName).c_str());

  strcpy(sFileName,m_pLevels[lvl].sFile);
  if (strchr(sFileName,'.')) *strchr(sFileName,'.')='\0';
  strcat(sFileName,".HiScore");
  saveHiScore(sFileName,&m_pLevels[lvl]);
}

int CLevels::getHiScorePos(int lvl, int iTime)
{
  for (int i=0; i<10; i++) if (m_pLevels[lvl].cHiScore[i].iTime>iTime || m_pLevels[lvl].cHiScore[i].iTime==0) return i;
  return 10;
}