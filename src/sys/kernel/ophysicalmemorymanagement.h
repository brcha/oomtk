#ifndef __OOMTKSYS_OPHYSICALMEMORYMANAGEMENT_H__
#define __OOMTKSYS_OPHYSICALMEMORYMANAGEMENT_H__
/*
 *  Copyright (C) 2008 by Filip Brcic <brcha@gna.org>
 *
 *  This file is part of OOMTK
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/** @file ophysicalmemorymanagement.h
 * @brief Physical memory management
 * 
 * Allocations of physical memory are guaranteed to be physically contigous, yet a physical region
 * is not guaranteed to be virtually mapped.
 */

#include <config.h>
#include <macros.h>
#include <types.h>
#include <cstddef>

#include <OOMTK/OSpinLock>

/**
 */
class OPhysicalMemoryManagement
{
  public:
    /**
     * @brief Get only one instance of this class
     * @returns the instance
     */
    static OPhysicalMemoryManagement * instance();
    
  protected:
    OPhysicalMemoryManagement();
    
  public:
    ~OPhysicalMemoryManagement();
    
  public:
    // data types
    /**
     * @brief Description of a physical memory allocation constraint
     * 
     * There are three general memory constraints: byte, word and page aligned.
     */
    struct constraint_t
    {
      kpa_t     base;
      kpa_t     bound;
      unsigned  align;
      bool      fromTop;  /* allocate from top or bottom of the region */
    };
    
    /**
     * @brief Classes of memory regions.
     * 
     * @warning Unused must be 0 since the initial vector is cleared in BSS.
     */
    enum Class
    {
      cls_Unused      = 0,      ///< @brief Unused (free) physical memory region
      cls_Addressable,          /**< @brief Undefined region of the addressable phys.mem. range.
       *
       * The initial memory hole is ADDRESSABLE range consisting of the
       * entire addressable physical memory space. All other regions branch
       * from that initial region (by splitting).
			     */
      cls_RAM,                  ///< @brief Allocatable RAM region
      cls_NVRAM,                ///< @brief Non-volatile RAM region
      cls_ROM,                  ///< @brief ROM region
      cls_Device,               ///< @brief Mmaped hardware device
    };
    
    /**
     * @brief Physical memory usage types
     * 
     * These describe the different types of uses for a defined physical memory region.
     * 
     * Regions with use-type Device or DevicePages are not mergeable since they can be
     * deleted with device removal. Other regions are not deallocatable and can therefore
     * be merged if they have the same class.
     * 
     * @warning Available must be 0 as it is cleared with BSS
     */
    enum Use
    {
      use_Available     = 0,    ///< @brief Allocatable RAM region
      
      // the following are not deallocatable
      use_BIOS,                 ///< @brief BIOS-owned region
      use_ACPI_NVS,             ///< @brief ACPI non-volatile store (ia32)
      use_ACPI_DATA,            ///< @brief ACPI data region (ia32)
      use_Kernel,               ///< @brief Owned by the kernel
      use_ISLImage,             ///< @brief Initial system load image (ie multiboot modules)
      use_KHeap,                ///< @brief Kernel heap backing frame
      use_KMap,                 ///< @brief Kernel mapping frame
      use_Pages,                ///< @brief Used by page space
      use_PageTable,            ///< @brief Used by page tables or directories
      use_SMP,                  ///< @brief SMP bootstrap pages
      
      // the following are not mergeable because they can be deleted
      use_Device,               ///< @brief Region owned by a device
      use_DevicePage,           ///< @brief Device page space
    };
    
    /**
     * @brief Descriptor for a physically contiguous region of physical memory.
     */
    struct info_t
    {
      kpa_t         base;         ///< @brief inclusive start of this range
      kpa_t         bound;        ///< @brief exclusive bound of this range
      Class         cls;          ///< @brief class of this range
      Use           use;          ///< @brief use of this range
      
      const char *  description;  ///< @brief Description of the range (not used, just for debugging)
    };
    
    enum {
      AllocFail     = (~((kpa_t) 0)),   ///< @brief Inform of failed allocation
    };
    
  public:
    /**
     * @brief Descibe the memory class
     * @param cls memory class
     * @returns the string describing the class
     */
    const char * describeClass(Class cls);
    
    /**
     * @brief Describe the memory use
     * @param use memory use
     * @returns the string describing the use
     */
    const char * describeUse(Use use);
    
    /**
     * @brief Create the initial memory hole
     * @param base address of the physical memory
     * @param bound of the physical memory
     */
    void initialize(kpa_t base, kpa_t bound);
    
    /**
     * @brief Allocate the region
     * @param base address of the region
     * @param bound of the region
     * @param cls class
     * @param use use
     * @param description debugging descrition
     * @returns the base address (or AllocFail if failed)
     */
    kpa_t allocRegion(kpa_t base, kpa_t bound, Class cls, Use use, const char * description);
    
    /**
     * @brief Allocate some amount of contigous memory and conform to the constraint.
     * @param constraint to conform to
     * @param size of the memory region
     * @param use use
     * @param description debugging description
     * @returns the base address of the region
     */
    kpa_t allocBytes(const constraint_t & constraint, size_t size, Use use, const char * description);
    
    /**
     * @brief Return the number of available units of size @p unitSize
     * @param constraint 
     * @param unitSize 
     * @param contiguous if true, return the maximum number of available units of the specified size.
     * @returns the number of available units
     */
    kpsize_t available(const constraint_t & constraint, kpsize_t unitSize, bool contiguous);
    
    /**
     * @brief Find the first region that contains [base,bound)
     * @param base of the wanted region
     * @param bound of the wanted region
     * @param ignoreAvail whether to ignore already allocated regions that are Available
     * @returns the newly found region
     */
    info_t * overlapsRegion(kpa_t base, kpa_t bound, bool ignoreAvail);
    
    /**
     * @brief Check if [base,bound) overlaps an allocated region
     * @param base address
     * @param bound of the region
     * @returns 
     */
    inline info_t * overlapsAllocatedRegion(kpa_t base, kpa_t bound)
    {
      return overlapsRegion(base, bound, true);
    }
    
    /**
     * @brief Find a region containing the address
     * @param addr to find a region with
     * @returns a region containing the address @c addr
     * 
     * @note this might return an available unallocated region
     */
    inline info_t * findRegion(kpa_t addr)
    {
      return overlapsRegion(addr, addr+1, false);
    }
    
    /**
     * @brief Dump the current region list to console for debugging (or some other purpose)
     */
    void showAll();
    
  protected:
    /**
     * @brief Cleanup the region list
     * 
     * This method merges what is mergeable
     */
    void cleanup();
    
    /**
     * @brief Decide which region is preferred to be allocated.
     * @param reg1 first region
     * @param reg2 second region
     * 
     * Mostly this method doesn't have a sense, but it is nicer to have region ordering in
     * some way. Also, some legacy DMA controllers tend to prefer allocation out of higher
     * memory due to restricted addressable bounds, though, I don't think that would be met
     * by this uKernel.
     */
    info_t * preferedRegion(info_t * reg1, info_t * reg2);
    
    /**
     * @brief Choose a region from which to allocate @c bytes of RAM while satisfying the constraint.
     * @param bytes size of the region
     * @param constraint to be satisfied
     * @returns the region
     */
    info_t * chooseRegion(size_t bytes, const constraint_t & constraint);
    
    /**
     * @brief Add a new physical memory region to the region list
     * @param base of the region
     * @param bound of the region
     * @param cls class of the region
     * @param use of the region
     * @param description of the region
     * @returns the allocated region
     */
    info_t * newRegion(kpa_t base, kpa_t bound, Class cls, Use use, const char * description);
    
    /**
     * @brief Check if two regions are mergeable
     * @param use of both regions
     * @returns true if mergeable
     */
    inline bool mergeable(Use use)
    {
      if (use == use_Device) return false;
      if (use == use_DevicePage) return false;
      return true;
    }
    
    /**
     * @brief Check of two regions have same descriptions
     * @param s1 description of the first region
     * @param s2 description of the second region
     * @returns true if they are the same
     */
    inline bool sameDescription(const char * s1, const char * s2);
    
  public:
    // Constraints
    constraint_t    needPages;
    constraint_t    needWords;
    constraint_t    needBytes;
    
  protected:
    info_t   m_table[PHYSMEM_NREGIONS];
    unsigned m_nInfo;
    
    OSpinLock m_lock;
};

/**
 * @brief Comparison function for re-sorting the physical memory regions structures.
 * @param obj1 the first region
 * @param obj2 the second region
 * @return -1 for less, 0 for equal and 1 for more
 * 
 * @note Entries of cls_Unused are sorted higher so that they end up on the top of the vector.
 */
bool operator > (OPhysicalMemoryManagement::info_t& obj1, OPhysicalMemoryManagement::info_t& obj2);
bool operator < (OPhysicalMemoryManagement::info_t& obj1, OPhysicalMemoryManagement::info_t& obj2);
bool operator == (OPhysicalMemoryManagement::info_t& obj1, OPhysicalMemoryManagement::info_t& obj2);
bool operator >= (OPhysicalMemoryManagement::info_t& obj1, OPhysicalMemoryManagement::info_t& obj2);
bool operator <= (OPhysicalMemoryManagement::info_t& obj1, OPhysicalMemoryManagement::info_t& obj2);

#endif /* __OOMTKSYS_OPHYSICALMEMORYMANAGEMENT_H__ */
