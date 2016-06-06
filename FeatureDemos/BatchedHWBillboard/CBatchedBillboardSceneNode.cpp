
#include "CBatchedBillboardSceneNode.h"
#include "ISceneManager.h"
#include "IVideoDriver.h"

namespace irr
{
namespace scene
{


CBatchedBillboardSceneNode::CBatchedBillboardSceneNode(ISceneNode* parent, ISceneManager* mgr,
        IBillboardSceneNode** nodes, u32 count, core::array<IEmptyBillboardSceneNode*> *impostorsOut,  s32 id,
		const core::vector3df& position)
		: IBatchedBillboardSceneNode(parent,mgr,nodes,count,impostorsOut,id,position), cosCutoff(0.95f)
{
    maxEdges[0].push_back(0.001f);
    maxEdges[1].push_back(0.001f);
    maxEdges[2].push_back(0.001f);
    minEdges[0].push_back(-0.001f);
    minEdges[1].push_back(-0.001f);
    minEdges[2].push_back(-0.001f);
    maxEdgeIndices[0] = maxEdgeIndices[1] = maxEdgeIndices[2] = 0;
    minEdgeIndices[0] = minEdgeIndices[1] = minEdgeIndices[2] = 0;
    if ((!nodes)||(!impostorsOut))
        return;
    for (u32 i=0; i<count; i++)
    {
        addBillboard(nodes[i]);
    }
    *impostorsOut = impostors;
}

CBatchedBillboardSceneNode::~CBatchedBillboardSceneNode()
{
    for (u32 i=0; i<impostors.size(); i++)
        impostors[i]->drop();
}

IEmptyBillboardSceneNode* CBatchedBillboardSceneNode::addBillboard(IBillboardSceneNode* node)
{
    if (!node)
        return 0;
    if (impostors.size()==0) {
        for (u32 i=0; i<node->getMaterialCount(); i++)
            billMaterial.push_back(node->getMaterial(i));
    }
    for (u32 j=0; j<billMaterial.size(); j++)
    {
        if (billMaterial[j]!=node->getMaterial(j))
            return 0;
    }
    node->setVisible(false);
    IEmptyBillboardSceneNode* impostor = new IEmptyBillboardSceneNode(this,node);
    impostors.push_back(impostor);
    billboards.push_back(node);
    indices.push_back(vertices.size());
    indices.push_back(vertices.size()+2);
    indices.push_back(vertices.size()+1);
    indices.push_back(vertices.size());
    indices.push_back(vertices.size()+3);
    indices.push_back(vertices.size()+2);

    video::SColor colorBottom(0xFFFFFF),colorTop(0xFFFFFF);
    node->getColor(colorTop,colorBottom);
	node->updateAbsolutePosition();
	core::vector3df pos = node->getAbsolutePosition();
	impostorTranslations.push_back(pos);
    vertices.push_back(video::S3DVertex(pos,core::vector3df(0.f,0.f,1.f),colorBottom,core::vector2df(1.f,1.f)));
    vertices.push_back(video::S3DVertex(pos,core::vector3df(0.f,0.f,1.f),colorTop,core::vector2df(1.f,0.f)));
    vertices.push_back(video::S3DVertex(pos,core::vector3df(0.f,0.f,1.f),colorTop,core::vector2df(0.f,0.f)));
    vertices.push_back(video::S3DVertex(pos,core::vector3df(0.f,0.f,1.f),colorBottom,core::vector2df(0.f,1.f)));
	core::vector3df scale = node->getScale();
	impostorScales.push_back(scale);
	core::dimension2df Size = node->getSize();
	impostorSizes.push_back(Size);

	core::aabbox3df box = node->getTransformedBoundingBox();
    maxEdges[0].push_back(box.MaxEdge.X);
    if (box.MaxEdge.X>maxEdges[0][maxEdgeIndices[0]])
    {
        maxEdgeIndices[0] = maxEdges[0].size()-1;
    }
    maxEdges[1].push_back(box.MaxEdge.Y);
    if (box.MaxEdge.Y>maxEdges[1][maxEdgeIndices[1]])
    {
        maxEdgeIndices[1] = maxEdges[1].size()-1;
    }
    maxEdges[2].push_back(box.MaxEdge.Z);
    if (box.MaxEdge.X>maxEdges[2][maxEdgeIndices[2]])
    {
        maxEdgeIndices[2] = maxEdges[2].size()-1;
    }
    minEdges[0].push_back(box.MinEdge.X);
    if (box.MinEdge.X<minEdges[0][minEdgeIndices[0]])
    {
        minEdgeIndices[0] = minEdges[0].size()-1;
    }
    minEdges[1].push_back(box.MinEdge.Y);
    if (box.MinEdge.Y<minEdges[1][minEdgeIndices[1]])
    {
        minEdgeIndices[1] = minEdges[1].size()-1;
    }
    minEdges[2].push_back(box.MinEdge.Z);
    if (box.MinEdge.X<minEdges[2][minEdgeIndices[2]])
    {
        minEdgeIndices[2] = minEdges[2].size()-1;
    }
}

void CBatchedBillboardSceneNode::removeBillboard(IEmptyBillboardSceneNode* node) {
    s32 index = impostors.binary_search(node);
    if (index==-1)
        return;
    impostors[index]->drop();
    impostors.erase(index);
    impostorTranslations.erase(index);
    impostorScales.erase(index);
    impostorSizes.erase(index);
    billboards.erase(index);
    index++;
    for (u32 i=0; i<3; i++)
    {
        minEdges[i].erase(index);
        maxEdges[i].erase(index);
        if (minEdgeIndices[i]==index)
        {
            minEdgeIndices[i]=0;
            for (u32 j=1; j<minEdges[i].size(); j++)
            {
                if (minEdges[i][j]<minEdges[i][minEdgeIndices[i]])
                    minEdgeIndices[i]=j;
            }
        }
        if (maxEdgeIndices[i]==index)
        {
            maxEdgeIndices[i]=0;
            for (u32 j=1; j<maxEdges[i].size(); j++)
            {
                if (maxEdges[i][j]<maxEdges[i][maxEdgeIndices[i]])
                    maxEdgeIndices[i]=j;
            }
        }
    }
}
void CBatchedBillboardSceneNode::removeBillboard(IEmptyBillboardSceneNode* node, bool noDrop) {
    s32 index = impostors.binary_search(node);
    if (index==-1)
        return;
    if (!noDrop)
        impostors[index]->drop();
    impostors.erase(index);
    impostorTranslations.erase(index);
    impostorScales.erase(index);
    impostorSizes.erase(index);
    billboards.erase(index);
    index++;
    for (u32 i=0; i<3; i++)
    {
        minEdges[i].erase(index);
        maxEdges[i].erase(index);
        if (minEdgeIndices[i]==index)
        {
            minEdgeIndices[i]=0;
            for (u32 j=1; j<minEdges[i].size(); j++)
            {
                if (minEdges[i][j]<minEdges[i][minEdgeIndices[i]])
                    minEdgeIndices[i]=j;
            }
        }
        if (maxEdgeIndices[i]==index)
        {
            maxEdgeIndices[i]=0;
            for (u32 j=1; j<maxEdges[i].size(); j++)
            {
                if (maxEdges[i][j]<maxEdges[i][maxEdgeIndices[i]])
                    maxEdgeIndices[i]=j;
            }
        }
    }
}
void CBatchedBillboardSceneNode::removeBillboard(IBillboardSceneNode* node) {
    s32 index = billboards.binary_search(node);
    if (index==-1)
        return;
    impostors[index]->drop();
    impostors.erase(index);
    impostorTranslations.erase(index);
    impostorScales.erase(index);
    impostorSizes.erase(index);
    billboards.erase(index);
    index++;
    for (u32 i=0; i<3; i++)
    {
        minEdges[i].erase(index);
        maxEdges[i].erase(index);
        if (minEdgeIndices[i]==index)
        {
            minEdgeIndices[i]=0;
            for (u32 j=1; j<minEdges[i].size(); j++)
            {
                if (minEdges[i][j]<minEdges[i][minEdgeIndices[i]])
                    minEdgeIndices[i]=j;
            }
        }
        if (maxEdgeIndices[i]==index)
        {
            maxEdgeIndices[i]=0;
            for (u32 j=1; j<maxEdges[i].size(); j++)
            {
                if (maxEdges[i][j]>maxEdges[i][maxEdgeIndices[i]])
                    maxEdgeIndices[i]=j;
            }
        }
    }
}

u32 CBatchedBillboardSceneNode::getCount()
{
    return impostors.size();
}

void CBatchedBillboardSceneNode::updateBillboardSize(const IEmptyBillboardSceneNode* const node, const core::dimension2d<f32>& size) const
{
    printf("G\n");
}

void CBatchedBillboardSceneNode::updateBillboardColor(IEmptyBillboardSceneNode* node, const video::SColor & overallColor)
{
    printf("A\n");
}

void CBatchedBillboardSceneNode::updateBillboardColor(IEmptyBillboardSceneNode* node, const video::SColor & topColor, const video::SColor & bottomColor)
{
    printf("Y\n");
}

void CBatchedBillboardSceneNode::updateBillboardScale(IEmptyBillboardSceneNode* node,const core::vector3df& scale)
{
    printf("F\n");
}

void CBatchedBillboardSceneNode::updateBillboardRotation(IEmptyBillboardSceneNode* node,const core::vector3df& rotation)
{
    printf("A\n");
}

void CBatchedBillboardSceneNode::updateBillboardPosition(IEmptyBillboardSceneNode* node,const core::vector3df& newpos)
{
    /*s32 index = impostors.binary_search(node);
    printf("A\n");
    if (index==-1)
        return;
    printf("Y\n");

    vertices[index*4].Pos += newpos;
    vertices[index*4+1].Pos += newpos;
    vertices[index*4+2].Pos += newpos;
    vertices[index*4+3].Pos += newpos;

    index++;/*
    maxEdges[0][index]+=newpos.X;
    if (index==maxEdgeIndices[0]&&newpos.X>0.f)
    {
        maxEdgeIndices[0]=0;
        for (u32 j=1; j<maxEdges[0].size(); j++)
        {
            if (maxEdges[0][j]>maxEdges[0][maxEdgeIndices[0]])
                maxEdgeIndices[0]=j;
        }
    }
    else if (maxEdges[0][index]>maxEdges[0][maxEdgeIndices[0]]) {
        maxEdgeIndices[0]=index;
    }
    maxEdges[1][index]+=newpos.Y;
    if (index==maxEdgeIndices[1]&&newpos.Y>0.f)
    {
        maxEdgeIndices[1]=0;
        for (u32 j=1; j<maxEdges[1].size(); j++)
        {
            if (maxEdges[1][j]>maxEdges[1][maxEdgeIndices[1]])
                maxEdgeIndices[1]=j;
        }
    }
    else if (maxEdges[1][index]>maxEdges[1][maxEdgeIndices[1]]) {
        maxEdgeIndices[1]=index;
    }
    maxEdges[2][index]+=newpos.Z;
    if (index==maxEdgeIndices[2]&&newpos.Z>0.f)
    {
        maxEdgeIndices[2]=0;
        for (u32 j=1; j<maxEdges[2].size(); j++)
        {
            if (maxEdges[2][j]>maxEdges[2][maxEdgeIndices[2]])
                maxEdgeIndices[2]=j;
        }
    }
    else if (maxEdges[2][index]>maxEdges[2][maxEdgeIndices[2]]) {
        maxEdgeIndices[2]=index;
    }
    minEdges[0][index]+=newpos.X;
    if (index==minEdgeIndices[0]&&newpos.X>0.f)
    {
        minEdgeIndices[0]=0;
        for (u32 j=1; j<minEdges[0].size(); j++)
        {
            if (minEdges[0][j]<minEdges[0][minEdgeIndices[0]])
                minEdgeIndices[0]=j;
        }
    }
    else if (minEdges[0][index]<minEdges[0][minEdgeIndices[0]]) {
        minEdgeIndices[0]=index;
    }
    minEdges[1][index]+=newpos.Y;
    if (index==minEdgeIndices[1]&&newpos.Y>0.f)
    {
        minEdgeIndices[1]=0;
        for (u32 j=1; j<minEdges[1].size(); j++)
        {
            if (minEdges[1][j]<minEdges[1][minEdgeIndices[1]])
                minEdgeIndices[1]=j;
        }
    }
    else if (minEdges[1][index]<minEdges[1][minEdgeIndices[1]]) {
        minEdgeIndices[1]=index;
    }
    minEdges[2][index]+=newpos.Z;
    if (index==minEdgeIndices[2]&&newpos.Z<0.f)
    {
        minEdgeIndices[2]=0;
        for (u32 j=1; j<minEdges[2].size(); j++)
        {
            if (minEdges[2][j]<minEdges[2][minEdgeIndices[2]])
                minEdgeIndices[2]=j;
        }
    }
    else if (minEdges[2][index]<minEdges[2][minEdgeIndices[2]]) {
        minEdgeIndices[2]=index;
    }*/
}

void CBatchedBillboardSceneNode::updateBillboardAbsoluteTransformation(IEmptyBillboardSceneNode* node,core::matrix4 AbsoluteTransformation)
{
    //! I dont want it to update every frame
}

video::SMaterial& CBatchedBillboardSceneNode::getMaterial(u32 num) {
    if (num<billMaterial.size())
        return video::IdentityMaterial;

    return billMaterial[num];
}

u32 CBatchedBillboardSceneNode::getMaterialCount() const {
    return billMaterial.size();
}

//! pre render event
void CBatchedBillboardSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this,ESNRP_TRANSPARENT);

	ISceneNode::OnRegisterSceneNode();
}

void CBatchedBillboardSceneNode::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();
	ICameraSceneNode* camera = SceneManager->getActiveCamera();

	if (!camera || !driver)
		return;

	core::vector3df view = camera->getTarget() - camera->getAbsolutePosition();
	view.normalize();
    if (lastCamView.dotProduct(view)<cosCutoff)
    {
        lastCamView=view;
        // make billboard look to camera
        core::vector3df up = camera->getUpVector();
        core::vector3df horizontal = up.crossProduct(view);
        if ( horizontal.getLength() == 0 )
        {
            horizontal.set(up.Y,up.X,up.Z);
        }
        horizontal.normalize();
        horizontal *= 0.5f;
        core::vector3df vertical = horizontal.crossProduct(view);
        vertical.normalize();
        vertical *= 0.5f;
        if (PassingNormals)
        {
            view *= -1.0f;
            for (u32 i=0; i<impostors.size(); i++)
            {
                for (s32 j=0; j<4; ++j)
                    vertices[i*4+j].Normal = view;
                core::vector3df pos = impostorTranslations[i];
                core::vector3df scale = impostorScales[i];
                core::vector3df hoz2 = horizontal*scale.X*impostorSizes[i].Width;
                core::vector3df ver2 = vertical*scale.Y*impostorSizes[i].Height;
                vertices[i*4].Pos = pos + hoz2 + ver2;
                vertices[1+i*4].Pos = pos + hoz2 - ver2;
                vertices[2+i*4].Pos = pos - hoz2 - ver2;
                vertices[3+i*4].Pos = pos - hoz2 + ver2;
            }
        }
        else
        {
            for (u32 i=0; i<impostors.size(); i++)
            {
                core::vector3df pos = impostorTranslations[i];
                core::vector3df scale = impostorScales[i];
                core::vector3df hoz2 = horizontal*scale.X*impostorSizes[i].Width;
                core::vector3df ver2 = vertical*scale.Y*impostorSizes[i].Height;
                vertices[i*4].Pos = pos + hoz2 + ver2;
                vertices[1+i*4].Pos = pos + hoz2 - ver2;
                vertices[2+i*4].Pos = pos - hoz2 - ver2;
                vertices[3+i*4].Pos = pos - hoz2 + ver2;
            }
        }
    }

	// draw

	if ( DebugDataVisible & scene::EDS_BBOX )
	{
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
		video::SMaterial m;
		m.Lighting = false;
		driver->setMaterial(m);
		driver->draw3DBox(getBoundingBox(), video::SColor(0,208,195,152));
	}

	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

    for (u32 i=0; i< billMaterial.size(); i++) {
        driver->setMaterial(billMaterial[0]);
        driver->drawIndexedTriangleList(vertices.pointer(),vertices.size(),indices.pointer(),vertices.size()/2);
    }
}

bool CBatchedBillboardSceneNode::isPassingNormals() const
{
    return PassingNormals;
}
void CBatchedBillboardSceneNode::setPassingNormals(bool passing) {
    PassingNormals = passing;
}

f32 CBatchedBillboardSceneNode::getLoDAngle() const
{
    return cosCutoff;
}
void CBatchedBillboardSceneNode::setLoDAngle(f32 angle)
{
    cosCutoff=cosf(angle*core::DEGTORAD);
}

}
}
