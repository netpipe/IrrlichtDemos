#ifdef PYTHON
#include<irrlicht.h>
#include<Python.h>
#include<stdio.h>
#include<vector>
#include<string.h>
#include<stdlib.h>

using namespace std;

using namespace irr;
using namespace core;
using namespace video;
using namespace gui;
using namespace scene;
using namespace io;


namespace Python {


    void ExecuteScript(irr::core::string<char> scriptname);

    IrrlichtDevice * device;
    IVideoDriver * driver;
    ISceneManager * smgr;
    IGUIEnvironment * guienv;
    IGUIFont * default_font;
    vector<ITexture *> texture_array; //Our array of textures
    void registerScene(ISceneManager &scene);
    void registerIrrDevice(IrrlichtDevice &device);
    PyObject * PyIrr_LoadTexture(PyObject * self,PyObject * args);
    PyObject * PyIrr_SetTexture(PyObject * self,PyObject * args);
    PyObject * PyIrr_AddCubeSceneNode(PyObject * self,PyObject * args);
    PyObject * PyIrr_DrawText(PyObject * self,PyObject * args);

    PyMODINIT_FUNC init_irr(void);

};
void Python::registerIrrDevice(IrrlichtDevice &Device)
{
    device = &Device;
}
void Python::registerScene(ISceneManager &scene)
{
    smgr = &scene;
}

PyObject * Python::PyIrr_LoadTexture(PyObject * self,PyObject * args)
{
	//Watch this, tricky,remember to pass string ADDRESS to PyArg_ParseTuple
	char * tex_name;
	PyArg_ParseTuple(args,"s",&tex_name);
	texture_array.push_back(driver->getTexture(tex_name));
	/*The line below is sorta kludgy, but it works.It won't hold up if you remove a texture
	from the array though,so watch your step, kid. I'll leave it to you to come up with
	a more intuitive method of storing the textures*/
	return Py_BuildValue("l",texture_array.size() - 1);
};

PyObject * Python::PyIrr_SetTexture(PyObject * self,PyObject * args)
{
	s32 tex_id,node_id;
	PyArg_ParseTuple(args,"ll",&node_id,&tex_id); //This is your new best friend, seriously
	/*Quite similar to the scanf family of functions, don't you think? It take a format
	string and some input data, and analyzes the input data and gives you the result
	in a manner specified by the format string*/
	ISceneNode * node = smgr->getSceneNodeFromId(node_id);
	if(node != NULL)
	{
		node->setMaterialTexture(0,texture_array[tex_id]);
	};
	/*This line returns a value of Py_None, which is more or less the same thing as a
	function with a return type of void in C++, in other words, no output values */
	return Py_BuildValue("");
};

PyObject * Python::PyIrr_AddCubeSceneNode(PyObject * self,PyObject * args)
{
	s32 node_id;
	float size;
	float px,py,pz,rx,ry,rz,sx,sy,sz;
	//Damn...thats a lot of parameters :)
	PyArg_ParseTuple(args,"lffffffffff",&node_id,&size,&px,&py,&pz,&rx,&ry,&rz,&sx,&sy,&sz);

	ISceneNode * node = smgr->getSceneNodeFromId(node_id);
	if(node == NULL)
	{
		node = smgr->addCubeSceneNode(size,NULL,node_id,vector3df(px,py,pz),vector3df(rx,ry,rz),vector3df(sx,sy,sz));
		/*I forgot this initially, so I wasn't seeing anything although the script
		was compiling >_<...stupid*/
		node->setMaterialFlag(EMF_LIGHTING,false);
	}
	else
	{
		return Py_BuildValue("");
	};
	return Py_BuildValue("l",node_id);

};

PyObject * Python::PyIrr_DrawText(PyObject * self,PyObject * args)
{

	//Must make this useful someday, not today
	char * message;
	s32 x,y,x1,y1;
	PyArg_ParseTuple(args,"sllll",&message,&x,&y,&x1,&y1);
	//The next three lines more or less give the procedure from converting a string of
	//type char* to wchar_t*...generally this is pretty useful in Irrlicht,so note well
	//int len = strlen(message) + 1;
	//wchar_t * conv_message = new wchar_t[len];
	//!mbstowcs(0,conv_message,len,message,_TRUNCATE);
	//default_font->draw(conv_message,rect<s32>(x,y,x1,y1),SColor(255,255,255,255));
	//delete [] conv_message;
	return Py_BuildValue("");

};


PyMethodDef irr_funcs[] =
{
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
	{"set_texture",Python::PyIrr_SetTexture,METH_VARARGS,"Adds a texture to a scene node"},
	{"draw_text",Python::PyIrr_DrawText,METH_VARARGS,"Renders text to the screen with default font"},
	{"add_cube",Python::PyIrr_AddCubeSceneNode,METH_VARARGS,"Adds a cube scene node"},
	{"load_texture",Python::PyIrr_LoadTexture,METH_VARARGS,"Loads a texture"},
	{NULL,NULL,0,NULL}
};


PyMODINIT_FUNC Python::init_irr(void)
{
	/*Note the return type of the function, which is PyMODINIT_FUNC, its
	actually more of a macro than a datatype(I think). Anyway, its essential to
	have this as the return type of a function that initializes a python module*/
    Py_InitModule("irr",irr_funcs);
};


void Python::ExecuteScript(irr::core::string<char> scriptname)
{
	/*Using it this way because the methods using the FILE * structure tend to go wonky
	  due to the fact that the FILE * datatype is not standardized, so you'll get problems
	  if you try to use it with a different version other than the one your Python build was
	  compiled with. Our solution looks kludgy but it works pretty well.*/
	irr::core::string<char> result;
	result = irr::core::string<char>("execfile('");
	result.append(scriptname);
	result.append("')");
	PyRun_SimpleString(result.c_str());
	/*This function was pretty annoying to write. The errors I kept on getting were textbook
	examples on why namespaces were important...seriously.It kept on confusing std::string
	with irr::core::string, but as you can see, I solved that problem by explicitly stating
	the namespace in which the string class I wanted was to be found*/
};
#endif
