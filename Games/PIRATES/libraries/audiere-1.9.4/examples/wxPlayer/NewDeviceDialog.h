#ifndef ADR_NEW_DEVICE_DIALOG_H
#define ADR_NEW_DEVICE_DIALOG_H


#include <vector>
#include <audiere.h>
#include "wx.h"


class NewDeviceDialog : public wxDialog {
public:
  NewDeviceDialog(wxWindow* parent);

  const std::string& getDevice() const;
  const std::string& getParameters() const;

private:
  void OnButton(wxCommandEvent& event);

  std::vector<audiere::AudioDeviceDesc> m_devices;

  wxChoice*   m_device;
  wxTextCtrl* m_parameters;

  wxButton* m_ok;
  wxButton* m_cancel;

  std::string m_device_str;
  std::string m_parameters_str;

  DECLARE_EVENT_TABLE()
};


#endif
