/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright 
 * information, see COPYRIGHT and COPYING.LESSER. 
 *
 * Copyright:     (c) 1997-2010 Lawrence Livermore National Security, LLC
 * Description:   DLBGTest class declaration 
 *
 ************************************************************************/
#ifndef included_DLBGTest
#define included_DLBGTest

#include <string>

#include "SAMRAI/tbox/Pointer.h"
#include "SAMRAI/tbox/Database.h"

/*
 * SAMRAI classes
 */
#include "SAMRAI/appu/VisItDataWriter.h"
#include "SAMRAI/appu/VisDerivedDataStrategy.h"
#include "SAMRAI/hier/Patch.h"
#include "SAMRAI/hier/PatchHierarchy.h"
#include "SAMRAI/hier/PatchLevel.h"
#include "SAMRAI/solv/CartesianRobinBcHelper.h"
#include "SAMRAI/solv/RobinBcCoefStrategy.h"
#include "SinusoidalFrontTagger.h"

using namespace SAMRAI;

/*!
 * @brief Class to test new PIND algorithm.
 */
class DLBGTest:
   public appu::VisDerivedDataStrategy
{

public:
   /*!
    * @brief Constructor.
    */
   DLBGTest(
      const std::string& object_name,
      const tbox::Dimension& dim,
      tbox::Pointer<hier::PatchHierarchy> patch_hierarchy,
      SAMRAI::tbox::Pointer<SAMRAI::tbox::Database> database);

   ~DLBGTest();

   mesh::StandardTagAndInitStrategy *
   getStandardTagAndInitObject();

   //@{ @name SAMRAI::appu::VisDerivedDataStrategy virtuals

   virtual bool
   packDerivedDataIntoDoubleBuffer(
      double* buffer,
      const hier::Patch& patch,
      const hier::Box& region,
      const std::string& variable_name,
      int depth_id) const;

   //@}

public:
   /*
    * Deallocate patch data allocated by this class.
    */
   void
   computeHierarchyData(
      hier::PatchHierarchy& hierarchy,
      double time);

   /*!
    * @brief Deallocate internally managed patch data on level.
    */
   void
   deallocatePatchData(
      hier::PatchLevel& level);

   /*!
    * @brief Deallocate internally managed patch data on hierarchy.
    */
   void
   deallocatePatchData(
      hier::PatchHierarchy& hierarchy);

#ifdef HAVE_HDF5
   /*!
    * @brief Tell a VisIt plotter which data to write for this class.
    */
   int
   registerVariablesWithPlotter(
      tbox::Pointer<appu::VisItDataWriter> writer);
#endif

   /*!
    * @brief Test refinement accuracy (assuming DLBG is used).
    */
   void
   checkDataRefinement(
      hier::PatchHierarchy& hierarchy);

private:
   std::string d_name;
   const tbox::Dimension d_dim;
   tbox::Pointer<hier::PatchHierarchy> d_hierarchy;

   SinusoidalFrontTagger d_tagger;

   /*!
    * @brief Front time.
    */
   double d_time;

};

#endif  // included_DLBGTest
