
/*
Copyright (C) 2008   Mathias Gottschlag

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in the
Software without restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "MaterialEditor.h"

#include <Horde3D.h>
#include <Horde3DUtils.h>

MaterialEditor::MaterialEditor(Horde3DWidget *hordewidget, Glib::RefPtr<Gnome::Glade::Xml> ui) : hordewidget(hordewidget)
{
	this->ui = ui;
	
	initialized = false;
}
MaterialEditor::~MaterialEditor()
{
	if (initialized)
	{
	}
}

void MaterialEditor::show(void)
{
	ui->get_widget("matpreviewtype")->show();
	if (!initialized)
	{
		// Initialize scene
		pipeline = Horde3D::addResource(ResourceTypes::Pipeline, "forward.pipeline.xml", 0);
		cubemodel = Horde3D::addResource(ResourceTypes::SceneGraph, "cube.scene.xml", 0);
		int testoverlay = Horde3D::addResource(ResourceTypes::Material, "testoverlay.material.xml", 0);
		Horde3DWidget::loadResources();
		Horde3D::showOverlay( 0.75f, 0, 0, 0, 1, 0, 1, 0,
			1, 0.2f, 1, 1, 0.75f, 0.2f, 0, 1,
			7, testoverlay );
		cube = Horde3D::addNodes(RootNode, cubemodel);
		Horde3D::setNodeTransform(cube, 0, 0, 0, 0, 0, 0, 1, 1, 1);
		camera = Horde3D::addCameraNode(RootNode, "Camera", pipeline);
		hordewidget->setCamera(camera);
		Horde3D::setNodeTransform(camera, 0, 0, 2.5, 0, 0, 0, 1, 1, 1);
		Horde3D::setupCameraView(camera, 90.0f, (float)640/480, 0.1f,1000.0f);
		Horde3DUtils::dumpMessages();
		initialized = true;
	}
	printf("Showing material editor.\n");
}
void MaterialEditor::hide(void)
{
	printf("Hiding material editor.\n");
	ui->get_widget("matpreviewtype")->hide();
}

