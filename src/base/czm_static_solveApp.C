#include "czm_static_solveApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

InputParameters
czm_static_solveApp::validParams()
{
  InputParameters params = MooseApp::validParams();

  return params;
}

czm_static_solveApp::czm_static_solveApp(InputParameters parameters) : MooseApp(parameters)
{
  czm_static_solveApp::registerAll(_factory, _action_factory, _syntax);
}

czm_static_solveApp::~czm_static_solveApp() {}

void
czm_static_solveApp::registerAll(Factory & f, ActionFactory & af, Syntax & syntax)
{
  ModulesApp::registerAll(f, af, syntax);
  Registry::registerObjectsTo(f, {"czm_static_solveApp"});
  Registry::registerActionsTo(af, {"czm_static_solveApp"});

  /* register custom execute flags, action syntax, etc. here */
}

void
czm_static_solveApp::registerApps()
{
  registerApp(czm_static_solveApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
czm_static_solveApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  czm_static_solveApp::registerAll(f, af, s);
}
extern "C" void
czm_static_solveApp__registerApps()
{
  czm_static_solveApp::registerApps();
}
