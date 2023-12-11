// Copyright NVIDIA Corporation 2002-2005
// TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, THIS SOFTWARE IS PROVIDED
// *AS IS* AND NVIDIA AND ITS SUPPLIERS DISCLAIM ALL WARRANTIES, EITHER EXPRESS
// OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL NVIDIA OR ITS SUPPLIERS
// BE LIABLE FOR ANY SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES
// WHATSOEVER (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS,
// BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR ANY OTHER PECUNIARY LOSS)
// ARISING OUT OF THE USE OF OR INABILITY TO USE THIS SOFTWARE, EVEN IF NVIDIA HAS
// BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES 

#pragma once
/** \file */

#include "nvsgcommon.h" // stuff commonly required
#include "nvsg/Group.h" // base class definition
// additional dependencies
#include <set>

namespace nvsg
{
  /*! \brief Switch node.
   *  \par Namespace: nvsg
   *  \remarks A Switch node is a special kind of grouping node 
   *  where one can define which children will be traversed and which will not (active/inactive children).\n
   *  The Switch node in this case is a special node. It can be thought of as a Dip Switch where 
   *  you can turn on and off the traversal of several children at the same time. 
   *  \par Example
   *  \code
   *                 sw 
   *                 |
   *     -----------------------
   *     |      |       |      |
   *     c0     c1      c2     c3
   *  
   *  [...]
   *  sw->setInactive();  // set all children inactive
   *  sw->setActive(0);   // set child c0 active
   *  sw->setActive(3);   // set child c3 active
   *  [...]
   *  \endcode
   *  In this sample only C0 and C3 will be active and traversed.*/
  class Switch : public Group
  {
    public:
      /*! \brief Default-constructs a Switch. 
       */
      NVSG_API Switch();

      /*! \brief Constructs a Switch from a Group. 
      * \param
      * rhs Group to construct the Switch from.
      * \remarks
      * This is kind of a partial copy constructor only copying the Group part from the source 
      * object.
      */
      NVSG_API explicit Switch(const Group &rhs);

      /*! \brief Constructs a Switch as a copy of another Switch.
      */
      NVSG_API Switch(const Switch &rhs);

      /*! \brief Destructs a Switch.
       */
      NVSG_API virtual ~Switch();

      /*! \brief Set all children of this Switch active. 
       *  \remarks Make all children visible so all traversers traverse them.
       *  \sa setInactive */
      NVSG_API void setActive();

      /*! \brief Make the specified child of the Switch to active. 
       *  \param index Zero-based index of the child. Providing an invalid index will lead to 
       *  undefined behavior.
       *  \remarks This function makes the specified child visible so all traversers traverse it.\n
       *  Currently active children will stay active.
       *  \sa setInactive */
      NVSG_API void setActive(size_t index);

      /*! \brief Set all children of the Switch to inactive. 
       *  \remarks Make all children invisible so no traverser traverses them.
       *  \sa setActive, isActive */
      NVSG_API void setInactive();

      /*! \brief Make the specified child of the Switch to inactive. 
       *  \param index Zero-based index of the child. Providing an invalid index will lead to 
       *  undefined behavior.
       *  \remarks This function makes the specified child invisible so no traverser traverses it.\n
       *  Currently inactive children will stay inactive.
       *  \sa setInactive, isActive */
      NVSG_API void setInactive(size_t index);

      /*! \brief Get the number of active children.
       *  \return Number of active children. 
       *  \remarks This function determines the number of currently active children 
       *  under this Switch.
       *  \sa setActive, setInactive, isActive */
      NVSG_API size_t getNumberOfActive() const;

      /*! \brief Get indices to active child nodes.
       *  \param indices Reference to a vector that will hold the returned indices. 
       *  \return The number of indices copied to /a indices.
       *  \remarks Copies the indices of all active child nodes to the vector /a indices.\n
       *  The internal format a Switch uses for storing indices is size_t, which expands to
       *  32-bit for 32-bit environments and 64-bit for 64-bit environments. 
       *  \sa setActive, setInactive, isActive */
      NVSG_API size_t getActive(std::vector<size_t>& indices) const;

      /*! \brief Test if any of the children is active.
       *  \return This function returns true when at least one of the children is active. 
       *  \remarks This function can be used to test if this Switch has at least 
       *  one active child.
       *  \sa setActive, setInactive, isActive */
      NVSG_API bool isActive() const;

      /*! \brief Test if the specified child is active.
       *  \param index Zero-based index of the child.
       *  \return This function returns true when the specified child is active. 
       *  \remarks This function can be used to test if a specific child is active.
       *  \sa setActive, setInactive, isActive */
      NVSG_API bool isActive(size_t index) const;

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant Switch to copy from.
       *  \return A reference to the assigned Switch.
       *  \remarks The assignment operator performs a deep copy and calls 
       *  the assignment operator of Group.*/
      NVSG_API Switch & operator=(const Switch & rhs);

      /*! \brief Test for equivalence with an other Switch. 
       *  \param p A pointer to the Switch to test for equivalence with.
       *  \param ignoreNames Optional parameter to ignore the names of the objects; default is \c true.
       *  \param deepCompare Optional parameter to perform a deep comparsion; default is \c false.
       *  \return \c true if the Switch \a p is equivalent to \c this, otherwise \c false.
       *  \remarks If \a p and \c this are equivalent as a Group, they are equivalent
       *  if they have the same anti-aliasing state, stipple factor, stipple pattern, and width.
       *  \sa Group */
      NVSG_API virtual bool isEquivalent(const Object *p, bool ignoreNames, bool deepCompare) const;

    protected:
      /*! \brief Called from the framework immediately after a child has been removed from the Switch.
       *  \param index Zero-based position that previously has referenced the removed child node.
       *  \remarks This function takes care of updating the indices in the container that holds
       *  the information on the active children. Removing children from the switch may have influence 
       *  on the position of other children.
       *  \sa Group::postRemoveChild */
      NVSG_API virtual void postRemoveChild(size_t index);

      /*! \brief Called from the framework immediately after a child was added to the Switch.
       *  \param index Zero-based position where the child node was added.
       *  \remarks This function takes care of updating the indices in the container that holds 
       *  the information on the active children. Adding children to the switch may have influence 
       *  on the position of other children.
       *  \sa Group::postAddChild */
      NVSG_API virtual void postAddChild(size_t index);

      /*! \brief Called from the framework if re-calculation of the Group's bounding sphere 
       *  is required.
       *  \return The function returns a nvmath::Sphere3f that represents the actual bounding 
       *  sphere of this Switch. The bounding sphere encloses the active children of the Switch.
       *  \remarks The function calculates the bounding sphere by accumulating the bounding spheres 
       *  of all active child nodes.
       * \sa Group::calculateBoundingSphere, nvmath::Sphere3f */
      NVSG_API virtual nvmath::Sphere3f calculateBoundingSphere( void ) const;

      /*! \brief Determine the hints containment status of this Switch.
       *  \param hints An unsigned short whose bits encode the hints containment to determine.
       *  \return An unsigned short whose bits encode the hints that are to be determined and also
       *  set in this Switch or any of it's active children.
       *  \remarks This function is called by the framework, if \a getHintsContainment is called while the
       *  hint containment flags mark that information as invalid.
       *  \sa containsHints, invalidateHintsContainment, getHintsContainment */
      NVSG_API virtual unsigned short determineHintsContainment( unsigned short hints ) const;

      /*! \brief Called from the framework if re-determination of the transparency containment 
       *  cache is required.
       *  \return The function returns \c true if the Switch contains at least one child which 
       *  contains  StateSet, that causes the corresponding geometry to appear transparent. 
       *  Otherwise, the function returns \c false.
       *  \remarks The function iterates through the child nodes and for each node determines 
       *  transparency containment. 
       *  The iteration stops with the first child node found that contains at least 
       *  one StateSet, and that causes the corresponding geometry to appear transparent.  
       *  \sa Group::determineTransparencyContainment, StateSet::isTransparent */
      NVSG_API virtual bool determineTransparencyContainment( void ) const;

    private:
      /** Index set (ordered) into the vector of children that's currently in use */
      std::set<size_t> m_activeChildren;
  };
  
} //  namespace nvsg
