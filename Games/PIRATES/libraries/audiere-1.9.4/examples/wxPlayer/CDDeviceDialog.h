#ifndef ADR_CD_DEVICE_DIALOG_H
#define ADR_CD_DEVICE_DIALOG_H


#include <vector>
#include <string>
#include "wx.h"


class CDDeviceDialog : public wxDialog {
public:
  CDDeviceDialog(wxWindow* parent);

  const std::string& getName() const;

private:
  void OnButton(wxCommandEvent& event);

  std::vector<std::string> m_devices;
  std::string m_name;

  wxChoice* m_device;
  wxTextCtrl* m_explicit_device;
  wxButton* m_ok;
  wxButton* m_cancel;

  DECLARE_EVENT_TABLE()
};


#endif
