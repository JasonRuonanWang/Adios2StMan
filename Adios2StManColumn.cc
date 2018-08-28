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

#include "Adios2StManColumn.h"

namespace casacore {

Adios2StManColumn::Adios2StManColumn(Adios2StMan *aParent, int aDataType,
                                     uInt aColNr, String aColName, std::shared_ptr<adios2::IO> aAdiosIO)
    : StManColumn(aDataType), itsStManPtr(aParent), itsCasaDataType(aDataType),
      itsCasaShape(0), itsAdiosIO(aAdiosIO), itsColumnName(aColName)
    {
        itsAdiosShape.resize(1);
        itsAdiosSingleRowStart.resize(1);
        itsAdiosSingleRowCount.resize(1);
    }


String Adios2StManColumn::getColumnName() { return itsColumnName; }

void Adios2StManColumn::setShapeColumn(const IPosition& aShape){
    itsCasaShape = aShape;
    itsAdiosShape.resize(aShape.size() + 1);
    itsAdiosSingleRowStart.resize(aShape.size() + 1);
    itsAdiosSingleRowCount.resize(aShape.size() + 1);
    for(int i=1; i<itsCasaShape.size() + 1; ++i){
        itsAdiosShape[i] = itsCasaShape[i-1];
        itsAdiosSingleRowCount[i] = itsCasaShape[i-1];
        itsAdiosSingleRowStart[i] = 0;
    }
    itsAdiosSingleRowStart[0] = 0;
    itsAdiosSingleRowCount[0] = 1;
}

IPosition Adios2StManColumn::shape(uInt aRowNr){
    return itsCasaShape;
}

int Adios2StManColumn::getDataTypeSize() { return itsDataTypeSize; }

int Adios2StManColumn::getDataType() { return itsCasaDataType; }

// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------

void Adios2StManColumn::putBoolV(uInt rownr, const Bool* dataPtr){
    putV(rownr, dataPtr);
}
void Adios2StManColumn::putCharV (uInt rownr, const Char* dataPtr){
    putV(rownr, dataPtr);
}
void Adios2StManColumn::putuCharV (uInt rownr, const uChar* dataPtr){
    putV(rownr, dataPtr);
}
void Adios2StManColumn::putShortV (uInt rownr, const Short* dataPtr){
    putV(rownr, dataPtr);
}
void Adios2StManColumn::putuShortV (uInt rownr, const uShort* dataPtr){
    putV(rownr, dataPtr);
}
void Adios2StManColumn::putIntV (uInt rownr, const Int* dataPtr){
    putV(rownr, dataPtr);
}
void Adios2StManColumn::putuIntV (uInt rownr, const uInt* dataPtr){
    putV(rownr, dataPtr);
}
void Adios2StManColumn::putfloatV (uInt rownr, const float* dataPtr){
    putV(rownr, dataPtr);
}
void Adios2StManColumn::putdoubleV (uInt rownr, const double* dataPtr){
    putV(rownr, dataPtr);
}
void Adios2StManColumn::putComplexV (uInt rownr, const Complex* dataPtr){
    putV(rownr, dataPtr);
}
void Adios2StManColumn::putDComplexV (uInt rownr, const DComplex* dataPtr){
    putV(rownr, dataPtr);
}
void Adios2StManColumn::putStringV (uInt rownr, const String* dataPtr){
    putV(rownr, dataPtr);
}


void Adios2StManColumn::getArrayCommonV(uint64_t rowStart, uint64_t nrRows,
                                        const Slicer &ns, void *data) {}

// *** access a row for an array column ***

void Adios2StManColumn::getArrayV(uInt rownr, void *dataPtr) {
  Slicer ns(IPosition(itsCasaShape.size(), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0), itsCasaShape);
  getArrayWrapper(rownr, 1, ns, dataPtr);
}

// *** access a slice of a row for an array column ***
// *** inactive by default
// *** only active when canAccessSlice() returns true in child class
void Adios2StManColumn::getSliceV(uInt aRowNr, const Slicer &ns,
                                  void *dataPtr) {
  getArrayWrapper(aRowNr, 1, ns, dataPtr);
}

// *** access all rows for an array column ***
// *** inactive by default
// *** only active when canAccessArrayColumn() returns true in child class
void Adios2StManColumn::getArrayColumnV(void *dataPtr) {
  Slicer ns(IPosition(itsCasaShape.size(), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0), itsCasaShape);
  getArrayWrapper(0, itsStManPtr->getNrRows(), ns, dataPtr);
}

// *** access a slice of all rows for an array column ***
// *** inactive by default
// *** only active when canAccessColumnSlice() returns true in child class
void Adios2StManColumn::getColumnSliceV(const Slicer &ns, void *dataPtr) {
  getArrayWrapper(0, itsStManPtr->getNrRows(), ns, dataPtr);
}

void Adios2StManColumn::getArrayWrapper(uint64_t rowStart, uint64_t nrRows,
                                        const Slicer &ns, void *dataPtr) {
  Bool deleteIt;
  void *data;
  switch (itsCasaDataType) {
  case TpArrayBool:
  case TpBool:
    data = ((Array<Bool> *)dataPtr)->getStorage(deleteIt);
    getArrayCommonV(rowStart, nrRows, ns, data);
    ((Array<Bool> *)dataPtr)->putStorage((Bool *&)data, deleteIt);
    break;
  case TpArrayChar:
  case TpChar:
    data = ((Array<Char> *)dataPtr)->getStorage(deleteIt);
    getArrayCommonV(rowStart, nrRows, ns, data);
    ((Array<Char> *)dataPtr)->putStorage((Char *&)data, deleteIt);
    break;
  case TpArrayUChar:
  case TpUChar:
    data = ((Array<uChar> *)dataPtr)->getStorage(deleteIt);
    getArrayCommonV(rowStart, nrRows, ns, data);
    ((Array<uChar> *)dataPtr)->putStorage((uChar *&)data, deleteIt);
    break;
  case TpArrayShort:
  case TpShort:
    data = ((Array<Short> *)dataPtr)->getStorage(deleteIt);
    getArrayCommonV(rowStart, nrRows, ns, data);
    ((Array<Short> *)dataPtr)->putStorage((Short *&)data, deleteIt);
    break;
  case TpArrayUShort:
  case TpUShort:
    data = ((Array<uShort> *)dataPtr)->getStorage(deleteIt);
    getArrayCommonV(rowStart, nrRows, ns, data);
    ((Array<uShort> *)dataPtr)->putStorage((uShort *&)data, deleteIt);
    break;
  case TpArrayInt:
  case TpInt:
    data = ((Array<Int> *)dataPtr)->getStorage(deleteIt);
    getArrayCommonV(rowStart, nrRows, ns, data);
    ((Array<Int> *)dataPtr)->putStorage((Int *&)data, deleteIt);
    break;
  case TpArrayUInt:
  case TpUInt:
    data = ((Array<uInt> *)dataPtr)->getStorage(deleteIt);
    getArrayCommonV(rowStart, nrRows, ns, data);
    ((Array<uInt> *)dataPtr)->putStorage((uInt *&)data, deleteIt);
    break;
  case TpArrayFloat:
  case TpFloat:
    data = ((Array<Float> *)dataPtr)->getStorage(deleteIt);
    getArrayCommonV(rowStart, nrRows, ns, data);
    ((Array<Float> *)dataPtr)->putStorage((Float *&)data, deleteIt);
    break;
  case TpArrayDouble:
  case TpDouble:
    data = ((Array<Double> *)dataPtr)->getStorage(deleteIt);
    getArrayCommonV(rowStart, nrRows, ns, data);
    ((Array<Double> *)dataPtr)->putStorage((Double *&)data, deleteIt);
    break;
  case TpArrayComplex:
  case TpComplex:
    data = ((Array<Complex> *)dataPtr)->getStorage(deleteIt);
    getArrayCommonV(rowStart, nrRows, ns, data);
    ((Array<Complex> *)dataPtr)->putStorage((Complex *&)data, deleteIt);
    break;
  case TpArrayDComplex:
  case TpDComplex:
    data = ((Array<DComplex> *)dataPtr)->getStorage(deleteIt);
    getArrayCommonV(rowStart, nrRows, ns, data);
    ((Array<DComplex> *)dataPtr)->putStorage((DComplex *&)data, deleteIt);
    break;
  }
}
}
