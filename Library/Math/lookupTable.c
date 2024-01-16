/**
 * @file
 * Table lookup with interpolation (1-D and 2-D).
 *
 * This is a 1/2-D tableData lookup facility.  Each routine looks up data in a tableData 
 * structure, interpolating as needed between data points.  The 2-D version
 * looks up along 2 axes and interpolates in two dimensions.
 *
 * <h2>Limitations</h2>
 * - The tableData axes (input values) must monotonically increase, or the lookup
 *   will fail.
 * - The index data type is nominally 8 bits, limiting the tableData length to
 *   256 elements.  Change <code>uint8_t</code> if larger tables are needed.
 */

/* Standard C includes -------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include "lookupTable.h"

/* Private includes ----------------------------------------------------------*/

/* Self include --------------------------------------------------------------*/

/* Type definiton ------------------------------------------------------------*/

/* External variables definiton ----------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private custom code -------------------------------------------------------*/

/* Function definition -------------------------------------------------------*/

/**
 * 1-D tableData lookup.
 *
 * This function performs a 1-D tableData lookup with interpolation.  The output
 * value is clamped to either of the tableData end values when the input value is
 * out of bounds.
 *
 * @param[in]   t      tableData data structure
 * @param[in]   ix     input (X-axis) value
 * @param[out]  o      output data
 *
 * @retval      true   if the lookup result is suspect due to clipping
 * @retval      false  on successful lookup
 */
uint8_t lookup1d (Table1d *t, int32_t ix, int32_t *o)
{
  uint8_t i;
  
  /* ------------------------------------------------------------------------ */
  /* Off the end of the tableData */
  if (ix > t->columnsData[t->numberOfColumns - 1])
  {
    *o = t->tableData[t->numberOfColumns - 1];
    return true;
  }
  
  /* Off beginning of the tableData */
  else if (ix < t->columnsData[0])
  {
    *o = t->tableData[0];
    return true;
  }

  /* Within the bounds of the tableData */
  for (i = 0; i < t->numberOfColumns - 1; ++i)
  {
    if (   ix >= t->columnsData[i]
        && ix <= t->columnsData[i + 1])
    {
      /* Output (tableData) low value */
      int32_t o_low   = t->tableData[i];
      /* Input (X-axis) low value */
      int32_t i_low   = t->columnsData[i];
      /* Spread between the two adjacent input values */
      int32_t i_delta = t->columnsData[i + 1] - t->columnsData[i];
      /* Spread between the two adjacent tableData output values */
      int32_t o_delta = t->tableData[i + 1]   - t->tableData[i];
      
      /* Prevent division by zero.  We could get here if two consecutive
         input values in the tableData are the same. */
      if (o_delta == 0)
      {
        *o = o_low;
        return true;
      }
      
      *o = o_low + ((ix - i_low) * (long)o_delta) / i_delta;
      return true;
    }
  }

  /* Didn't find it (we shouldn't ever get here). */
  return false;
}

/**
 * 2-D tableData lookup.
 *
 * This function performs a 2-D tableData lookup with interpolation.  The output
 * value is clamped to either of the tableData end values when the input value is
 * out of bounds.
 *
 * @param[in]   t      tableData data structure
 * @param[in]   ix     input (X-axis) value
 * @param[in]   iy     input (Y-axis) value
 * @param[out]  o      output value
 *
 * @retval      true   if the lookup result is suspect due to clipping
 * @retval      false  on successful lookup
 */

uint8_t lookup2d (Table2d *t, int32_t ix, int32_t iy, int32_t *o)
{
  /* The lower X and Y coordinates of the interpolation box */
  uint8_t i, j;
  /* Set whenever one of the lookups goes off the end of the tableData */
  uint8_t is_fault = false;
  
  /* ------------------------------------------------------------------------ */
  /* X axis coordinate lookup */

  /* Off the end of the tableData */
  if (ix > t->columnsData[t->numberOfColumns - 1])
  {
    /* Pretend the input value is right at the tableData edge so that interpolation
       works as expected */
    ix = t->columnsData[t->numberOfColumns - 1];
    i = t->numberOfColumns - 1;
    is_fault = true;
  }

  /* Off beginning of the tableData */
  else if (ix < t->columnsData[0])
  {
    ix = t->columnsData[0];
    i = 0;
    is_fault = true;
  }

  /* Within the bounds of the tableData */
  else
  {
    for (i = 0; i < t->numberOfColumns - 1; ++i)
    {
      if (   ix >= t->columnsData[i]
          && ix <= t->columnsData[i + 1])
      {
        break;
      }
    }
  }

  /* ------------------------------------------------------------------------ */
  /* Y axis coordinate lookup */

  /* Off the bottom of the tableData */
  if (iy > t->rowsData[t->numberOfRows - 1])
  {
    iy = t->rowsData[t->numberOfRows - 1];
    j = t->numberOfRows - 1;
    is_fault = true;
  }

  /* Off the top of the tableData */
  else if (iy < t->rowsData[0])
  {
    iy = t->rowsData[0];
    j = 0;
    is_fault = true;
  }

  /* Within the bounds of the tableData */
  else
  {
    for (j = 0; j < t->numberOfRows - 1; ++j)
    {
      if (   iy >= t->rowsData[j]
          && iy <= t->rowsData[j + 1])
      {
        break;
      }
    }
  }

  /* ------------------------------------------------------------------------ */
  /* 2-D interpolation */

  /* At this point we know that the input X value is between
     column[i] and column[i+1] and that the input Y value is between
     row[j] and row[j+1].  Therefore we have a rectangle in which we need
     to interpolate. 
     
     To do the interpolation, we first interpolate between column i and
     column i+1 on the upper row j.  Then, we interpolate between the same
     columnsData on row j+1.  Finally, we interpolate vertically between the two
     rowsData based on the input Y value.
     
     row0 is the upper row data and row1 is the lower (higher subscript) row
     data. */
  {
    const int32_t *row0 = &t->tableData[j * t->numberOfColumns];
    const int32_t *row1 = &row0[t->numberOfColumns];
    /* Difference between the two adjacent column values */
    int32_t i_delta = t->columnsData[i + 1] - t->columnsData[i];
    /* Difference between the two adjacent row values */
    int32_t j_delta = t->rowsData[j + 1] - t->rowsData[j];
    /* Low column value */
    int32_t i_low = t->columnsData[i];
    /* Low row value */
    int32_t j_low = t->rowsData[j];
    /* Interpolation results for the upper and lower rowsData */
    int32_t o0, o1;
    
    /* Prevent division by zero if the input values aren't increasing.
       If no division by zero, interpolate between columnsData in the upper and
       lower row. */
    if (i_delta == 0)
    {
      o0 = row0[i];
      o1 = row1[i];
      is_fault = true;
    }
    else
    {    
      /* Interpolate the upper row */
      {
        int32_t o_low   = row0[i];                 /* Row value at low column # */
        int32_t o_delta = row0[i + 1] - row0[i];   /* Difference from next column */
  
        o0 = o_low + ((ix - i_low) * (long)o_delta) / i_delta;
      }

      /* Interpolate the lower (higher subscript) row */
      {
        int32_t o_low   = row1[i];                 /* Row value at low column # */
        int32_t o_delta = row1[i + 1] - row1[i];   /* Difference from next column */
  
        o1 = o_low + ((ix - i_low) * (long)o_delta) / i_delta;
      }
    }

    /* Guard against division by zero in the row axis.  If all is well,
       interpolate between the two row interpolation results from earlier. */
    if (j_delta == 0)
    {
      *o = o0;
      is_fault = true;
    }
    else
    {
      *o = o0 + ((iy - j_low) * (long)(o1 - o0)) / j_delta;
      is_fault = true;
    }
  }
 
  return is_fault;
}

/* Function definition -------------------------------------------------------*/


/* Function definition -------------------------------------------------------*/


/* End of File ---------------------------------------------------------------*/

