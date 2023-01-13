//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#include "czm_static_solveTestApp.h"
#include "czm_static_solveApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"
#include "ModulesApp.h"

InputParameters
czm_static_solveTestApp::validParams()
{
  InputParameters params = czm_static_solveApp::validParams();
  return params;
}

czm_static_solveTestApp::czm_static_solveTestApp(InputParameters parameters) : MooseApp(parameters)
{
  czm_static_solveTestApp::registerAll(
      _factory, _action_factory, _syntax, getParam<bool>("allow_test_objects"));
}

czm_static_solveTestApp::~czm_static_solveTestApp() {}

void
czm_static_solveTestApp::registerAll(Factory & f, ActionFactory & af, Syntax & s, bool use_test_objs)
{
  czm_static_solveApp::registerAll(f, af, s);
  if (use_test_objs)
  {
    Registry::registerObjectsTo(f, {"czm_static_solveTestApp"});
    Registry::registerActionsTo(af, {"czm_static_solveTestApp"});
  }
}

void
czm_static_solveTestApp::registerApps()
{
  registerApp(czm_static_solveApp);
  registerApp(czm_static_solveTestApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
// External entry point for dynamic application loading
extern "C" void
czm_static_solveTestApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  czm_static_solveTestApp::registerAll(f, af, s);
}
extern "C" void
czm_static_solveTestApp__registerApps()
{
  czm_static_solveTestApp::registerApps();
}
