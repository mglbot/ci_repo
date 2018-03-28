////////////////////////////////////////////////////////////////////////////////
// This file is distributed under the University of Illinois/NCSA Open Source
// License.  See LICENSE file in top directory for details.
//
// Copyright (c) 2016 Jeongnim Kim and QMCPACK developers.
//
// File developed by:
//
// File created by: Jeongnim Kim, jeongnim.kim@intel.com, Intel Corp.
////////////////////////////////////////////////////////////////////////////////
// -*- C++ -*-

/**
 * @file WaveFunction.h
 * @brief Top level wavefunction container
 *
 * Represents a product of wavefunction components (classes based on
 * WaveFunctionComponentBase).
 *
 * Corresponds to QMCWaveFunction/TrialWaveFunction.h in the QMCPACK source.
 */

#ifndef QMCPLUSPLUS_WAVEFUNCTIONS_H
#define QMCPLUSPLUS_WAVEFUNCTIONS_H
#include <Utilities/Configuration.h>
#include <Utilities/RandomGenerator.h>
#include <Particle/DistanceTable.h>
#include <QMCWaveFunctions/Determinant.h>
#include <QMCWaveFunctions/DeterminantRef.h>
#include <QMCWaveFunctions/Jastrow/BsplineFunctor.h>
#include <QMCWaveFunctions/Jastrow/PolynomialFunctor3D.h>
#include <QMCWaveFunctions/Jastrow/OneBodyJastrowRef.h>
#include <QMCWaveFunctions/Jastrow/OneBodyJastrow.h>
#include <QMCWaveFunctions/Jastrow/TwoBodyJastrowRef.h>
#include <QMCWaveFunctions/Jastrow/TwoBodyJastrow.h>
#include <QMCWaveFunctions/Jastrow/ThreeBodyJastrowRef.h>
#include <QMCWaveFunctions/Jastrow/ThreeBodyJastrow.h>

namespace qmcplusplus
{
/** A minimal TrialWavefunction
 */
struct WaveFunctionBase
{
  using valT = OHMMS_PRECISION;
  using posT = TinyVector<OHMMS_PRECISION, OHMMS_DIM>;
  typedef OHMMS_PRECISION RealType;

  valT LogValue;
  DistanceTableData *d_ee;
  DistanceTableData *d_ie;

  inline void setRmax(valT x) { d_ie->setRmax(x); }

  virtual ~WaveFunctionBase() {}
  virtual void evaluateLog(ParticleSet &P)                    = 0;
  virtual posT evalGrad(ParticleSet &P, int iat)              = 0;
  virtual valT ratioGrad(ParticleSet &P, int iat, posT &grad) = 0;
  virtual valT ratio(ParticleSet &P, int iat)                 = 0;
  virtual void acceptMove(ParticleSet &P, int iat)            = 0;
  virtual void restore(int iat)                               = 0;
  virtual void evaluateGL(ParticleSet &P)                     = 0;
};

struct WaveFunction : public WaveFunctionBase
{
  using J1OrbType = OneBodyJastrow<BsplineFunctor<valT>>;
  using J2OrbType = TwoBodyJastrow<BsplineFunctor<valT>>;
  using J3OrbType = ThreeBodyJastrow<PolynomialFunctor3D>;
  using DetType   = DiracDeterminant;

  bool FirstTime;
  J1OrbType *J1;
  J2OrbType *J2;
  J3OrbType *J3;
  int nelup;
  DetType *Det_up;
  DetType *Det_dn;

  WaveFunction(ParticleSet &ions, ParticleSet &els,
               RandomGenerator<RealType> RNG);
  ~WaveFunction();
  void evaluateLog(ParticleSet &P);
  posT evalGrad(ParticleSet &P, int iat);
  valT ratioGrad(ParticleSet &P, int iat, posT &grad);
  valT ratio(ParticleSet &P, int iat);
  void acceptMove(ParticleSet &P, int iat);
  void restore(int iat);
  void evaluateGL(ParticleSet &P);
};

} // qmcplusplus

namespace miniqmcreference
{
/** A minimial TrialWaveFunction - the reference version.
 */
using namespace qmcplusplus;
struct WaveFunctionRef : public qmcplusplus::WaveFunctionBase
{

  using J1OrbType = OneBodyJastrow<BsplineFunctor<valT>>;
  using J2OrbType = TwoBodyJastrow<BsplineFunctor<valT>>;
  using J3OrbType = ThreeBodyJastrow<PolynomialFunctor3D>;
  using DetType   = DiracDeterminantRef;

  bool FirstTime;
  J1OrbType *J1;
  J2OrbType *J2;
  J3OrbType *J3;
  int nelup;
  DetType *Det_up;
  DetType *Det_dn;
  PooledData<valT> Buffer;

  WaveFunctionRef(ParticleSet &ions, ParticleSet &els,
                  RandomGenerator<RealType> RNG);
  ~WaveFunctionRef();
  void evaluateLog(ParticleSet &P);
  posT evalGrad(ParticleSet &P, int iat);
  valT ratioGrad(ParticleSet &P, int iat, posT &grad);
  valT ratio(ParticleSet &P, int iat);
  void acceptMove(ParticleSet &P, int iat);
  void restore(int iat);
  void evaluateGL(ParticleSet &P);
};
} // miniqmcreference

#endif
