#include "ShaderSystemInterface.h"

namespace script {

void ShaderSystemInterface::foreachShader(shaders::ShaderVisitor& visitor) {
	GlobalShaderSystem().foreachShader(visitor);
}

// IScriptInterface implementation
void ShaderSystemInterface::registerInterface(boost::python::object& nspace) {
	// Add the declaration for a Shader object
	nspace["Shader"] = boost::python::class_<ScriptShader>(
		"Shader", boost::python::init<const IShaderPtr&>())
		.def("getName", &ScriptShader::getName)
		.def("getShaderFileName", &ScriptShader::getShaderFileName)
		.def("isNull", &ScriptShader::isNull)
	;
	
	// Expose the ShaderVisitor interface
	nspace["ShaderVisitor"] = 
		boost::python::class_<ShaderVisitorWrapper, boost::noncopyable>("ShaderVisitor")
		.def("visit", boost::python::pure_virtual(&ShaderVisitorWrapper::visit))
	;

	// Add the module declaration to the given python namespace
	nspace["GlobalShaderSystem"] = boost::python::class_<ShaderSystemInterface>("GlobalShaderSystem")
		.def("foreachShader", &ShaderSystemInterface::foreachShader)
	;

	// Now point the Python variable "GlobalShaderSystem" to this instance
	nspace["GlobalShaderSystem"] = boost::python::ptr(this);
}

} // namespace script
