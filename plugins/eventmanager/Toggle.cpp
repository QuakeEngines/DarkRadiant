#include "Toggle.h"

Toggle::Toggle(const Callback& callback) :
	_callback(callback),
	_callbackActive(false),
	_toggled(false)
{}

Toggle::~Toggle() {
	for (ToggleWidgetList::iterator i = _toggleWidgets.begin(); i != _toggleWidgets.end(); ++i) {
		if (GTK_IS_WIDGET(i->first)) {
			g_signal_handler_disconnect(i->first, i->second);
		}
	}
}

bool Toggle::empty() const {
	return false;
}

bool Toggle::isToggle() const {
	return true;
}

// Set the toggled state to true/false, according to <toggled> and update
// any associated widgets or notify any callbacks.
bool Toggle::setToggled(const bool toggled) {
	if (_callbackActive) {
		return false;
	}
	
	// Update the toggle status and export it to the GTK button
	_toggled = toggled;
	updateWidgets();
	
	return true;
}

void Toggle::updateWidgets() {
	_callbackActive = true;
	
	for (ToggleWidgetList::iterator i = _toggleWidgets.begin();
		 i != _toggleWidgets.end();
		 ++i)
	{
		GtkWidget* widget = i->first;
		if (GTK_IS_TOGGLE_TOOL_BUTTON(widget)) {
			gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(widget), _toggled);
		}
		else if (GTK_IS_CHECK_MENU_ITEM(widget)) {
			gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(widget), _toggled);
		}
	}
	
	_callbackActive = false;
}

// On key press >> toggle the internal state
void Toggle::keyDown() {
	toggle();
}

bool Toggle::isToggled() const {
	return _toggled;
}

void Toggle::connectWidget(GtkWidget* widget) {
	if (GTK_IS_TOGGLE_TOOL_BUTTON(widget)) {
			
		gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(widget), _toggled);
		
		// Connect the toggleToolbutton to the static callback of this class
		gulong handle = g_signal_connect(G_OBJECT(widget), "toggled", G_CALLBACK(onToggleToolButtonClicked), this);

		// Store the pointer for later use
		_toggleWidgets[widget] = handle;
	}
	else if (GTK_IS_TOGGLE_BUTTON(widget)) {
		
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), _toggled);
		
		// Connect the toggleToolbutton to the static callback of this class
		gulong handle = g_signal_connect(G_OBJECT(widget), "toggled", G_CALLBACK(onToggleToolButtonClicked), this);

		// Store the pointer for later use
		_toggleWidgets[widget] = handle;
	}
	else if (GTK_IS_CHECK_MENU_ITEM(widget)) {
		
		gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(widget), _toggled);
		
		gulong handle = g_signal_connect(G_OBJECT(widget), "toggled", G_CALLBACK(onCheckMenuItemClicked), this);

		_toggleWidgets[widget] = handle;
	}
}

// Invoke the registered callback and update/notify
void Toggle::toggle() {
	if (_callbackActive) {
		return;
	}
	
	// Check if the toggle event is enabled
	if (_enabled) {
		// Invert the <toggled> state 
		_toggled = !_toggled;
		
		// Call the connected function
		_callback();
	}
	
	// Update any attached GtkObjects in any case
	updateWidgets();
}

// The static GTK callback methods that can be connected to a ToolButton or a MenuItem
gboolean Toggle::onToggleToolButtonClicked(GtkToggleToolButton* toolButton, gpointer data) {
	
	// Cast the passed pointer onto a Toggle class. Note: this may also call a toggle() method
	// of a derived class.
	Toggle* self = reinterpret_cast<Toggle*>(data);
	
	// Check sanity and toggle this item
	if (self != NULL) {
		self->toggle();
	}
	
	return true;
}

gboolean Toggle::onToggleButtonClicked(GtkToggleButton* toggleButton, gpointer data) {
	
	// Cast the passed pointer onto a Toggle class. Note: this may also call a toggle() method
	// of a derived class.
	Toggle* self = reinterpret_cast<Toggle*>(data);
	
	// Check sanity and toggle this item
	if (self != NULL) {
		self->toggle();
	}
	
	return true;
}

gboolean Toggle::onCheckMenuItemClicked(GtkMenuItem* menuitem, gpointer data) {
	Toggle* self = reinterpret_cast<Toggle*>(data);
	
	// Check sanity and toggle this item
	if (self != NULL) {
		self->toggle();
	}
	
	return true;
}
