////////////////////////////////////////////////////////////////////////////////
// This file is distributed under the University of Illinois/NCSA Open Source
// License.  See LICENSE file in top directory for details.
//
// Copyright (c) 2016 Jeongnim Kim and QMCPACK developers.
//
// File developed by:
// Ken Esler, kpesler@gmail.com,
//    University of Illinois at Urbana-Champaign
// Miguel Morales, moralessilva2@llnl.gov,
//    Lawrence Livermore National Laboratory
// Jeremy McMinnis, jmcminis@gmail.com,
//    University of Illinois at Urbana-Champaign
// Jeongnim Kim, jeongnim.kim@gmail.com,
//    University of Illinois at Urbana-Champaign
// Raymond Clay III, j.k.rofling@gmail.com,
//    Lawrence Livermore National Laboratory
// Mark A. Berrill, berrillma@ornl.gov,
//    Oak Ridge National Laboratory
//
// File created by:
// Jeongnim Kim, jeongnim.kim@gmail.com,
//    University of Illinois at Urbana-Champaign
////////////////////////////////////////////////////////////////////////////////

#ifndef QMCPLUSPLUS_WAVEFUNCTIONCOMPONENTBASE_H
#define QMCPLUSPLUS_WAVEFUNCTIONCOMPONENTBASE_H
#include "Utilities/Configuration.h"
#include "Particle/ParticleSet.h"
#include "Particle/DistanceTableData.h"

/**@file WaveFunctionComponentBase.h
 *@brief Declaration of WaveFunctionComponentBase
 */
namespace qmcplusplus
{

/// forward declaration of WaveFunctionComponentBase
class WaveFunctionComponentBase;

typedef WaveFunctionComponentBase *WaveFunctionComponentBasePtr;

/**@defgroup WaveFunctionComponent Wavefunction Component group
 * @brief Classes which constitute a many-body trial wave function
 *
 * A many-body trial wave function is
 * \f[
 \Psi(\{ {\bf R}\}) = \prod_i \psi_{i}(\{ {\bf R}\}),
 * \f]
 * where \f$\Psi\f$s are represented by
 * the derived classes from WaveFunctionComponentBase.
 */
/** @ingroup WaveFunctionComponentComponent
 * @brief An abstract class for a component of a many-body trial wave function
 */
struct WaveFunctionComponentBase : public QMCTraits
{

  /// recasting enum of DistanceTableData to maintain consistency
  enum
  {
    SourceIndex  = DistanceTableData::SourceIndex,
    VisitorIndex = DistanceTableData::VisitorIndex,
    WalkerIndex  = DistanceTableData::WalkerIndex
  };

  /** enum for a update mode */
  enum
  {
    ORB_PBYP_RATIO, /*!< particle-by-particle ratio only */
    ORB_PBYP_ALL, /*!< particle-by-particle, update Value-Gradient-Laplacian */
    ORB_PBYP_PARTIAL, /*!< particle-by-particle, update Value and Grdient */
    ORB_WALKER,       /*!< walker update */
    ORB_ALLWALKER     /*!< all walkers update */
  };

  typedef ParticleAttrib<ValueType> ValueVectorType;
  typedef ParticleAttrib<GradType> GradVectorType;
  typedef PooledData<RealType> BufferType;
  typedef ParticleSet::Walker_t Walker_t;

  /** flag to set the optimization mode */
  bool IsOptimizing;
  /** boolean to set optimization
   *
   * If true, this object is actively modified during optimization
   */
  bool Optimizable;
  /** true, if FermionWF */
  bool IsFermionWF;
  /** true, if compute for the ratio instead of buffering */
  bool Need2Compute4PbyP;

  /** current update mode */
  int UpdateMode;
  /** current \f$\log\phi \f$
   */
  RealType LogValue;
  /** current phase
   */
  RealType PhaseValue;
  /** A vector for \f$ \frac{\partial \nabla \log\phi}{\partial \alpha} \f$
   */
  GradVectorType dLogPsi;
  /** A vector for \f$ \frac{\partial \nabla^2 \log\phi}{\partial \alpha} \f$
   */
  ValueVectorType d2LogPsi;
  /** Name of this wavefunction component
   */
  std::string WaveFunctionComponentName;

  /// default constructor
  WaveFunctionComponentBase()
      : IsOptimizing(false), Optimizable(true), UpdateMode(ORB_WALKER),
        LogValue(1.0), PhaseValue(0.0), WaveFunctionComponentName("WaveFunctionComponentBase")
  {
    /// store instead of computing
    Need2Compute4PbyP = false;
  }

  /// default destructor
  virtual ~WaveFunctionComponentBase() {}

  /** evaluate the value of the wavefunction
   * @param P active ParticleSet
   * @param G Gradients, \f$\nabla\ln\Psi\f$
   * @param L Laplacians, \f$\nabla^2\ln\Psi\f$
   *
   */
  virtual RealType evaluateLog(ParticleSet &P,
                               ParticleSet::ParticleGradient_t &G,
                               ParticleSet::ParticleLaplacian_t &L) = 0;

  /** return the current gradient for the iat-th particle
   * @param Pquantum particle set
   * @param iat particle index
   * @return the gradient of the iat-th particle
   */
  virtual GradType evalGrad(ParticleSet &P, int iat) = 0;

  /** evaluate the ratio of the new to old wavefunction component value
   * @param P the active ParticleSet
   * @param iat the index of a particle
   * @param grad_iat Gradient for the active particle
   */
  virtual ValueType ratioGrad(ParticleSet &P, int iat, GradType &grad_iat) = 0;

  /** a move for iat-th particle is accepted. Update the content for the next
   * moves
   * @param P target ParticleSet
   * @param iat index of the particle whose new position was proposed
   */
  virtual void acceptMove(ParticleSet &P, int iat) = 0;

  /** evalaute the ratio of the new to old wavefunction component value
   *@param P the active ParticleSet
   *@param iat the index of a particle
   *@return \f$ \psi( \{ {\bf R}^{'} \} )/ \psi( \{ {\bf R}^{'}\})\f$
   *
   *Specialized for particle-by-particle move.
   */
  virtual ValueType ratio(ParticleSet &P, int iat) = 0;

  /** compute G and L after the sweep
   * @param P active ParticleSet
   * @param G Gradients, \f$\nabla\ln\Psi\f$
   * @param L Laplacians, \f$\nabla^2\ln\Psi\f$
   * @param fromscratch, recompute internal data if true
   *
   */
  virtual void evaluateGL(ParticleSet &P, ParticleSet::ParticleGradient_t &G,
                          ParticleSet::ParticleLaplacian_t &L,
                          bool fromscratch = false) = 0;
};
}
#endif
