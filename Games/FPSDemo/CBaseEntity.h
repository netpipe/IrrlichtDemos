#ifndef _CBASEENTITY_H_
#define _CBASEENTITY_H_

#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include "CEntityTypes.h"
#include "EParameterLayout.h"

using namespace irr;

namespace agEngine
{
    namespace entities
    {
        class CEntityManager;

        class CBaseEntity : public virtual IReferenceCounted
        {
        protected:
            core::aabbox3d<f32> Box;
            video::SMaterial Material;
            core::array<core::stringc > stringParameters;
            core::array<s32> intParameters;
            core::array<f32> floatParameters;
            core::array<bool> boolParameters;
            scene::ISceneNode* associatedSceneNode;
            btRigidBody* rigidBody;

            bool needsUpdate;
        public:
            CBaseEntity(scene::ISceneNode* node = 0)
            : associatedSceneNode(node), needsUpdate(false)
            {
                if (node)
                    node->grab();
            }

            virtual void update(CEntityManager* emgr)
            {
                if (associatedSceneNode)
                {
                    if (boolParameters[isAnimated])
                    {
                        scene::IAnimatedMeshSceneNode* temp = (scene::IAnimatedMeshSceneNode*)associatedSceneNode;
                        floatParameters[currentAnimationFrame] = temp->getFrameNr();
                    }

                    if (needsUpdate)
                    {
                        associatedSceneNode->setPosition(core::vector3df(floatParameters[xPosition], floatParameters[yPosition], floatParameters[zPosition]));
                        associatedSceneNode->setRotation(core::vector3df(floatParameters[xRotation], floatParameters[yRotation], floatParameters[zRotation]));
                    }
                }
            }

            virtual bool parseEvent(const SEvent& event)
            {
                // Parse input events
                return false;
            }

            virtual ENTITY_TYPE getEType() { return E_BASE_ENTITY; };
            virtual void associateWithSceneNode(scene::ISceneNode* newSceneNode) { associatedSceneNode = newSceneNode; }
            virtual scene::ISceneNode* getAssociatedSceneNode() { return associatedSceneNode; }
            virtual void parseEntityAttributes(io::IXMLReader* reader) {};

            virtual const core::aabbox3d<f32>& getBoundingBox() const
            {
                return Box;
            }

            virtual u32 getMaterialcount() const
            {
                return 1;
            }

            virtual video::SMaterial& getMaterial(u32 i)
            {
                return Material;
            }


            // Allocator functions
            virtual void allocateStringParams(u32 i)
            {
                stringParameters.reallocate(i);
                for (u32 ii = 0; ii < i; ++ ii)
                    stringParameters.push_back("");
            }

            virtual void allocateIntParams(u32 i)
            {
                intParameters.reallocate(i);
                for (u32 ii = 0; ii < i; ++ ii)
                    intParameters.push_back(0);
            }

            virtual void allocateFloatParams(u32 i)
            {
                floatParameters.reallocate(i);
                for (u32 ii = 0; ii < i; ++ ii)
                    floatParameters.push_back(0.0f);
            }

            // Getter member functions
            virtual const core::stringc& getStringParam(u32 i) const
            {
                return stringParameters[i];
            }

            virtual u32 getIntParam(u32 i) const
            {
                return intParameters[i];
            }

            virtual f32 getFloatParam(u32 i) const
            {
                return floatParameters[i];
            }

            virtual u32 getStringParamCount() const
            {
                return stringParameters.size();
            }

            virtual u32 getIntParamCount() const
            {
                return intParameters.size();
            }

            virtual u32 getFloatParamCount() const
            {
                return floatParameters.size();
            }

            virtual btRigidBody* getPhysicsRigidBody()
            {
                return rigidBody;
            }

            // Setter member functions
            virtual void setStringParam(u32 i, const core::stringw& value)
            {
                stringParameters[i] = value;
            }

            virtual void setIntParam(u32 i, s32 value)
            {
                intParameters[i] = value;
            }

            virtual void setFloatParam(u32 i, f32 value)
            {
                floatParameters[i] = value;
            }

            virtual void setPhysicsRigidBody(btRigidBody* body)
            {
                rigidBody = body;
            }

            // Modder member functions
            virtual void modIntParam(u32 i, s32 modValue)
            {
                intParameters[i] += modValue;
            }

            virtual void modFloatParam(u32 i, f32 modValue)
            {
                floatParameters[i] += modValue;
            }

            // save and load parameters
            virtual void serializeParameters(io::IAttributes* out)
            {
                out->addInt("sParmCount", stringParameters.size());
                out->addInt("iParmCount", intParameters.size());
                out->addInt("fParamCount", floatParameters.size());
                out->addInt("bParamCount", boolParameters.size());

                u32 i;

                u32 sceneNodeId = 0;
                if (associatedSceneNode)
                    sceneNodeId = associatedSceneNode->getID();

                out->addInt("AssociatedNode", sceneNodeId);

                if (associatedSceneNode)
                    out->addBool("AssociatedNodeVisible", associatedSceneNode->isVisible());
                else
                    out->addBool("AssociatedNodeVisible", false);

                for (i = 0; i < stringParameters.size(); ++i)
                    out->addString("sParm", stringParameters[i].c_str());

                for (i = 0; i < intParameters.size(); ++i)
                    out->addInt("iParm", intParameters[i]);

                for (i = 0; i < floatParameters.size(); ++i)
                    out->addFloat("fParm", floatParameters[i]);

                for (i = 0; i < boolParameters.size(); ++i)
                    out->addBool("bParm", boolParameters[i]);
            }

            virtual void deserializeParameters(io::IAttributes* in, scene::ISceneManager* smgr)
            {
                u32 sParmCount, iParmCount, fParmCount, bParmCount;

                sParmCount = in->getAttributeAsInt("sParmCount");
                iParmCount = in->getAttributeAsInt("iParmCount");
                fParmCount = in->getAttributeAsInt("fParmCount");
                bParmCount = in->getAttributeAsInt("bParmCount");

                u32 sceneNodeId = in->getAttributeAsInt("AssociatedNode");
                bool sceneNodeVisible = in->getAttributeAsBool("AssociatedNodeVisible");

                const u32 parameterOffset = 5;

                u32 i;

                for (i = parameterOffset; i < (sParmCount + parameterOffset); ++i)
                    stringParameters.push_back(in->getAttributeAsStringW(i));

                for (i = (parameterOffset + sParmCount); i < (sParmCount + iParmCount + parameterOffset); ++i)
                    intParameters.push_back(in->getAttributeAsInt(i));

                for (i = (parameterOffset + sParmCount + iParmCount); i < (sParmCount + iParmCount + fParmCount + parameterOffset); ++i)
                    floatParameters.push_back(in->getAttributeAsFloat(i));

                for (i = (parameterOffset + sParmCount + iParmCount + fParmCount); i < (sParmCount + iParmCount + fParmCount + bParmCount + parameterOffset); ++i)
                    boolParameters.push_back(in->getAttributeAsBool(i));

                if (stringParameters[modelFile] != "")
                {
                    // Is Animated?
                    if (boolParameters[isAnimated])
                    {
                        scene::IAnimatedMesh* mesh = smgr->getMesh(stringParameters[modelFile].c_str());
                        associatedSceneNode = smgr->addAnimatedMeshSceneNode(mesh, smgr->getRootSceneNode(), sceneNodeId);
                        associatedSceneNode->setVisible(sceneNodeVisible);
                        scene::IAnimatedMeshSceneNode* temp = (scene::IAnimatedMeshSceneNode*)associatedSceneNode;
                        temp->setCurrentFrame(floatParameters[currentAnimationFrame]);
                        mesh->drop();
                    }
                    else
                    {
                        scene::IMesh* mesh = smgr->getMesh(stringParameters[modelFile].c_str());
                        associatedSceneNode = smgr->addMeshSceneNode(mesh, smgr->getRootSceneNode(), sceneNodeId);
                        associatedSceneNode->setVisible(sceneNodeVisible);
                        mesh->drop();
                    }
                    associatedSceneNode->setPosition(core::vector3df(floatParameters[xPosition], floatParameters[yPosition], floatParameters[zPosition]));
                    associatedSceneNode->setRotation(core::vector3df(floatParameters[xRotation], floatParameters[yRotation], floatParameters[zRotation]));
                }
            }

            // Convenience Functions
            void setPosition(f32 x, f32 y, f32 z)
            {
                floatParameters[xPosition] = x;
                floatParameters[yPosition] = y;
                floatParameters[zPosition] = z;

                if (associatedSceneNode)
                    associatedSceneNode->setPosition(core::vector3d<f32>(x, y, z));
            }

            void setRotation(f32 x, f32 y, f32 z)
            {
                floatParameters[xRotation] = x;
                floatParameters[yRotation] = y;
                floatParameters[zRotation] = z;

                if (associatedSceneNode)
                    associatedSceneNode->setRotation(core::vector3d<f32>(x, y, z));
            }

            core::vector3d<f32> getPosition() const
            {
                return core::vector3d<f32>(floatParameters[xPosition], floatParameters[yPosition], floatParameters[zPosition]);
            }

            core::vector3d<f32> getRotation() const
            {
                return core::vector3d<f32>(floatParameters[xRotation], floatParameters[yRotation], floatParameters[zRotation]);
            }

            core::vector3d<f32> getTarget() const
            {
                core::vector3d<f32> temp(0, 0, -1);
                temp.rotateXYBy(floatParameters[zRotation]);
                temp.rotateXZBy(floatParameters[yRotation]);
                temp.rotateYZBy(floatParameters[xRotation]);
                return temp;
            }
        };
    }
}

#endif
