#ifndef PYTHON_H
#define PYTHON_H

#include <python2.7/Python.h>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <stdlib.h>
#ifdef TAR1
#include <libtar.h>
#endif
#include <QString>

#include <mainwindow.h>
#include "ui_mainwindow.h"


#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <libtar.h>
#endif


#include <fcntl.h>
using namespace std;

//vector<ITexture *> texture_array; //Our array of textures

QString test3;

void ExecuteScript(QString scriptname);


void MainWindow::settext(){

    ui->label->setText(test3.toLatin1());

}

static PyObject * qt_TextBox(PyObject * self,PyObject * args)
{
//    //Watch this, tricky,remember to pass string ADDRESS to PyArg_ParseTuple
    char * tex_name;
    PyArg_ParseTuple(args,"s",&tex_name);
//    texture_array.push_back(driver->getTexture(tex_name));
//    /*The line below is sorta kludgy, but it works.It won't hold up if you remove a texture
//    from the array though,so watch your step, kid. I'll leave it to you to come up with
//    a more intuitive method of storing the textures*/

    test3 = tex_name;//"rffr"; //tex_name;
    return Py_None;
   // settext(test);
};

PyObject * qt_using(PyObject * self,PyObject * args){
    char * script;
    char * arg;
    PyArg_ParseTuple(args,"ss",&script,&arg);

    ExecuteScript(script);
return Py_BuildValue("");
}

PyObject * qt_Delay(PyObject * self,PyObject * args){ //PyIrr_Delay
    //repurpose this for a path move delay
    float delay;
    PyArg_ParseTuple(args,"f",&delay);
 //   device->sleep(delay);
    usleep(delay);
return Py_BuildValue("");
}

PyObject * qt_Sleep(PyObject * self,PyObject * args){
    int ammount;
    PyArg_ParseTuple(args,"i",&ammount);
    usleep(ammount);
//    device->sleep(ammount);
return Py_BuildValue("");
}

static PyObject * qt_LoadTexture(PyObject * self,PyObject * args)
{
//    //Watch this, tricky,remember to pass string ADDRESS to PyArg_ParseTuple
    char * tex_name;
    PyArg_ParseTuple(args,"s",&tex_name);
//    texture_array.push_back(driver->getTexture(tex_name));
//    /*The line below is sorta kludgy, but it works.It won't hold up if you remove a texture
//    from the array though,so watch your step, kid. I'll leave it to you to come up with
//    a more intuitive method of storing the textures*/

    test3 += tex_name;//"rffr"; //tex_name;
    return Py_None;
   // settext(test);
};

PyObject * qt_exec(PyObject * self,PyObject * args){ //active camera
char * script;
char * arg;
    PyArg_ParseTuple(args,"ss",&script,&arg);
//sts = system(command);s
//popen("wget http://google.com", "w");
}

PyObject * qt_getTime(PyObject * self,PyObject * args){ //active camera
    int type;
    PyArg_ParseTuple(args,"i",&type);
//    u32 tm = device->getTimer()->getRealTime();
    //device->sleep(ammount);
//return Py_BuildValue("i",tm);
}

/*Yup, this is an array of type PyMethodDef containing all the functions which we want
to expose to the Python interpreter. The first parameter is a string telling what the name
of the function will be inside the script. The second parameter is the function in C++ code
that corresponds to it. The third parameter tells the number of arguments of the function.
Generally, you just leave that as METH_VARARGS and there's no problem, but if you like
bringing down problems on yourself, you can check the Python Documentation to see the
alternative parameters. The fourth parameter is a quick description of the function,
a 'docstring' I believe they call it. Its pretty useful, especially when you're modifying
the sourcecode. You can also get the docstring from the Python script by typing something in
the form of [module].[function or class].__doc__ to get a docstring. To try this, you can
boot up your Python IDE and type import math. Press enter, then type: math.sin.__doc__. Note
that there are two underscores in front and behind of doc, that is not a typo.*/
static PyMethodDef qt_funcs[] =
{
    {"gettime",qt_getTime,METH_VARARGS,"Adds a texture to a scene node"},
    {"exec",qt_exec,METH_VARARGS,"Adds a texture to a scene node"},
    {"sleep",qt_Sleep,METH_VARARGS,"Adds a texture to a scene node"},
    {"delay",qt_Delay,METH_VARARGS,"Adds a texture to a scene node"},
    {"using",qt_using,METH_VARARGS,"Adds a texture to a scene node"},
    {"settext",qt_TextBox,METH_VARARGS,"Adds a texture to a scene node"},
    {NULL,NULL,0,NULL}
};

//The initialization function, this links everything
PyMODINIT_FUNC init_qt(void)
{
    /*Note the return type of the function, which is PyMODINIT_FUNC, its
    actually more of a macro than a datatype(I think). Anyway, its essential to
    have this as the return type of a function that initializes a python module*/
    Py_InitModule("QT",qt_funcs);
};

void ExecuteScript(QString scriptname)
{
    /*Using it this way because the methods using the FILE * structure tend to go wonky
      due to the fact that the FILE * datatype is not standardized, so you'll get problems
      if you try to use it with a different version other than the one your Python build was
      compiled with. Our solution looks kludgy but it works pretty well.*/
    QString result;
    result = "execfile('";
    result.append(scriptname.toStdString().c_str());
    result.append("')");
    PyRun_SimpleString(result.toLocal8Bit().data());
    //PyRun_SimpleString("./main.pys");
    /*This function was pretty annoying to write. The errors I kept on getting were textbook
    examples on why namespaces were important...seriously.It kept on confusing std::string
    with irr::core::string, but as you can see, I solved that problem by explicitly stating
    the namespace in which the string class I wanted was to be found*/
};


void MainWindow::qtPython()
{
   #ifdef TAR11 //for emscripten
    TAR* tar;
    if (tar_open(&tar, "./pydata.tar", NULL, O_RDONLY, 0, 0) != 0) {
        fprintf(stderr, "Error: failed to open pydata.tar\n");
        exit(1);
    }
    if (tar_extract_all(tar, (char*) "/") != 0) {
        fprintf(stderr, "Error: failed to extract pydata.tar\n");
        exit(1);
    }
    tar_close(tar);


   Py_Initialize(); //Initialize Python
   setenv("PYTHONHOME", "/", 0);
#else
    Py_Initialize(); //Initialize Python
#endif

    init_qt(); //Initialize our module

    #ifdef TAR1
    ExecuteScript("./main.pys"); //Using our handy dandy script execution function
#else
    ExecuteScript("./windows.pys");
#endif
//emscripten_exit_with_live_runtime();

    Py_Finalize(); //Shut down the Python interpreter

}

#endif // PYTHON_H
