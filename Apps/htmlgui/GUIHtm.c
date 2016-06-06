/*
    GUIHtml Version 1.1
    GUIHtml is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Author and copyright: 08-19-2009 Paolo stragapede
*/

#include "GUIHtm.h"
#include "SocketMngr.h"

typedef struct WidgetData_gui{
    char * data;
    struct WidgetData_gui * next;
}WidgetData_str;

//#define MAX_FILE_PATH_LEN  100
typedef struct ELEMENT_GUI{
    int ParentId;
    char * DataName;//FilesPath;//[MAX_FILE_PATH_LEN] ;
    char  TagName[30];
    char * Resurce;
    char * WidgetInit;
    char  DataType;
    int WidgetType ;
    void (*EventCallBack)(char * Tag,char * GUIevents);
    void (*CallBackGetData)(char * Tag,char * GUIevents);
    WidgetData_str * WidgetData;
    unsigned int Index;

}strGuiElement;

#define DEFAULT_HTML_PAGE 0

/// Informationa bout the type of file to be sent to the browser


#define WIDGET      1
#define DATA_REQUESTED   2

/// WIDGET TYPE
#define NOT_WIDGET         0
#define WIDGET_HTML_PAGE   1
#define WIDGET_IMAGE       2
#define WIDGET_TABLE       3
#define WIDGET_CHECKBOX    4
#define WIDGET_BUTTON      5
#define WIDGET_TEXTINPUT   6




#define FILE_ELEMENT       20
static strGuiElement GuiElement[FILE_ELEMENT];
static unsigned int GuiIndex =0;

static char  ReadBuff[5000];


static void WriteImage( strGuiElement GuiDef  );
static void SendBinaryGUI(strGuiElement GuiDef,char * cRequest);
static void SendHTMLGUI(strGuiElement GuiDef,char * cRequest);
static void WriteTableData( strGuiElement * GuiDef  );
static void WriteCheckBox( strGuiElement * GuiDef  );
static void WriteButton( strGuiElement GuiDef  );
static void WriteTextInput( strGuiElement *GuiDef  );

static int GetIdxByName(char *widgName);
static void DeleteWidgetData(WidgetData_str **  Wdata );

void InitGUI(unsigned int port){
	InitSockMnger(port);
	return;
}

void CheckGUI(void){
    unsigned int DataLen =0;
    unsigned int idx =0,txtpos =0;
    char fFound=0;
    char * End =NULL;
    char * DataSection = NULL;

    //"Server: GET"   "HTTP"
    DataLen =ReadSocket( ReadBuff,5000);

    if(DataLen && GuiIndex){

        End = strstr(ReadBuff ,"HTTP");
        if(End != NULL ){
            End[0] =0;
        }
        printf("Server: %s \n", ReadBuff);

        ///Check for User Command Events;
        for (idx =0; idx < GuiIndex ;idx++ ){

                if(  strstr(ReadBuff , GuiElement[idx].DataName ) != NULL  ){

                    if(WIDGET_BUTTON == GuiElement[idx].WidgetType && NULL != GuiElement[idx].EventCallBack){
                        if( NULL != GuiElement[idx].EventCallBack ){
	                        GuiElement[idx].EventCallBack( GuiElement[idx].DataName  , ReadBuff);
			}
                    }

                     if(WIDGET_CHECKBOX == GuiElement[idx].WidgetType   ){
			if( NULL != GuiElement[idx].EventCallBack ){
                        	GuiElement[idx].EventCallBack( GuiElement[idx].DataName  , ReadBuff);
                        }
                    }

		    if(WIDGET_TEXTINPUT == GuiElement[idx].WidgetType   ){
			if( NULL != GuiElement[idx].EventCallBack ){
                                DataSection =  strstr(ReadBuff ,GuiElement[idx].DataName);
				if(DataSection != NULL ){
				    DataSection += strlen(GuiElement[idx].DataName);
                                    DataSection += strlen("="); //skip the '=' present in the form spec.
		                    for(txtpos = 0; DataSection[txtpos];txtpos++){
				        if('+' == DataSection[txtpos]){
						DataSection[txtpos] = ' ' ;
					}
				    }
				    GuiElement[idx].EventCallBack( GuiElement[idx].DataName  , DataSection);
				}							
                        }
                    }


                    if(WIDGET_IMAGE == GuiElement[idx].WidgetType){
                            SendBinaryGUI(GuiElement[idx],ReadBuff);
                            fFound=1;
                            break;
                    }

                    if(WIDGET_HTML_PAGE == GuiElement[idx].WidgetType ){
                                SendHTMLGUI(GuiElement[idx],ReadBuff);
                                fFound=1;
                                break;
                    }
                }
        }

        ///if no name may be default page or form without target for html request send default document;
        if(!fFound){
            for (idx =0; idx < GuiIndex;idx++ ){
                if(WIDGET_HTML_PAGE == GuiElement[idx].WidgetType){
                    SendHTMLGUI(GuiElement[idx],ReadBuff);
                    break;
                }
            }
        }
	CloseTransaction();
        
    }

 return;
}


///Description: function used to send Data for Event widget
///- a call back to retrive the data the callback return 0 no more data 1 others data.
///- a string for the C format data.
static void WriteImage( strGuiElement GuiDef  ){

    //int iRet=0;
    //char * cGetData = NULL;
    char cBuff[200];
    sprintf(cBuff,"<IMG src='%s'  name ='SatPath.bmp' border=0>",GuiDef.DataName);
    WriteSocket(cBuff);

    return;
}



///Description: function used to send Data for Event widget
///- a call back to retrive the data the callback return 0 no more data 1 others data.
///- a string for the C format data.
static void WriteButton( strGuiElement GuiDef  ){

    //int iRet=0;
    //char * cGetData = NULL;
    char cBuff[200];

    sprintf(cBuff,"<FORM name='%s' >",GuiDef.DataName);
    WriteSocket(cBuff);
    sprintf(cBuff,"\n\r<INPUT TYPE='HIDDEN' name = '%s'  value ='%s'> ",GuiDef.DataName,GuiDef.DataName);
    WriteSocket(cBuff);
    sprintf(cBuff,"<INPUT TYPE = 'SUBMIT'  value ='%s'>\n</FORM>",GuiDef.DataName);
	WriteSocket(cBuff);

    return;
}

///Description: function used to send Data for Text Input group widget
///- a call back to retrive the data the callback return 0 no more data 1 others data.
///- a string for the C format data.
static void WriteTextInput( strGuiElement *GuiDef  ){
    char cBuff[200];
    WidgetData_str ** PointerData =NULL ;


    sprintf(cBuff,"<FORM name='%s'",GuiDef->DataName);
    WriteSocket(cBuff);

    if(NULL != GuiDef->WidgetInit){
        WriteSocket(GuiDef->WidgetInit);
    }

    if(NULL != GuiDef->CallBackGetData){
	    GuiDef->CallBackGetData(GuiDef->DataName,"");
    }

    sprintf(cBuff,"\n\r<INPUT TYPE='text'  name ='%s' value='",GuiDef->DataName);
    WriteSocket(cBuff);

    if(NULL != GuiDef->WidgetData){
        PointerData = &GuiDef->WidgetData;
        
        while(NULL != *PointerData){
           
            sprintf(cBuff,"%s \n\r",(*PointerData)->data);
            WriteSocket(cBuff);
            PointerData = &(*PointerData)->next;
        }
    }
    sprintf(cBuff,"'>");
    WriteSocket(cBuff);

    sprintf(cBuff,"<INPUT TYPE = 'SUBMIT'  value ='%s'>\n</FORM>",GuiDef->DataName);
    WriteSocket(cBuff);

    return;
}


///Description: function used to send Data for checkbox group widget
///- a call back to retrive the data the callback return 0 no more data 1 others data.
///- a string for the C format data.
static void WriteCheckBox( strGuiElement *GuiDef  ){
    char cBuff[200];
    WidgetData_str ** PointerData =NULL ;


    sprintf(cBuff,"<FORM name='%s'",GuiDef->DataName);
    WriteSocket(cBuff);

    if(NULL != GuiDef->WidgetInit){
        WriteSocket(GuiDef->WidgetInit);
    }

    if(NULL != GuiDef->CallBackGetData){
	    GuiDef->CallBackGetData(GuiDef->DataName,"");
    }

    if(NULL != GuiDef->WidgetData){
        PointerData = &GuiDef->WidgetData;
        while(NULL != *PointerData){
            WriteSocket("\n\r<INPUT TYPE='checkbox' ");
            sprintf(cBuff,"name ='%s' value='%s'> %s <BR>\n\r",GuiDef->DataName,(*PointerData)->data,(*PointerData)->data);
            WriteSocket(cBuff);
            PointerData = &(*PointerData)->next;
        }
    }

    sprintf(cBuff,"<INPUT TYPE = 'SUBMIT'  value ='%s'>\n</FORM>",GuiDef->DataName);
    WriteSocket(cBuff);

    return;
}


///Description: function used to send Data for Table widget
///- a call back to retrive the data the callback return 0 no more data 1 others data.
///- a string for the C format data.
static void WriteTableData( strGuiElement * GuiDef  ){

    char cBuff[255];
 
    WidgetData_str ** PointerData =NULL ;

    WriteSocket("<TABLE ");
    if(NULL != GuiDef->WidgetInit){
        WriteSocket(GuiDef->WidgetInit);
    }
    WriteSocket(">");
    WriteSocket("\n\r<TR>");


    if( NULL != GuiDef->CallBackGetData){
          GuiDef->CallBackGetData(GuiDef->DataName,"");
    }
    if(NULL != GuiDef->WidgetData){
        PointerData = &GuiDef->WidgetData;
        while(NULL != *PointerData){
            if(0 == strcmp("\n",(*PointerData)->data) ){
                WriteSocket("</TR>");
                WriteSocket("<TR>");
            }else{
                sprintf(cBuff,"<TD>%s</TD> \n\r",(*PointerData)->data);
                WriteSocket(cBuff);
            }
            PointerData = &(*PointerData)->next;
        }

    }
    WriteSocket("</TR>");
    WriteSocket("</TABLE>");
    return;
}
///Description: Send data formatted for html table to web by writing on the socket,
void WriteWidgetField(char * widgName, char * Data){
    int   DataLen=0,GuiIndex=0;
    WidgetData_str ** PointerData =NULL ;

    DataLen = strlen(Data);
    GuiIndex= GetIdxByName(widgName);
    if(-1 == GuiIndex){
               printf(" bad name for widget");
               return;
    }
    PointerData = &GuiElement[GuiIndex].WidgetData;
    while(NULL != *PointerData){
                PointerData = & (*PointerData)->next;
    }
    *PointerData =(WidgetData_str *)  malloc(sizeof(WidgetData_str));
    (*PointerData)->data =(char *)  malloc( DataLen + 1 );
    (*PointerData)->next = NULL;
    strcpy( (*PointerData)->data,Data);

    return;
}
///Description: Send data formatted for html table to web by writing on the socket,
void WriteWidgetNewRecord(char * widgName){
    int GuiIndex= 0;
    GuiIndex= GetIdxByName(widgName);
    if(-1 == GuiIndex){
               printf(" bad name for widget");
               return;
    }
    switch(GuiElement[GuiIndex].WidgetType  ){
        case WIDGET_TABLE:
            WriteWidgetField(widgName, "\n");
        break;
        default:
        break;
    }
    return;
}


///Description: Send the required HTML page  
///- A template is used to store widgets in form of comment 
///- Code insert HTML tags for the widget 
static void SendHTMLGUI(strGuiElement GuiDef,char * cRequest){
    FILE * DataSock = NULL;
    unsigned int ReadNum =0;
    char cBuff;
    unsigned int idx=0;

        if(GuiDef.EventCallBack != NULL){
            GuiDef.EventCallBack( GuiDef.DataName  , cRequest);
        }

       for (idx =0; idx < GuiIndex;idx++ ){
            GuiElement[idx].Index=0;
       }

       DataSock =fopen(GuiDef.Resurce, "r");
       if(NULL == DataSock){
		printf("Can not find HTML page template in the working directory\n\r");
		return;
       }

       while(1){
            ReadNum = fread ( &cBuff,1,1,DataSock);
            if(ReadNum){

                    WriteSocketLen( &cBuff, 1);

                    for (idx =0; idx < GuiIndex;idx++ ){
                        if(  GuiDef.ParentId==GuiElement[idx].ParentId && WIDGET == GuiElement[idx].DataType ){
                            if( GuiElement[idx].TagName[GuiElement[idx].Index] == cBuff ){
                                GuiElement[idx].Index++;
                                if( 0 == GuiElement[idx].TagName[GuiElement[idx].Index]   ){
                                    GuiElement[idx].Index=0;

                                    switch(GuiElement[idx].WidgetType){
                                     case WIDGET_TABLE:
                                        WriteTableData(&GuiElement[idx]);
                                     break;
                                     case WIDGET_CHECKBOX:
                                        WriteCheckBox(& GuiElement[idx]);
                                     break;
                                     case WIDGET_TEXTINPUT:
                                        WriteTextInput(& GuiElement[idx]);
                                     break; 
                                     case WIDGET_BUTTON:
                                        WriteButton(GuiElement[idx]);
                                     break;
                                     case WIDGET_IMAGE:
                                        WriteImage(GuiElement[idx]);
                                     break;
                                     default:

                                     break;
                                    }

                                }
                            }else{
                                GuiElement[idx].Index=0;
                            }
                        }
                    }
            }else{
             break;
            }
        }
        fclose(DataSock);

    return;
}

static void SendBinaryGUI(strGuiElement GuiDef,char * cRequest){
    FILE * DataSock = NULL;
    int ReadNum =0;

        if(NULL != GuiDef.EventCallBack ){
            GuiDef.EventCallBack( GuiDef.DataName  , cRequest);
        }

        DataSock =fopen(GuiDef.Resurce, "rb");
        if(NULL != DataSock){
		while(1){
		    ReadNum = fread ( ReadBuff,1,5000,DataSock);
		    if(ReadNum){
		         WriteSocketLen(ReadBuff, ReadNum);
		    }else{
		     break;
		    }
		}
		fclose(DataSock);
	}
        return;
}



/// Description: Load GUI element structure with information neded to write GUI
/// - path file template or file requested by the Browser
/// - Tag Name in the file where the data must be purtted int
/// - Type the File to be loaded and sent to the browser has a format bynary or text use "rb" or "r"
/// - Action if the data must be appended to copleate a Tag elemant (the Tag neme)
/// -PUT NULL for not used data.
/// - return 0 if elements exeded the static array dimension
char AddHtmlPage(int Id,char * path, void (*EvntFunc)(char * Tag,char * GUIevents)){
    char cRet =0;

    if(GuiIndex < FILE_ELEMENT ){
        GuiElement[GuiIndex].ParentId= Id;
        GuiElement[GuiIndex].DataName ="HTML";
        GuiElement[GuiIndex].Resurce = path;
        GuiElement[GuiIndex].DataType = DATA_REQUESTED ;
        GuiElement[GuiIndex].WidgetType = WIDGET_HTML_PAGE ;
        GuiElement[GuiIndex].CallBackGetData = NULL;
        GuiElement[GuiIndex].EventCallBack = EvntFunc ;
        GuiElement[GuiIndex].Index=0;
        GuiElement[GuiIndex].WidgetData = NULL;
        GuiIndex++;
        cRet =1;
    }

    return cRet;
}


char AddImage(int Id,char * WidgetName,char *path, void (*EvntFunc) (char * Tag,char * GUIevents) ){
    char cRet =0;

    if(strlen(WidgetName) > 20 ){
      printf("Widget name too long msaximun 20 chars\n\r");
      return 0;
    }

    if(GuiIndex < FILE_ELEMENT ){
        GuiElement[GuiIndex].ParentId= Id;
        GuiElement[GuiIndex].DataName =WidgetName;
        sprintf(GuiElement[GuiIndex].TagName,"<!--%s-->",WidgetName);
        GuiElement[GuiIndex].Resurce = path;
        GuiElement[GuiIndex].DataType = WIDGET;
        GuiElement[GuiIndex].WidgetType = WIDGET_IMAGE;
        GuiElement[GuiIndex].CallBackGetData = NULL;
        GuiElement[GuiIndex].EventCallBack = EvntFunc ;
        GuiElement[GuiIndex].Index=0;
        GuiElement[GuiIndex].WidgetData = NULL;
        GuiIndex++;
        cRet =1;
    }

    return cRet;
}

char AddUserButton(int Id,char  * WidgetName,void (*EvntFunc)(char * Tag,char * GUIevents)){
    char cRet =0;
    if(strlen(WidgetName) > 20 ){
      printf("Widget name too long msaximun 20 chars\n\r");
      return 0;
    }
    if(GuiIndex < FILE_ELEMENT ){
        GuiElement[GuiIndex].ParentId= Id;
        GuiElement[GuiIndex].DataName =WidgetName;
        sprintf(GuiElement[GuiIndex].TagName,"<!--%s-->",WidgetName);
        GuiElement[GuiIndex].DataType = WIDGET;
        GuiElement[GuiIndex].WidgetType =  WIDGET_BUTTON;
        GuiElement[GuiIndex].CallBackGetData = NULL;
        GuiElement[GuiIndex].EventCallBack = EvntFunc ;
        GuiElement[GuiIndex].Index=0;
        GuiElement[GuiIndex].WidgetData = NULL;
        GuiIndex++;
        cRet =1;
    }

    return cRet;

}


char AddTextInput( int Id,char * WidgetName ,char *Init, void  (*GetDataFunc)(char * Tag,char * GUIevents), void (*EvntFunc)(char * Tag,char * GUIevents) ){

    char cRet =0;
    if(strlen(WidgetName) > 20 ){
      printf("Widget name too long msaximun 20 chars\n\r");
      return 0;
    }
    if(GuiIndex < FILE_ELEMENT ){
        GuiElement[GuiIndex].ParentId= Id;
        GuiElement[GuiIndex].DataName =WidgetName;
        sprintf(GuiElement[GuiIndex].TagName,"<!--%s-->",WidgetName);
        GuiElement[GuiIndex].WidgetInit = Init;
        GuiElement[GuiIndex].DataType = WIDGET;
        GuiElement[GuiIndex].WidgetType = WIDGET_TEXTINPUT;
        GuiElement[GuiIndex].CallBackGetData = GetDataFunc;
        GuiElement[GuiIndex].EventCallBack = EvntFunc;
        GuiElement[GuiIndex].Index=0;
        GuiElement[GuiIndex].WidgetData = NULL;

        GuiIndex++;
    }


    return cRet;

}

char AddCheckBox( int Id,char * WidgetName ,char *Init, void  (*GetDataFunc)(char * Tag,char * GUIevents), void (*EvntFunc)(char * Tag,char * GUIevents) ){

    char cRet =0;
    if(strlen(WidgetName) > 20 ){
      printf("Widget name too long msaximun 20 chars\n\r");
      return 0;
    }
    if(GuiIndex < FILE_ELEMENT ){
        GuiElement[GuiIndex].ParentId= Id;
        GuiElement[GuiIndex].DataName =WidgetName;
        sprintf(GuiElement[GuiIndex].TagName,"<!--%s-->",WidgetName);
        GuiElement[GuiIndex].WidgetInit = Init;
        GuiElement[GuiIndex].DataType = WIDGET;
        GuiElement[GuiIndex].WidgetType = WIDGET_CHECKBOX;
        GuiElement[GuiIndex].CallBackGetData = GetDataFunc;
        GuiElement[GuiIndex].EventCallBack = EvntFunc;
        GuiElement[GuiIndex].Index=0;
        GuiElement[GuiIndex].WidgetData = NULL;

        GuiIndex++;
    }


    return cRet;

}


char AddTable( int Id,char * WidgetName ,char * Init, void  (*GetDataFunc)(char * Tag,char * GUIevents)){

    char cRet =0;

    if(strlen(WidgetName) > 20 ){
      printf("Widget name too long msaximun 20 chars\n\r");
      return 0;
    }

    if(GuiIndex < FILE_ELEMENT ){
        GuiElement[GuiIndex].ParentId= Id;
        GuiElement[GuiIndex].DataName =WidgetName;
        sprintf(GuiElement[GuiIndex].TagName,"<!--%s-->",WidgetName);
        GuiElement[GuiIndex].WidgetInit = Init;
        GuiElement[GuiIndex].DataType = WIDGET;
        GuiElement[GuiIndex].WidgetType = WIDGET_TABLE;
        GuiElement[GuiIndex].CallBackGetData = GetDataFunc;
        GuiElement[GuiIndex].EventCallBack = NULL;
        GuiElement[GuiIndex].Index=0;
        GuiElement[GuiIndex].WidgetData = NULL;

        GuiIndex++;
        cRet =1;
    }

    return cRet;

}

///Description: function used to clear specific widget data
void ClearWidgetData(char  * wdataName){
    int GuiIndex =0;

    GuiIndex = GetIdxByName(wdataName);
    if(GuiIndex >= 0){
        DeleteWidgetData(&GuiElement[GuiIndex].WidgetData);
    }
    return;
}

///Description: free dinamic memory used for widgets
void DeleteWidgets(void){
    int GuiIndex=0;

    for (GuiIndex=0;GuiIndex< FILE_ELEMENT ;GuiIndex++){
            DeleteWidgetData(&GuiElement[GuiIndex].WidgetData);
    }
    return;
}

///Description: function used to clear specific widget data: #start clear from last element backwise#
static void DeleteWidgetData(WidgetData_str * *  Wdata ){
    WidgetData_str ** PointerData =NULL ;
    WidgetData_str **  ClearNext =NULL ;
    WidgetData_str * Temp; ///Used to avoid clear random memory area as there is not a next pointer to the first element

    while(NULL != *Wdata ){
        PointerData =  Wdata;
        ClearNext = &Temp;
        while(NULL != *Wdata ){
                if(NULL == (*PointerData)->next ){
                    free ( (*PointerData)->data );
                    free (*PointerData );
                    *PointerData = NULL;
                    *ClearNext = NULL;
                    break;
                }else{
                    ClearNext = &(*PointerData)->next;
                    PointerData = &(*PointerData)->next;
                }
        }
    }

    return;
}


///Description: find array index in the gui element array.
static int GetIdxByName(char *widgName){
 int GuiIndex =0,iRet= -1;

 for (GuiIndex=0;GuiIndex< FILE_ELEMENT ;GuiIndex++){
     if(strcmp(GuiElement[GuiIndex].DataName ,widgName)==0){
      iRet = GuiIndex;
      break;
    }
 }
   return iRet;
}

///Drop Memory and close open connection;
void ExitGUI (void){
	CloseConnection();
        DeleteWidgets();
	return;
}
