#pragma once

#include "CZMComputeLocalTractionTotalBase.h"

class SlipWeakeningMaterial : public CZMComputeLocalTractionTotalBase
{
public:
  static InputParameters validParams();
  SlipWeakeningMaterial(const InputParameters & parameters);

protected:
  /// method computing the total traction and its derivatives
  void computeInterfaceTractionAndDerivatives() override;

  Real _Dc;
  Real _mu_s;
  Real _mu_d;

  const MaterialProperty<RankTwoTensor> & _rot;

  const MaterialProperty<RankTwoTensor> & _stress;
 
};