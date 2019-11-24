#ifndef _CBASESCRIPTEDENTITY_H_
#define _CBASESCRIPTEDENTITY_H_

#include "CBaseEntity.h"
#include "CScriptManager.h"

namespace agEngine
{
    namespace entities
    {
        class CBaseScriptedEntity : public CBaseEntity
        {
        public:
            CBaseScriptedEntity(script::CScriptManager* scriptMgr)
            : CBaseEntity(0), scMgr(scriptMgr)
            {
                // Push back the first parameter (which is the node name)
                stringParameters.push_back("BaseEntity");
                // Push back the scripting filename
                stringParameters.push_back("null");

                if (scMgr)
                    scMgr->grab();
            }

            virtual ~CBaseScriptedEntity()
            {
                if (scMgr)
                    scMgr->drop();
            }

            virtual void setScript(const core::stringc& scriptName, const core::stringc& objExpression)
            {
                stringParameters[1] = scriptName;
                if (scMgr)
                {
                    // Get the scripting manager to load the script and return it's contents
                    core::stringc finalScript = scMgr->loadScript(scriptName);

                    // Do a search and replace of the objExpression provided with the node name of this entity
                    s32 offset = finalScript.find(objExpression.c_str());
                    core::stringc rebuiltScript;

                    while (offset >= 0)
                    {
                        rebuiltScript = stringParameters[nodeName] +
                            finalScript.subString(offset + objExpression.size(),
                            finalScript.size() - offset + objExpression.size());

                        finalScript = rebuiltScript;
                        offset = finalScript.find(objExpression.c_str());
                    }

                    // Compile the resultant script
                    scMgr->compileScript(finalScript);
                }
            }

            virtual void setName(core::stringc& nodeName)
            {
                stringParameters[0] = nodeName;
            }

            virtual core::stringc getScript() const
            {
                return stringParameters[1];
            }

            virtual core::stringc getName() const
            {
                return stringParameters[0];
            }

        protected:
            script::CScriptManager* scMgr;
        };
    }
}

#endif
