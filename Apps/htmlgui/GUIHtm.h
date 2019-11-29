/*
    Copyright (C) 2009 Paolo Stragapede
    Author and copyright: 08-19-2009 Paolo stragapede
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

*/

#ifndef _USER_GUI_
#define _USER_GUI_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//#include "SocketMngr.h"


#ifdef __cplusplus
extern "C" {
#endif


void InitGUI(unsigned int port);

void CheckGUI(void); ///Wait Event from the Browser Client , user

///HTML Add an HTML template page
char AddHtmlPage(int Id,char * path, void (*EvntFunc)(char * Tag,char * GUIevents));

///BUTTON
 char AddUserButton(int Id,char  * WidgetName,void (*EventFunc)(char * Tag,char * GUIevents));

///TABLE
 char AddTable( int Id,char * WidgetName ,char *Init, void  (*GetDataFunc)(char * Tag,char * GUIevents));

/// CHECK BOX
 char AddCheckBox( int Id,char * WidgetName ,char *Init, void  (*GetDataFunc)(char * Tag,char * ParamStr),  void (*EvntFunc)(char * Tag,char * GUIevents) );

/// TEXT INPUT 
 char AddTextInput( int Id,char * WidgetName ,char *Init, void  (*GetDataFunc)(char * Tag,char * ParamStr),  void (*EvntFunc)(char * Tag,char * GUIevents) );

 ///IMAGES
  char AddImage(int Id,char * WidgetName ,char *path,  void (*EvntFunc)(char * Tag,char * GUIevents));

///WIDGETS
///Description: write data in the widget memory, -parameters: widget name , data to be stored
void WriteWidgetField(char * widgName, char * Data);
/// write a new recrod tag in the widget data area.
void WriteWidgetNewRecord(char * widgName);
/// Free the data area dinamic memory for all widgets
void DeleteWidgets(void);
/// Free the data area dinamic memory for the specific widgets name
void ClearWidgetData(char  * wdataName);

/// Close the GUI interface
void ExitGUI (void);

#ifdef __cplusplus
}
#endif

#endif // LOG_H

