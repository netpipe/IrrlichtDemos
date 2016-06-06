
#include "tesselatedImage.h"


TesselatedImage::TesselatedImage(IrrlichtDevice *device, stringc img, vector3df startFr, vector3df startTo, vector3df targetPos, float speed, int maxTes, float colChange, Type type, float dim, float gap)
{
    this->startFrom = startFr;
    this->startTo = startTo;
    this->targetPos = targetPos;
    this->maxTesserae = maxTes;
    this->colorChangesPerSecond = colChange;
    this->type = type;
    this->dim = dim;
    this->gap = gap;
    this->speed = speed;
    polyCount = 8;

    IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
    IImage *image = driver->createImageFromFile(img.c_str());



    if (!image)
    {
        //throw new std::exception("Could not load image.");
        return;
    }

    int x = image->getDimension().Width;
    int y = image->getDimension().Height;
    int sumPixel = x * y;

    int pixelPerObject = 0;

    // determine how many pixels need to get summed up to stay within the limit of max objects
    int neededObjects = 0;
    do
    {
        pixelPerObject++;
        columns = x / pixelPerObject + (x % pixelPerObject ? 1 : 0);
        rows    = y / pixelPerObject + (y % pixelPerObject ? 1 : 0);
        neededObjects = columns * rows;
    }
    while (neededObjects > maxTesserae);

    // go through all the pixels of the image and create an array element for every block of 'pixelPerObject' pixels
    // the color values are remembered to calculate the average at the end
    int arrayIndex = 0;
    NodeInfo *curNode = 0;
    SColor pixelColor;
    for (int j=0; j < y; j++)
    {
        for (int i=0; i < x; i++)
        {
            arrayIndex = i / pixelPerObject + ((j / pixelPerObject) * columns);

            // check when to create a new array element for a pixel block
            if (nodeInfo.size() <= (unsigned)arrayIndex)
            {
                curNode = new NodeInfo();
                nodeInfo.push_back(curNode);
            }
            else
            {
                curNode = nodeInfo[arrayIndex];
            }

            // update node info, one more pixel involved and add its color to build average color later
            curNode->pixelInvolved++;
            pixelColor = image->getPixel(i, j);
            curNode->color[0] += pixelColor.getAlpha();
            curNode->color[1] += pixelColor.getRed();
            curNode->color[2] += pixelColor.getGreen();
            curNode->color[3] += pixelColor.getBlue();
        }
    }

    // now go through the array and calculate the average color of such a pixel block
    // create the graphical objects
    // calculate the target position of the objects
    // the target position passed in the constructor is the upper left corner position of the final picture (for now)

    float diameter = 0;
    f32 targetPosY = 0;
    unsigned int size = nodeInfo.size();
    for (unsigned int i=0; i < size; i ++)
    {
        curNode = nodeInfo[i];
        curNode->color[0] /= curNode->pixelInvolved;
        curNode->color[1] /= curNode->pixelInvolved;
        curNode->color[2] /= curNode->pixelInvolved;
        curNode->color[3] /= curNode->pixelInvolved;

        // create node and set properties
        if (type == Sphere)
        {
            curNode->node = smgr->addSphereSceneNode(dim, polyCount);
            // use vertex buffer
            ((IMeshSceneNode*)curNode->node)->getMesh()->setHardwareMappingHint(EHM_STATIC);
        }
        else
        {
            curNode->node = smgr->addBillboardSceneNode(0, dimension2d<f32>(dim,dim));
            curNode->node->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
            ITexture *tex = driver->getTexture("media/tessera.png");
            curNode->node->setMaterialTexture(0, tex);
        }
        curNode->node->setMaterialFlag(EMF_LIGHTING, false);

        if (!diameter)
        {
            if (type == Billboard)
            {
                // correct diameter because bounding box of billboard is not correct
                diameter = curNode->node->getTransformedBoundingBox().getExtent().X * 1.50f;
            }
            else
            {
                diameter = dim * 2;
            }
            // calculate top Y - because we start from the top when looping through the array
            targetPosY = targetPos.Y + (rows - 1) * diameter + (rows - 1) * gap;
        }

        // set target position - z position stays the same, x and y vary
        int col = i % columns;
        int row = i / columns;
        curNode->pos[0] = targetPos.X + diameter * col + col * gap;
        curNode->pos[1] = targetPosY  - diameter * row - row * gap;
        curNode->pos[2] = targetPos.Z;

        if (type == Sphere)
        {
            // set target color
            driver->getMeshManipulator()->setVertexColors(((IMeshSceneNode*)curNode->node)->getMesh(), SColor((u32)curNode->color[0], (u32)curNode->color[1], (u32)curNode->color[2], (u32)curNode->color[3]));
        }
        else
        {
            // set random color for a Billboard
            // set this as the current color
            curNode->curCol[0] = (f32)(rand() % 255);
            curNode->curCol[1] = (f32)(rand() % 255);
            curNode->curCol[2] = (f32)(rand() % 255);
            curNode->curCol[3] = (f32)(rand() % 255);
            ((IBillboardSceneNode*)curNode->node)->setColor(SColor((u32)curNode->curCol[0], (u32)curNode->curCol[1], (u32)curNode->curCol[2], (u32)curNode->curCol[3]));
        }

        // set random position
        curNode->node->setPosition(vector3df(startFrom.X + rand() % (int)abs(startFrom.X - startTo.X),
                                             startFrom.Y + rand() % (int)abs(startFrom.Y - startTo.Y),
                                             startFrom.Z + rand() % (int)abs(startFrom.Z - startTo.Z)));
    }
    status = Reset;
}

TesselatedImage::~TesselatedImage()
{
    unsigned int size = nodeInfo.size();
    for (unsigned int i=0; i < size; i++)
    {
        nodeInfo[i]->node->remove();
    }

    nodeInfo.clear();
}

void TesselatedImage::reset()
{
///    GraphicsIrr *graphics = GraphicsIrr::getInstance();

    // reset positions and colors of tesserae
    NodeInfo *curNode = 0;
    unsigned int size = nodeInfo.size();
    for (unsigned int i=0; i < size; i++)
    {
        curNode = nodeInfo[i];

        // set random position
        curNode->node->setPosition(vector3df(startFrom.X + rand() % (int)abs(startFrom.X - startTo.X),
                                             startFrom.Y + rand() % (int)abs(startFrom.Y - startTo.Y),
                                             startFrom.Z + rand() % (int)abs(startFrom.Z - startTo.Z)));

        if (type == Billboard)
        {
            // set random color
            // set this as the current color
            curNode->curCol[0] = (f32)(rand() % 255);
            curNode->curCol[1] = (f32)(rand() % 255);
            curNode->curCol[2] = (f32)(rand() % 255);
            curNode->curCol[3] = (f32)(rand() % 255);
            ((IBillboardSceneNode*)curNode->node)->setColor(SColor((u32)curNode->curCol[0], (u32)curNode->curCol[1], (u32)curNode->curCol[2], (u32)curNode->curCol[3]));
        }
    }
    status = Reset;
}

void TesselatedImage::startAnimation()
{
    status = Animating;
}

void TesselatedImage::move(double deltaTime)
{
    if (status != Animating)
        return;

    // remember if movement has been done
    bool changesDone = false;

    // go through each tessera and correct position and color (for billboards only) a bit
    NodeInfo *curNode = 0;
    unsigned int size = nodeInfo.size();
    f32 curPos[3];
    f32 newPos[3];
    vector3df curPosVec;
    float tolerance = speed * 0.1f;

    for (unsigned int i=0; i < size; i ++)
    {
        curNode = nodeInfo[i];

        // retrieve current position
        curPosVec = curNode->node->getPosition();
        curPos[0] = curPosVec.X;
        curPos[1] = curPosVec.Y;
        curPos[2] = curPosVec.Z;

        // correct position a bit
        for (int i=0; i < 3; i++)
        {
            if (curNode->pos[i] - curPos[i] > tolerance)
            {
                newPos[i] = curPos[i] + (f32)(speed * deltaTime);
                changesDone = true;
            }
            else
            if (curNode->pos[i] - curPos[i] < -tolerance)
            {
                newPos[i] = curPos[i] - (f32)(speed * deltaTime);
                changesDone = true;
            }
            else
            {
                newPos[i] = curNode->pos[i];
            }
        }
        curNode->node->setPosition(vector3df(newPos[0], newPos[1], newPos[2]));

        // adjust the color if it is a billboard
        // billboards have random colors at the beginning
        if (type == Billboard)
        {
            // correct color a bit
            for (int i=0; i < 4; i++)
            {
                if (abs((int)(curNode->color[i] - curNode->curCol[i])) <= colorChangesPerSecond)
                {
                    curNode->curCol[i] = (f32)curNode->color[i];
                }
                else
                if (curNode->color[i] > curNode->curCol[i])
                {
                    curNode->curCol[i] += (f32)(colorChangesPerSecond * deltaTime);
                    changesDone = true;
                }
                else
                if (curNode->color[i] < curNode->curCol[i])
                {
                    curNode->curCol[i] -= (f32)(colorChangesPerSecond * deltaTime);
                    changesDone = true;
                }
            }
            ((IBillboardSceneNode*)curNode->node)->setColor(SColor((u32)curNode->curCol[0], (u32)curNode->curCol[1], (u32)curNode->curCol[2], (u32)curNode->curCol[3]));
        }
    }

    if (!changesDone)
        status = Finished;
}

bool TesselatedImage::hasAnimationFinished()
{
    if (status == Finished)
        return true;
    else
        return false;
}
