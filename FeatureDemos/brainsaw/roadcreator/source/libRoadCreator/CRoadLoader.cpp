  #include <CRoadLoader.h>
  #include <SAnimatedMesh.h>

  #include <CSegment.h>
  #include <CSurface.h>
  #include <CConnection.h>
  #include <CTextureParameters.h>

using namespace irr;

bool CRoadLoader::sameMaterial(const video::SMaterial &m1, const video::SMaterial &m2) {
  #ifdef _IRREDIT_PLUGIN
    core::stringc s1=m1.getTexture(0)->getName(),
                  s2=m2.getTexture(0)->getName();
  #else
    core::stringc s1=m1.getTexture(0)->getName().getPath(),
                  s2=m2.getTexture(0)->getName().getPath();
  #endif

  return s1==s2;
}

void CRoadLoader::addBufferToArray(scene::IMeshBuffer *p, core::array<scene::IMeshBuffer *> &aBuffers) {
  bool bAdded=false;

  for (u32 j=0; j<aBuffers.size() && !bAdded; j++) {
    video::SMaterial mat1=p->getMaterial(),
                     mat2=aBuffers[j]->getMaterial();

    if (sameMaterial(mat1,mat2)) {
      scene::IMeshBuffer *pBuffer=aBuffers[j];
      pBuffer->append(p->getVertices(),p->getVertexCount(),p->getIndices(),p->getIndexCount());
      pBuffer->recalculateBoundingBox();
      p->drop();
      bAdded=true;
    }
  }

  if (!bAdded) {
    p->getMaterial().Lighting=false;
    p->recalculateBoundingBox();
    aBuffers.push_back(p);
  }
}

CRoadLoader::CRoadLoader(IrrlichtDevice *pDevice) {
  m_pDevice=pDevice;
  m_sCurrentRoad="";
  m_pSurface=NULL;
  m_vOfffset=core::vector3df(0.0f,0.0f,0.0f);
  m_bShrinkNode=false;
}

bool CRoadLoader::loadRoad(const core::stringc sName) {
  m_lConnections.clear();
  m_lSegments.clear();

  m_sCurrentRoad=sName;

  bool bRet=true;
  io::IReadFile *pFile=m_pDevice->getFileSystem()->createAndOpenFile(sName.c_str());

  if (pFile) {
    io::IXMLReader *pReader=m_pDevice->getFileSystem()->createXMLReader(pFile);
    if (pReader) {
      u32 iState=0,
          iSegTex=0,
          iConTex=0,
          iGndTex=0;

      CSegment    *pSeg=NULL;
      CConnection *pCon=NULL;

      while (pReader->read()) {
        if (pReader->getNodeType()==io::EXN_ELEMENT) {
          if (core::stringw(pReader->getNodeName())==L"Segment"   ) iState=1;
          if (core::stringw(pReader->getNodeName())==L"Connection") iState=2;
          if (core::stringw(pReader->getNodeName())==L"Materials" ) iState=3;
          if (core::stringw(pReader->getNodeName())==L"Surface"   ) iState=6;
          if (core::stringw(pReader->getNodeName())==L"Parameters") iState=9;

          if (iState==1 && core::stringw(pReader->getNodeName())==L"TextureParams") iState=4;
          if (iState==2 && core::stringw(pReader->getNodeName())==L"TextureParams") iState=5;
          if (iState==6 && core::stringw(pReader->getNodeName())==L"TextureParams") iState=7;

          if (core::stringw(pReader->getNodeName())==L"attributes") {
            //Load a segment
            if (iState==1) {
              io::IAttributes *pAttr=m_pDevice->getFileSystem()->createEmptyAttributes();
              pAttr->read(pReader,true);
              pSeg=new CSegment(m_pDevice);
              m_lSegments.push_back(pSeg);
              pSeg->load(pAttr);
              pAttr->drop();
              iSegTex=0;
            }

            //Load a connection
            if (iState==2) {
              io::IAttributes *pAttr=m_pDevice->getFileSystem()->createEmptyAttributes();
              pAttr->read(pReader,true);
              pCon=new CConnection(m_pDevice,NULL);
              iConTex=0;
              pCon->load(pAttr);

              core::list<CSegment *>::Iterator it;
              for (it=m_lSegments.begin(); it!=m_lSegments.end(); it++) {
                CSegment *p=*it;
                if (p->getName()==pCon->getSegment1Name()) pCon->setSegment1NoInit(p);
                if (p->getName()==pCon->getSegment2Name()) pCon->setSegment2NoInit(p);
              }

              m_lConnections.push_back(pCon);
              pAttr->drop();
            }

            //Load texture parameters of a segment
            if (iState==4) {
              if (pSeg!=NULL && iSegTex<_SEGMENT_NUMBER_OF_BUFFERS) {
                io::IAttributes *pAttr=m_pDevice->getFileSystem()->createEmptyAttributes();
                pAttr->read(pReader,true);
                pSeg->getTextureParameters(iSegTex)->load(pAttr);
                pSeg->update();
                pAttr->drop();
              }
            }

            //Load texture parameters of a connection
            if (iState==5) {
              if (pCon!=NULL && iConTex<_CONNECTION_NUMBER_OF_BUFFERS) {
                io::IAttributes *pAttr=m_pDevice->getFileSystem()->createEmptyAttributes();
                pAttr->read(pReader,true);
                pCon->getTextureParameters(iConTex)->load(pAttr);
                pCon->update();
                pAttr->drop();
              }
            }

            //Load surface data
            if (iState==6) {
              io::IAttributes *pAttr=m_pDevice->getFileSystem()->createEmptyAttributes();
              pAttr->read(pReader,true);
              if (m_pSurface==NULL) m_pSurface=new CSurface(m_pDevice,NULL);
              m_pSurface->load(pAttr);
              m_pSurface->setVisible(true);
              pAttr->drop();
            }

            //Load texture parameters of the surface
            if (iState==7) {
              io::IAttributes *pAttr=m_pDevice->getFileSystem()->createEmptyAttributes();
              pAttr->read(pReader,true);
              m_pSurface->getTextureParameters(iGndTex)->load(pAttr);
              m_pSurface->recalcMeshBuffer();
              pAttr->drop();
            }

            if (iState==9) {
              io::IAttributes *pAttr=m_pDevice->getFileSystem()->createEmptyAttributes();
              pAttr->read(pReader,true);
              m_bShrinkNode=pAttr->getAttributeAsBool("shrinknode");
              pAttr->drop();
            }
          }
        }

        if (pReader->getNodeType()==io::EXN_ELEMENT_END) {
          switch (iState) {
            case 1:
              if (core::stringw(pReader->getNodeName())==L"Segment") {
                iState=0;
                pSeg=NULL;
                iSegTex=0;
              }
              break;

            case 2:
              if (core::stringw(pReader->getNodeName())==L"Connection") {
                iState=0;
                pCon=NULL;
                iConTex=0;
              }
              break;

            case 3:
              if (core::stringw(pReader->getNodeName())==L"Materials") iState=0;
              break;

            case 4:
              if (core::stringw(pReader->getNodeName())==L"TextureParams") {
                iState=1;
                iSegTex++;
              }
              break;

            case 5:
              if (core::stringw(pReader->getNodeName())==L"TextureParams") {
                iState=2;
                iConTex++;
              }
              break;

            case 6:
              if (core::stringw(pReader->getNodeName())==L"Surface") {
                iState=0;
                m_pSurface->recalcMeshBuffer();
              }
              break;

            case 7:
              if (core::stringw(pReader->getNodeName())==L"TextureParams") {
                iState=6;
                iGndTex++;
              }
              break;

            case 9:
              if (core::stringw(pReader->getNodeName())==L"Parameters") {
                iState=0;
              }
          }
        }
      }

      m_sCurrentRoad=sName;
      pReader->drop();

      core::list<CSegment    *>::Iterator sit;
      core::list<CConnection *>::Iterator cit;

      for (sit=m_lSegments   .begin(); sit!=m_lSegments   .end(); sit++) (*sit)->update();
      for (cit=m_lConnections.begin(); cit!=m_lConnections.end(); cit++) (*cit)->update();
    }
    else {
      printf("ERROR: can't create XML reader for file \"%s\".\n",sName.c_str());
      bRet=false;
    }
    pFile->drop();
  }
  else {
    printf("ERROR: can't open file \"%s\" for reading.\n",sName.c_str());
    bRet=false;
  }
  return bRet;
}

/**
 * This method saves a road to a file
 * @param sName the file to save the road to
 * @see CSegment::save
 * @see CConnection::save
 * @see CTextureParameters::save
 */
void CRoadLoader::saveRoad() {
  io::IWriteFile *pFile=m_pDevice->getFileSystem()->createAndWriteFile(m_sCurrentRoad.c_str(),false);
  if (pFile) {
    io::IXMLWriter *pWriter=m_pDevice->getFileSystem()->createXMLWriter(pFile);
    if (pWriter) {
      pWriter->writeXMLHeader();
      pWriter->writeElement(L"BulletByteRoadCreator",false);
      pWriter->writeLineBreak();

      pWriter->writeElement(L"Parameters",false);
      pWriter->writeLineBreak();

      io::IAttributes *pAttr=m_pDevice->getFileSystem()->createEmptyAttributes();
      pAttr->addBool("shrinknode",m_bShrinkNode);
      pAttr->write(pWriter);
      pAttr->drop();


      pWriter->writeClosingTag(L"Parameters");
      pWriter->writeLineBreak();

      //Write the segments
      core::list<CSegment *>::Iterator sit;
      for (sit=m_lSegments.begin(); sit!=m_lSegments.end(); sit++) {
        CSegment *p=*sit;
        pWriter->writeElement(L"Segment",false);
        pWriter->writeLineBreak();
        io::IAttributes *pAttr=m_pDevice->getFileSystem()->createEmptyAttributes();
        p->save(pAttr);
        pAttr->write(pWriter);
        pAttr->drop();

        //Write the segment texture parameters
        for (u32 i=0; i<_SEGMENT_NUMBER_OF_BUFFERS; i++) {
          core::stringw s=L"TextureParams";

          pWriter->writeElement(s.c_str(),false);
          pWriter->writeLineBreak();

          CTextureParameters *t=p->getTextureParameters(i);
          io::IAttributes *pTexAttr=m_pDevice->getFileSystem()->createEmptyAttributes();
          t->save(pTexAttr);
          pTexAttr->write(pWriter);
          pTexAttr->drop();

          pWriter->writeClosingTag(s.c_str());
          pWriter->writeLineBreak();
        }

        pWriter->writeClosingTag(L"Segment");
        pWriter->writeLineBreak();
      }

      //Write the connections
      core::list<CConnection *>::Iterator cit;
      for (cit=m_lConnections.begin(); cit!=m_lConnections.end(); cit++) {
        CConnection *p=*cit;
        pWriter->writeElement(L"Connection",false);
        pWriter->writeLineBreak();
        io::IAttributes *pAttr=m_pDevice->getFileSystem()->createEmptyAttributes();
        p->save(pAttr);
        pAttr->write(pWriter);
        pAttr->drop();

        //Write the connection's texture parameters
        for (u32 i=0; i<_CONNECTION_NUMBER_OF_BUFFERS; i++) {
          core::stringw s=L"TextureParams";

          pWriter->writeElement(s.c_str(),false);
          pWriter->writeLineBreak();

          CTextureParameters *t=p->getTextureParameters(i);
          io::IAttributes *pTexAttr=m_pDevice->getFileSystem()->createEmptyAttributes();
          t->save(pTexAttr);
          pTexAttr->write(pWriter);
          pTexAttr->drop();

          pWriter->writeClosingTag(s.c_str());
          pWriter->writeLineBreak();
        }
        pWriter->writeClosingTag(L"Connection");
        pWriter->writeLineBreak();
      }

      if (m_pSurface->isVisible()) {
        pWriter->writeElement(L"Surface",false);
        pWriter->writeLineBreak();
        io::IAttributes *pAttr=m_pDevice->getFileSystem()->createEmptyAttributes();
        m_pSurface->save(pAttr);
        pAttr->write(pWriter);
        pAttr->drop();

        for (u32 i=0; i<2; i++) {
          CTextureParameters *p=m_pSurface->getTextureParameters(i);
          if (p!=NULL) {
            core::stringw s=L"TextureParams";

            pWriter->writeElement(s.c_str(),false);
            pWriter->writeLineBreak();

            io::IAttributes *pAttr=m_pDevice->getFileSystem()->createEmptyAttributes();
            p->save(pAttr);
            pAttr->write(pWriter);
            pAttr->drop();

            pWriter->writeClosingTag(s.c_str());
            pWriter->writeLineBreak();
          }
        }
      }

      pWriter->writeClosingTag(L"BulletByteRoadCreator");
      pWriter->drop();
    }
    else printf("ERROR: unable to create XML writer for file \"%s\".\n",m_sCurrentRoad.c_str());
    pFile->drop();
  }
  else printf("ERROR: unable to write file \"%s\".\n",m_sCurrentRoad.c_str());
}

/**
 * Remove a road from memory
 */
void CRoadLoader::destroyRoad() {
  m_sCurrentRoad="";

  //remove all segments
  #ifdef _IRREDIT_PLUGIN
    while (m_lSegments.getSize()>0) {
  #else
    while (m_lSegments.size()>0) {
  #endif
    core::list<CSegment *>::Iterator it=m_lSegments.begin();
    CSegment *p=*it;
    m_lSegments.erase(it);
    delete p;
  }

  //remove all connections
  #ifdef _IRREDIT_PLUGIN
    while (m_lConnections.getSize()>0) {
  #else
    while (m_lConnections.size()>0) {
  #endif
    core::list<CConnection *>::Iterator it=m_lConnections.begin();
    CConnection *p=*it;
    m_lConnections.erase(it);
    delete p;
  }
}

core::list<CSegment    *> &CRoadLoader::getSegments   () { return m_lSegments   ; }
core::list<CConnection *> &CRoadLoader::getConnections() { return m_lConnections; }

const core::stringc CRoadLoader::getCurrentRoadName() { return m_sCurrentRoad; }
void CRoadLoader::setCurrentRoadName(const core::stringc s) { m_sCurrentRoad=s; }

scene::IAnimatedMesh *CRoadLoader::createMesh() {
  core::array<core::aabbox3df> aBoxes;
  core::array<scene::IMeshBuffer *> aBuffers;

  aBuffers.clear();

  core::vector3df vCenterPos=core::vector3df(0.0f,0.0f,0.0f);

  core::list<CSegment *>::Iterator sit;
  for (sit=m_lSegments.begin(); sit!=m_lSegments.end(); sit++) {
    CSegment *pSeg=*sit;
    vCenterPos+=pSeg->getPosition();
    for (u32 i=0; i<_SEGMENT_NUMBER_OF_BUFFERS; i++) {
      bool bDoIt=true;
      for (u32 iTex=0; iTex<pSeg->getTextureCount() && bDoIt; iTex++) {
        if (pSeg->getTextureParameters(iTex)->getTexture()=="") bDoIt=false;
      }

      if (bDoIt) {
        scene::IMeshBuffer *p=pSeg->getMeshBuffer(i);
        if (p && p->getVertexCount()>0) {
          aBoxes.push_back(p->getBoundingBox());
          addBufferToArray(p,aBuffers);
        }
      }
    }
  }

  #ifdef _IRREDIT_PLUGIN
    vCenterPos/=m_lSegments.getSize();
  #else
    vCenterPos/=m_lSegments.size();
  #endif

  vCenterPos.X=(f32)abs((s32)vCenterPos.X);
  vCenterPos.Y=(f32)abs((s32)vCenterPos.Y);
  vCenterPos.Z=(f32)abs((s32)vCenterPos.Z);

  core::list<CConnection *>::Iterator cit;
  for (cit=m_lConnections.begin(); cit!=m_lConnections.end(); cit++) {
    CConnection *pCon=*cit;
    for (u32 i=0; i<_CONNECTION_NUMBER_OF_BUFFERS; i++) {
      bool bDoIt=true;
      for (u32 iTex=0; iTex<pCon->getTextureCount() && bDoIt; iTex++) {
        if (pCon->getTextureParameters(iTex)->getTexture()=="") bDoIt=false;
      }

      if (bDoIt) {
        scene::IMeshBuffer *p=pCon->getMeshBuffer(i);
        if (p!=NULL && p->getVertexCount()>0) {
          addBufferToArray(p,aBuffers);
        }
      }
    }
  }

  if (m_pSurface!=NULL && m_pSurface->isVisible()) {
    for (u32 i=0; i<2; i++) {
      bool bDoIt=true;

      for (u32 iTex=0; iTex<m_pSurface->getTextureCount() && bDoIt; iTex++) {
        if (m_pSurface->getTextureParameters(iTex)->getTexture()=="") bDoIt=false;
      }

      if (bDoIt) {
        scene::IMeshBuffer *p=m_pSurface->getMeshBuffer(i);
        if (p!=NULL && p->getVertexCount()>0) {
          addBufferToArray(p,aBuffers);
        }
      }
    }
  }

  for (u32 i=0; i<aBuffers.size(); i++) {
    scene::IMeshBuffer *p=aBuffers[i];
    for (u32 j=0; j<p->getVertexCount(); j++) {
      ((video::S3DVertex *)p->getVertices())[j].Color=video::SColor(0xFF,0xFF,0xFF,0xFF);
    }
  }

	if (m_bShrinkNode) {
    printf("Shrink!\n");
    m_vOfffset=vCenterPos;
    #ifndef _ROAD_CREATOR_TOOL
      for (u32 i=0; i<aBuffers.size(); i++) {
        scene::IMeshBuffer *p=aBuffers[i];
        for (u32 j=0; j<p->getVertexCount(); j++) {
          p->getPosition(j)=p->getPosition(j)-m_vOfffset;
        }
      }
    #endif
	}

  scene::SMesh *pMesh=new scene::SMesh();
  for (u32 i=0; i<aBuffers.size(); i++) {
    aBuffers[i]->recalculateBoundingBox();
    aBoxes.push_back(aBuffers[i]->getBoundingBox());
    pMesh->addMeshBuffer(aBuffers[i]);
  }

  core::aabbox3df cBox=pMesh->getBoundingBox();

  for (u32 i=0; i<aBoxes.size(); i++) cBox.addInternalBox(aBoxes[i]);
  pMesh->setBoundingBox(cBox);

	scene::SAnimatedMesh *pAnimatedMesh=new scene::SAnimatedMesh();
	pAnimatedMesh->addMesh(pMesh);

  return pAnimatedMesh;
}

bool CRoadLoader::deleteConnection(CConnection *pToDelete) {
  core::list<CConnection *>::Iterator it;
  for (it=m_lConnections.begin(); it!=m_lConnections.end(); it++) {
    CConnection *p=*it;
    if (p==pToDelete) {
      m_lConnections.erase(it);
      delete p;
      return true;
    }
  }
  return false;
}

bool CRoadLoader::deleteSegment(CSegment *pToDelete) {
  core::list<CSegment *>::Iterator it;
  for (it=m_lSegments.begin(); it!=m_lSegments.end(); it++) {
    CSegment *p=*it;
    if (p==pToDelete) {
      m_lSegments.erase(it);
      delete p;
      return true;
    }
  }
  return false;
}
