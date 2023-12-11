#include "desteer/controller/FluentSimpleController.hpp"

using namespace desteer;
using namespace controller;
using namespace behavior;

FluentSimpleController * FluentSimpleController::WithSeek(){
    _seekBehavior = new SeekBehavior(_mob);
    SetBehaviorFlag(EBF_SEEK,true);
    return this;
};
FluentSimpleController * FluentSimpleController::WithSeek(irr::core::vector3df target){
    SetSeekTarget(target);
    return this->WithSeek();
};

FluentSimpleController * FluentSimpleController::WithArrive(){
    SetBehaviorFlag(EBF_ARRIVE,true);
    return this;
};
FluentSimpleController * FluentSimpleController::WithArrive(irr::core::vector3df target){
    SetArriveTarget(target);
    return this->WithArrive();
};

FluentSimpleController * FluentSimpleController::WithHide(){
    SetBehaviorFlag(EBF_HIDE,true);
    return this;
};
FluentSimpleController * FluentSimpleController::WithHide(entity::IMobileEntity *target){
    SetHideTarget(target);
    return this->WithArrive();
};

FluentSimpleController * FluentSimpleController::WithPursuit(){
    SetBehaviorFlag(EBF_PURSUIT,true);
    return this;
}
FluentSimpleController * FluentSimpleController::WithPursuit(entity::IMobileEntity * target){
    SetPursuitTarget(target);
    return this->WithPursuit();
}
FluentSimpleController * FluentSimpleController::WithObstacles(EntityGroup &obstacles){
    SetObstacles(obstacles);
    return this;
}
