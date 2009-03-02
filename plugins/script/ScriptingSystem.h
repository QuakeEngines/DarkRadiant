#ifndef _SCRIPTING_SYSTEM_H_
#define _SCRIPTING_SYSTEM_H_

#include <boost/python.hpp>
#include <map>
#include <vector>

#include "iscript.h"
#include "PythonConsoleWriter.h"
#include "icommandsystem.h"

#include "ScriptCommand.h"

namespace script {

// Forward declaration
class StartupListener;
typedef boost::shared_ptr<StartupListener> StartupListenerPtr;

class ScriptingSystem :
	public IScriptingSystem
{
	PythonConsoleWriter _outputWriter;
	PythonConsoleWriter _errorWriter;

	bool _initialised;

	typedef std::pair<std::string, IScriptInterfacePtr> NamedInterface;
	typedef std::vector<NamedInterface> Interfaces;
	Interfaces _interfaces;

	boost::python::object _mainModule;
	boost::python::object _mainNamespace;
	boost::python::dict _globals;

	StartupListenerPtr _startupListener;

	// The path where the script files are hosted
	std::string _scriptPath;

	// All named script commands (pointing to .py files)
	typedef std::map<std::string, ScriptCommandPtr> ScriptCommandMap;
	ScriptCommandMap _commands;

public:
	ScriptingSystem();

	// Adds a script interface to this system
	void addInterface(const std::string& name, const IScriptInterfacePtr& iface);

	// (Re)loads all scripts from the scripts/ folder
	void reloadScriptsCmd(const cmd::ArgumentList& args);

	/**
	 * This actually initialises the Scripting System, adding all
	 * registered interfaces to the Python context. After this call
	 * the scripting system is ready for use.
	 *
	 * This method also invokes "scripts/init.py" when done.
	 */
	void initialise();

	// Runs a specific script file (command target)
	void runScriptFile(const cmd::ArgumentList& args);

	// Runs a named script command (command target)
	void runScriptCommand(const cmd::ArgumentList& args);

	// Executes a script file
	void executeScriptFile(const std::string& filename);

	// Runs the named command (or rather the .py file behind it)
	void executeCommand(const std::string& name);

	// RegisterableModule implementation
	const std::string& getName() const;
	const StringSet& getDependencies() const;
	void initialiseModule(const ApplicationContext& ctx);
	void shutdownModule();

private: 
	bool interfaceExists(const std::string& name);

	void reloadScripts();

	void loadCommandScript(const std::string& scriptFilename);
};
typedef boost::shared_ptr<ScriptingSystem> ScriptingSystemPtr;

} // namespace script

#endif /* _SCRIPTING_SYSTEM_H_ */
