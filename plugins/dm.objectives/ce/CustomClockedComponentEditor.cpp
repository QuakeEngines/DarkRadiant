#include "CustomClockedComponentEditor.h"
#include "../SpecifierType.h"
#include "../Component.h"

#include "i18n.h"

#include <wx/spinctrl.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>

namespace objectives
{

namespace ce
{

// Registration helper, will register this editor in the factory
CustomClockedComponentEditor::RegHelper CustomClockedComponentEditor::regHelper;

// Constructor
CustomClockedComponentEditor::CustomClockedComponentEditor(wxWindow* parent, Component& component) :
	_component(&component),
	_scriptFunction(new wxTextCtrl(parent, wxID_ANY))
{
	_interval = new wxSpinCtrlDouble(parent, wxID_ANY);
	_interval->SetValue(1);
	_interval->SetRange(0, 65535);
	_interval->SetIncrement(0.1);
	_interval->SetDigits(1);

	// Main vbox
	wxStaticText* label = new wxStaticText(parent, wxID_ANY, _("Script Function:"));
	label->SetFont(label->GetFont().Bold());

	_panel->GetSizer()->Add(label, 0, wxBOTTOM, 6);
	_panel->GetSizer()->Add(_scriptFunction, 0, wxBOTTOM | wxEXPAND, 6);

	label = new wxStaticText(parent, wxID_ANY, _("Clock interval:"));
	label->SetFont(label->GetFont().Bold());
	_panel->GetSizer()->Add(label, 0, wxBOTTOM, 6);

	wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
	hbox->Add(_interval, 0, wxEXPAND | wxRIGHT, 6);
	hbox->Add(new wxStaticText(parent, wxID_ANY, _("seconds:")), 0, wxEXPAND | wxALIGN_CENTER_VERTICAL);

	_panel->GetSizer()->Add(hbox, 0, wxBOTTOM | wxEXPAND, 6);

	// Load the initial values into the boxes
	_scriptFunction->SetValue(component.getArgument(0));

	float interval = component.getClockInterval();
	_interval->SetValue(interval >= 0 ? interval : 1.0);
}

// Write to component
void CustomClockedComponentEditor::writeToComponent() const
{
    assert(_component);

	_component->setArgument(0, _scriptFunction->GetValue().ToStdString());
	_component->setClockInterval(static_cast<float>(_interval->GetValue()));
}

} // namespace ce

} // namespace objectives
