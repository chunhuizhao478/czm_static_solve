#include "SlipWeakeningMaterial.h"
#include "InterfaceKernel.h"

registerMooseObject("czm_static_solveApp", SlipWeakeningMaterial);

InputParameters
SlipWeakeningMaterial::validParams()
{
  InputParameters params = CZMComputeLocalTractionTotalBase::validParams();
  params.addClassDescription("Linear Slip Weakening Traction Separation Law.");
  params.addParam<Real>("Dc", 1.0, "Value of characteristic length");
  params.addParam<Real>("mu_s", 1.0, "Value of static friction coefficient");
  params.addParam<Real>("mu_d", 1.0, "Value of dynamic friction coefficient");
  return params;
}

SlipWeakeningMaterial::SlipWeakeningMaterial(const InputParameters & parameters)
  : CZMComputeLocalTractionTotalBase(parameters),
    _Dc(getParam<Real>("Dc")),
    _mu_s(getParam<Real>("mu_s")),
    _mu_d(getParam<Real>("mu_d")),
    _rot(getMaterialPropertyByName<RankTwoTensor>(_base_name + "czm_total_rotation")),
    _stress(getMaterialPropertyByName<RankTwoTensor>(_base_name + "stress"))
{
}

void
SlipWeakeningMaterial::computeInterfaceTractionAndDerivatives()
{   
   //Parameter initialization
   Real mu_s = _mu_s; 
   Real mu_d = _mu_d; 
   Real Dc = _Dc;

   //updated local trangential traction
   Real tau_f = 0;

   //updated local normal traction
   Real tau_n = 0;
  
   //global tangential traction
   Real T1_o_global = std::norm(_stress[_qp] * _normals[_qp] - _stress[_qp] * _normals[_qp] * _normals[_qp] * _normals[_qp]);
   
   //global normal traction
   Real T2_o_global = _stress[_qp] * _normals[_qp] * _normals[_qp];

   //global traction vector
   RealVectorValue Tglobal(T1_o_global,T2_o_global,0.0);

   //local traction vector
   RealVectorValue Tlocal = _rot[_qp].transpose() * Tglobal;
   
   //local tangential traction
   Real T1_o = Tlocal(1);

   //local normal traction
   Real T2_o = Tlocal(0);
  
   //Compute fault traction
   if ( T2_o < 0 ){
      tau_n = T2_o;
   }
   else{
      tau_n = 0;
   }

   //Compute Tangential Friction Strength
   Real T1_fs = mu_s * (-T2_o);
   
   if ( T1_o > T1_fs ){
      //Compute friction strength
      if (std::abs(_interface_displacement_jump[_qp](1)) < Dc)
      {
        tau_f = (mu_s - (mu_s - mu_d)*std::abs(_interface_displacement_jump[_qp](1))/Dc)*(-T2_o);
      } 
      else
      {
        tau_f = mu_d * (-T2_o);
      }
   }
   else{
        tau_f = T1_o;
   }

   //Assign back traction in CZM
   RealVectorValue traction;

   traction(0) = tau_n; 
   traction(1) = -tau_f; 
   traction(2) = 0;

   _interface_traction[_qp] = traction;
   _dinterface_traction_djump[_qp] = 0;

}