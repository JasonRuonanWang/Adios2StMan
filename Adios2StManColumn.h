//    (c) Oak Ridge National Laboratory
//    1 Bethel Valley Road, Oak Ridge, TN 37830, United States
//
//    This library is free software: you can redistribute it and/or
//    modify it under the terms of the GNU General Public License as published
//    by the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this library. If not, see <http://www.gnu.org/licenses/>.
//
//    Any bugs, questions, concerns and/or suggestions please email to
//    wangr1@ornl.gov or jason.ruonan.wang@gmail.com

#ifndef ADIOSSTMANCOLUMN_H
#define ADIOSSTMANCOLUMN_H

#include "Adios2StMan.h"

#include <casacore/casa/Arrays/Array.h>
#include <casacore/tables/DataMan/StManColumn.h>

namespace casacore {

class Adios2StManColumn : public StManColumn {
public:
  Adios2StManColumn(Adios2StMan *aParent, int aDataType, uInt aColNr,
                    String aColName, adios2::IO aAdiosIO);

  virtual void create(uInt aNrRows, std::shared_ptr<adios2::Engine> aAdiosEngine)=0;
  virtual void setShapeColumn(const IPosition& aShape);
  virtual IPosition shape(uInt aRowNr);

  int getDataTypeSize();
  int getDataType();
  String getColumnName();

  // *** access a row for an array column ***
  virtual void getArrayV(uInt rownr, void *dataPtr);

  // *** access a slice of a row for an array column ***
  // *** inactive by default
  // *** only active when canAccessSlice() returns true in child class
  virtual void getSliceV(uInt aRowNr, const Slicer &ns, void *dataPtr);

  // *** access all rows for an array column ***
  // *** inactive by default
  // *** only active when canAccessArrayColumn() returns true in child class
  virtual void getArrayColumnV(void *dataPtr);

  // *** access a slice of all rows for an array column ***
  // *** inactive by default
  // *** only active when canAccessColumnSlice() returns true in child class
  virtual void getColumnSliceV(const Slicer &ns, void *dataPtr);

protected:
  void getArrayWrapper(uint64_t rowStart, uint64_t nrRows, const Slicer &ns,
                       void *dataPtr);
  virtual void getArrayCommonV(uint64_t rowStart, uint64_t nrRows,
                               const Slicer &ns, void *data);
  virtual void putArrayCommonV(uint64_t row, const void *data);

  Adios2StMan *itsStManPtr;

  String itsColumnName;
  char itsColumnType; // 's'-scalar, 'd'-direct array, 'i'-indirect array
  IPosition itsCasaShape;
  int itsDataTypeSize;
  int itsCasaDataType;

  adios2::IO itsAdiosIO;
  std::shared_ptr<adios2::Engine> itsAdiosEngine;
  std::string itsAdiosDataType;
  adios2::Dims itsAdiosShape;
  adios2::Dims itsAdiosSingleRowStart;
  adios2::Dims itsAdiosSingleRowCount;
};
}

#endif
