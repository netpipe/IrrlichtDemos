#include <stdio.h>
#include "CBox2DManager.h"

CBox2DManager::CBox2DManager() {
}

CBox2DManager::~CBox2DManager() {
}

CBox2DManager *CBox2DManager::getSharedInstance() {
  /*static CBox2DManager sharedInstance;
  return &sharedInstance;*/
  return NULL;
}

void CBox2DManager::addBox2DStaticSceneNode(CBox2DSceneNode *pNode) {
}
