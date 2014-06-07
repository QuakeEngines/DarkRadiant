#include "DestroyComponentEditor.h"
#include "../SpecifierType.h"
#include "../Component.h"

#include "string/convert.h"

#include "i18n.h"
#include <wx/stattext.h>
#include <wx/spinctrl.h>

namespace objectives 
{

namespace ce
{

// Registration helper, will register this editor in the factory
DestroyComponentEditor::RegHelper DestroyComponentEditor::regHelper;

// Constructor
DestroyComponentEditor::DestroyComponentEditor(wxWindow* parent, Component& component) :
	_component(&component),
	_itemSpec(new SpecifierEditCombo(parent, SpecifierType::SET_ITEM()))
{
	_amount = new wxSpinCtrl(parent, wxID_ANY);
	_amount->SetValue(1);
	_amount->SetRange(0, 65535);

	wxStaticText* label = new wxStaticText(parent, wxID_ANY, _("Item:"));
	label->SetFont(label->GetFont().Bold());

	_panel->GetSizer()->Add(label, 0, wxBOTTOM, 6);
	_panel->GetSizer()->Add(_itemSpec, 0, wxBOTTOM | wxEXPAND, 6);

	_panel->GetSizer()->Add(new wxStaticText(parent, wxID_ANY, _("Amount:")), 0, wxBOTTOM, 6);
	_panel->GetSizer()->Add(_amount, 0, wxBOTTOM | wxEXPAND, 6);

    // Populate the SpecifierEditCombo with the first specifier
    _itemSpec->setSpecifier(
        component.getSpecifier(Specifier::FIRST_SPECIFIER)
    );

	// Initialise the spin button with the value from the first component argument
	_amount->SetValue(string::convert<double>(component.getArgument(0)));
}

// Write to component
void DestroyComponentEditor::writeToComponent() const
{
    assert(_component);
    _component->setSpecifier(
        Specifier::FIRST_SPECIFIER, _itemSpec->getSpecifier()
    );

	_component->setArgument(0,
		string::to_string(_amount->GetValue()));
}

} // namespace ce

} // namespace objectives
