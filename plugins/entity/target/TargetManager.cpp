#include "TargetManager.h"

namespace entity {

// Constructor
TargetManager::TargetManager() :
	_emptyTarget(new Target)
{
	// Just to be sure, clear the empty target
	_emptyTarget->clear();
}

// Static accessor method
TargetManager& TargetManager::Instance() {
	static TargetManager _instance;
	return _instance;
}

TargetPtr TargetManager::getTarget(const std::string name) {
	if (name.empty()) {
		return _emptyTarget;
	}

	TargetList::iterator found = _targets.find(name);

	if (found != _targets.end()) {
		return found->second;
	}

	// Doesn't exist yet, create this one
	TargetPtr target(new Target);
	target->clear();

	// Insert into the local map and return
	_targets.insert(TargetList::value_type(name, target));

	return target;
}

void TargetManager::associateTarget(const std::string& name, scene::Instance* instance) {
	if (name.empty()) {
		return; // don't associate empty names
	}

	TargetList::iterator found = _targets.find(name);

	if (found != _targets.end()) {
		if (found->second->isEmpty()) {
			// Already registered, but empty => associate it
			found->second->setInstance(instance);
		}
		else {
			// Non-empty target!
			globalErrorStream() << "TargetManager: Target " << name.c_str() << " already associated!\n";
		}

		return;
	}

	// Doesn't exist yet, create a new target
	TargetPtr target(new Target);

	// Associate the target
	target->setInstance(instance);

	// Insert into the local map and return
	_targets.insert(TargetList::value_type(name, target));
}

void TargetManager::clearTarget(const std::string& name) {
	// Locate and clear the named target
	TargetList::iterator found = _targets.find(name);

	if (found != _targets.end()) {
		// Found, clear it
		found->second->clear();
	}
}

} // namespace entity
