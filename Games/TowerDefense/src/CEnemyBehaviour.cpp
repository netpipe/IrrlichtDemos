#include "CEnemyBehaviour.h"
#include "CTile.h"
#include "CMap.h"
#include "PathNode.h"
#include "CEntityManager.h"
#include "CStateManager.h"


CEnemyBehaviour::CEnemyBehaviour(CEntity* entity, CEntityManager* manager, CMap* map) : CComponent(entity, manager, ECT_LOGIC, "EnemyLogic"), Position(entity->getPropertyValue<irr::core::vector3df>("Position")), TargetPosition(entity->getPropertyValue<irr::core::vector3df>("TargetPosition"))
{
    Map = map;
    c = true;
}

CEnemyBehaviour::~CEnemyBehaviour()
{
}

void CEnemyBehaviour::update(irr::f32 timeDiff)
{
    if (c)
        calcPath();

    if (Path.getSize() > 0)
    {
        if (Map->getTile(Position) == *Path.begin())
        {
            irr::core::list<CTile*>::Iterator it = Path.begin();
            Path.erase(it);
            if (Path.getSize() > 0)
                TargetPosition = (*Path.begin())->getPos();
            else
            {
                getPropertyValue<bool>("Alive") = false;
                Manager->getManager()->FireEvent("EnemyWin", 1, getOwner());
                getOwner()->remove();
            }
        }
    }
}

int G = 0;
void CEnemyBehaviour::calcPath(void)
{
    c = false;

    CTile* target = Map->getGoal();
    CTile* current = Map->getTile(Position);

    PathNode::ClosedList.clear();
    PathNode::Goals.clear();
    Path.clear();

    PathNode* root = new PathNode(current, target, NULL, 0);
    root->addChildren();

    printf("found Path[%i]\n", PathNode::Goals.size());

    if (PathNode::Goals.size() > 0)
    {
        if (G >= PathNode::Goals.size())
            G = 0;
        PathNode::Goals[G]->getPath(Path);
        G++;
    }

    delete root;

}
