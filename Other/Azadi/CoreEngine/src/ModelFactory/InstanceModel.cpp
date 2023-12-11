#include "InstanceModel.h"

InstanceModel::InstanceModel(CoreModel *cm)
{
    instModel = new CalModel(cm->coreMod);
    parentCore = cm;
    lastTick = 0;

    for (std::map<std::string, int>::iterator it = parentCore->meshes.begin(); it != parentCore->meshes.end(); ++it)
        instModel->attachMesh(it->second);

    renderer = instModel->getRenderer();

    x = y = z = rx = ry = rz = 0;

    textureFact = TextureFactory::getInstance();

    instModel->setMaterialSet(parentCore->materials["default"]);
    AnimationNames = cm->animNames;
}

InstanceModel::InstanceModel(InstanceModel *im)
{
    *this = im;
}

void InstanceModel::setPosition(const float &X, const float &Y, const float &Z)
{
    x = X; y = Y; z = Z;
}

void InstanceModel::setRotation(const float &RX, const float &RY, const float &RZ)
{
    rx = RX;
    ry = RY;
    rz = RZ;
}

void InstanceModel::startAction(const std::string &animName, const float &easeIn, const float &easeOut)
{
    instModel->getMixer()->executeAction(parentCore->animations[animName], easeIn, easeOut);
}

void InstanceModel::stopAction(const std::string &animName)
{
    instModel->getMixer()->removeAction(parentCore->animations[animName]);
}

void InstanceModel::startCycle(const std::string &animName, const float &weight, const float &delay)
{
    instModel->getMixer()->blendCycle(parentCore->animations[animName], weight, delay);
}

void InstanceModel::stopCycle(const std::string &animName, const float &delay)
{
    instModel->getMixer()->clearCycle(parentCore->animations[animName], delay);
}

void InstanceModel::update()
{
    if (lastTick != 0)
        instModel->update(float(SDL_GetTicks() - lastTick) / 1000);

    lastTick = SDL_GetTicks();
}

void InstanceModel::draw()
{
    BoundingBoxes.clear();
    glPushMatrix();

    const float axisRot = 90.0f;
    glRotatef(-axisRot, 1.0f, 0.0f, 0.0f);

    glTranslatef(x, y, z);
    glRotatef(rz, 0.0f, 0.0f, 1.0f);
    glRotatef(ry, 0.0f, 1.0f, 0.0f);
    glRotatef(rx, 1.0f, 0.0f, 0.0f);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glPushMatrix();

    renderer->beginRendering();

    for (unsigned long int i = 0; i < unsigned(renderer->getMeshCount()); ++i)
    {
        for (unsigned long int j = 0; j < unsigned(renderer->getSubmeshCount(i)); ++j)
        {
            renderer->selectMeshSubmesh(i, j);
            float verts[renderer->getVertexCount()][3];
            unsigned long int vertCount;

            vertCount = renderer->getVertices(&verts[0][0]);

            int faces[renderer->getFaceCount()][3];
            unsigned long int faceCount;

            float texCoords[renderer->getVertexCount()][2];
            renderer->getTextureCoordinates(0, &texCoords[0][0]);

            for (unsigned long int k = 0; k < unsigned(renderer->getMapCount()); ++k)
            {
                TextureFactory::getInstance()->applyTexture(parentCore->textures[(unsigned long int)renderer->getMapUserData(k)]);
            }

            faceCount = renderer->getFaces(&faces[0][0]);

            float hiX, loX;
            float hiY, loY;
            float hiZ, loZ;

            hiX = loX = verts[0][0];
            hiY = loY = verts[0][1];
            hiZ = loZ = verts[0][2];

            for (unsigned long int k = 1; k < vertCount; ++k)
            {
                if (verts[k][0] < loX)
                    loX = verts[k][0];
                else if (verts[k][0] > hiX)
                    hiX = verts[k][0];

                if (verts[k][1] < loY)
                    loY = verts[k][1];
                else if (verts[k][1] > hiY)
                    hiY = verts[k][1];

                if (verts[k][2] < loZ)
                    loZ = verts[k][2];
                else if (verts[k][2] > hiZ)
                    hiZ = verts[k][2];
            }

            glBegin(GL_TRIANGLES);
            for (unsigned long int k = 0; k < faceCount; ++k)
            {
                glTexCoord2f(texCoords[ faces[k][0] ][0], texCoords[ faces[k][0] ][1]);
                glVertex3f(verts[ faces[k][0] ][0], verts[ faces[k][0] ][1], verts[ faces[k][0] ][2]);
                glTexCoord2f(texCoords[ faces[k][1] ][0], texCoords[ faces[k][1] ][1]);
                glVertex3f(verts[ faces[k][1] ][0], verts[ faces[k][1] ][1], verts[ faces[k][1] ][2]);
                glTexCoord2f(texCoords[ faces[k][2] ][0], texCoords[ faces[k][2] ][1]);
                glVertex3f(verts[ faces[k][2] ][0], verts[ faces[k][2] ][1], verts[ faces[k][2] ][2]);
            }
            glEnd();

            if (1)
            {
                glDisable(GL_CULL_FACE);
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                glBegin(GL_QUADS);
                {
                    glVertex3f(loX, loY, loZ);
                    glVertex3f(hiX, loY, loZ);
                    glVertex3f(hiX, hiY, loZ);
                    glVertex3f(loX, hiY, loZ);

                    glVertex3f(loX, hiY, hiZ);
                    glVertex3f(hiX, hiY, hiZ);
                    glVertex3f(hiX, loY, hiZ);
                    glVertex3f(loX, loY, hiZ);

                    glVertex3f(loX, loY, loZ);
                    glVertex3f(hiX, loY, loZ);
                    glVertex3f(hiX, loY, hiZ);
                    glVertex3f(loX, loY, hiZ);

                    glVertex3f(loX, hiY, hiZ);
                    glVertex3f(hiX, hiY, hiZ);
                    glVertex3f(hiX, hiY, loZ);
                    glVertex3f(loX, hiY, loZ);
                }
                glEnd();
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                glEnable(GL_CULL_FACE);
            }

            float tloY = loY;
            float tloZ = loZ;
            float thiY = hiY;
            float thiZ = hiZ;

            loY = (tloY * (cos(axisRot * 0.0174532925f))) + (tloZ * (-sin(axisRot * 0.0174532925f)));
            hiY = (thiY * (cos(axisRot * 0.0174532925f))) + (thiZ * (-sin(axisRot * 0.0174532925f)));
            loZ = (tloY * (sin(axisRot * 0.0174532925f))) + (tloZ * (cos(axisRot * 0.0174532925f)));
            hiZ = (thiY * (sin(axisRot * 0.0174532925f))) + (thiZ * (cos(axisRot * 0.0174532925f)));

            std::vector<float> bbox;
            bbox.push_back(loX);
            bbox.push_back(loY);
            bbox.push_back(loZ);
            bbox.push_back(hiX);
            bbox.push_back(hiY);
            bbox.push_back(hiZ);

            BoundingBoxes.push_back(bbox);
        }
    }

    renderer->endRendering();

    glPopMatrix();
    glPopMatrix();
}

