/**
 * @file lookupTable.h
 * @author
 * @brief lookup Table with interpolation (1-D and 2-D) header file.
 * @version 0.1
 * @date
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef LOOKUPTABLE_H_
#define LOOKUPTABLE_H_

/* Standard C includes -------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

#define true 1
#define false 0

/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
/** One dimensional lookup tableData. */
typedef const struct
{
  /** Number of elements in the tableData.  This must be at least 2. */
  uint8_t numberOfColumns;
  /** List of input values. */
  const int32_t *columnsData;
  /** Table data (output values).  The output values list must have the same
      length as the input list. */
  const int32_t *tableData;
} Table1d;

/** Two dimensional lookup tableData. */
typedef struct
{
  /** Number of columnsData (X values) in the tableData.  Must be at least 2. */
  uint8_t numberOfColumns;
  /** Number of rowsData (Y values) in the tableData.  Must be at least 2. */
  uint8_t numberOfRows;
  /** X-axis input values list. */
  const int32_t *columnsData;
  /** Y-axis input values list. */
  const int32_t *rowsData;
  /** Table data.  This is an array of <code>columnsData</code>X<code>rowsData</code>,
      arranged in rowsData.  For example, <code>tableData[1]</code> is the second 
      column in the first row. */
  const int32_t *tableData;
} Table2d;

/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/* Prototypes */
uint8_t lookup1d (Table1d *t, int32_t ix, int32_t *o);
uint8_t lookup2d (Table2d *t, int32_t ix, int32_t iy, int32_t *o);


#endif
/* End of File ---------------------------------------------------------------*/

