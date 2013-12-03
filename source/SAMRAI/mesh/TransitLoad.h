/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright
 * information, see COPYRIGHT and COPYING.LESSER.
 *
 * Copyright:     (c) 1997-2013 Lawrence Livermore National Security, LLC
 * Description:   Container of loads for TreeLoadBalancer.
 *
 ************************************************************************/

#ifndef included_mesh_TransitLoad
#define included_mesh_TransitLoad

#include "SAMRAI/SAMRAI_config.h"
#include "SAMRAI/hier/BoxContainer.h"
#include "SAMRAI/hier/BoxLevel.h"
#include "SAMRAI/hier/MappingConnector.h"
#include "SAMRAI/tbox/MessageStream.h"

#include <iostream>
#include <string>

namespace SAMRAI {
namespace mesh {


/*!
 * @brief Base class for container of work in transit through the tree
 * in the TreeLoadBalancer algorithm.
 *
 * TransitLoad base class follows the prototype design pattern.
 * Subclasses must implement clone(), initialize(), and a copy
 * constructor according to this design pattern.
 *
 * TransitLoad has dual responsibilities.
 *
 * - First, it's a container of loads that move around the tree.  The
 * implementation may represent the work however it wants and should
 * be able to shift load from one container to another.  See
 * adjustLoad().
 *
 * - Second, it generates the mappings between the pre- and
 * post-balance BoxLevels.  See
 * assignContentsToLocalProcessAndPopulateMaps().  It must have this
 * responsibility because the implementation alone knows how the work
 * is represented.
 *
 * @see mesh::TreeLoadBalancer
 */

class TransitLoad {

public:

   TransitLoad();

   TransitLoad( const TransitLoad &other );

   virtual ~TransitLoad() {}

   //@{
   //! @name Methods required by prototype design pattern

   //! @brief Clone object according to design pattern.
   virtual boost::shared_ptr<TransitLoad> clone() const = 0;

   //! @brief Initialize object according to design pattern.
   virtual void initialize() = 0;

   //@}


   //@{
   //! @brief Container-like characteristics

   //! @brief Return the total load contained.
   virtual double getSumLoad() const = 0;

   //! @brief Insert all boxes from the given BoxContainer.
   virtual void insertAll( const hier::BoxContainer &other ) = 0;

   /*!
    * @brief Insert all boxes from the given TransitSet.
    *
    * @param other [i] Other TransitLoad container whose
    * implementation matches this one.
    */
   virtual void insertAll( const TransitLoad &other ) = 0;

   //! @brief Return number of items in this container.
   virtual size_t getNumberOfItems() const = 0;

   //! @brief Return number of processes contributing to the contents.
   virtual size_t getNumberOfOriginatingProcesses() const = 0;

   //! @brief Whether container is empty.
   virtual bool empty() const {
      return getNumberOfItems() == 0;
   }

   //@}

   //@{
   //! @name Packing/unpacking for communication.

   virtual void putToMessageStream( tbox::MessageStream &msg ) const = 0;

   virtual void getFromMessageStream( tbox::MessageStream &msg ) = 0;
   //@}


   /*!
    * @brief Adjust the load in this TransitSet by moving work
    * between it and another TransitSet.
    *
    * @param[in,out] hold_bin Holding bin for reserve load.  hold_bin
    * implementation is guaranteed to match this one.
    *
    * @param[in] ideal_load The load that this bin should have.
    *
    * @param[in] low_load Return when this bin's load is in the range
    * [low_load,high_load]
    *
    * @param[in] high_load Return when this bin's load is in the range
    * [low_load,high_load]
    *
    * @return Net load added to this TransitSet.  If negative, load
    * decreased.
    */
   virtual double
   adjustLoad(
      TransitLoad& hold_bin,
      double ideal_load,
      double low_load,
      double high_load ) = 0;


   /*!
    * @brief Assign contents to local process and generate
    * balanced<==>unbalanced map.
    *
    * This method uses communication to set up the map.
    *
    * @param balanced_box_level Empty BoxLevel to populate with the
    * contents of this TransitLoad.
    *
    * @param balanced_to_unalanced Empty Connector to populate with
    * the balanced--->unbalanced edges.
    *
    * @param unbalanced_to_alanced Empty Connector to populate with
    * the unbalanced--->balanced edges.
    */
   virtual void
   assignContentToLocalProcessAndPopulateMaps(
      hier::BoxLevel& balanced_box_level,
      hier::MappingConnector &balanced_to_unbalanced,
      hier::MappingConnector &unbalanced_to_balanced ) = 0;


   //! @brief Whether to allow box breaking.
   void setAllowBoxBreaking( bool allow_box_breaking ) {
      d_allow_box_breaking = allow_box_breaking;
   }

   bool getAllowBoxBreaking() const {
      return d_allow_box_breaking;
   }


   //@{
   //! @name Diagnostic support

   //! @brief Set prefix of internal diagnostic timer names.
   virtual void
   setTimerPrefix(
      const std::string& ) {}

   //! @brief Object printing method to aid in debugging.
   virtual void recursivePrint(
      std::ostream &co=tbox::plog,
      const std::string &border=std::string(),
      int detail_depth=1 ) const;

   //@}

private:

   bool d_allow_box_breaking;

};


}
}

#endif
