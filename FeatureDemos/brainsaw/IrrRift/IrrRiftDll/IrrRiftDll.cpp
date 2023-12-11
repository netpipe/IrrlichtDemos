// Win32Project1.cpp : Definiert die exportierten Funktionen für die DLL-Anwendung.
//

#include "stdafx.h"
#include "IrrRiftDll.h"
#include <OVR.h>

OVR::Ptr<OVR::DeviceManager> g_pRiftManager = NULL;
OVR::Ptr<OVR::HMDDevice    > g_pRiftHMD     = NULL;
OVR::Ptr<OVR::SensorDevice > g_pRiftSensor  = NULL;
OVR::SensorFusion            g_cRiftFusion;

bool g_bRiftAvailable = true;

WIN32PROJECT1_API bool IrrRift_install(void) {
  OVR::System::Init(OVR::Log::ConfigureDefaultLog(OVR::LogMask_All));
  g_pRiftManager = *OVR::DeviceManager::Create();

  //g_bRiftAvailable = g_pRiftManager->EnumerateDevices<OVR::HMDDevice>().IsAvailable();

  printf("g_bRiftAvailable: %i\n", (int)g_bRiftAvailable);
  /*if (g_bRiftAvailable)*/ {
    g_pRiftHMD    = *g_pRiftManager->EnumerateDevices<OVR::HMDDevice>().CreateDevice();
    g_pRiftSensor = *g_pRiftHMD->GetSensor();

    if (g_pRiftSensor) g_cRiftFusion.AttachToSensor(g_pRiftSensor);
  }

  //return g_bRiftAvailable;
  return true;
}

WIN32PROJECT1_API void IrrRift_destall(void) {
  if (g_pRiftHMD    ) g_pRiftHMD    ->Release();
  if (g_pRiftManager) g_pRiftManager->Release();

  OVR::System::Destroy();
}

WIN32PROJECT1_API void IrrRift_poll(float &x, float &y, float &z) {
  if (!g_bRiftAvailable) {
    x = 0.0;
    y = 0.0;
    z = 0.0;
  }
  else {
    OVR::Quatf l_cHmdOrient = g_cRiftFusion.GetOrientation();
    l_cHmdOrient.GetEulerAngles<OVR::Axis_X, OVR::Axis_Y, OVR::Axis_Z>(&x, &y, &z);
  }
}
