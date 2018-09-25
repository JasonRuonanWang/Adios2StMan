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

namespace casacore
{

Adios2StManColumn::Adios2StManColumn(Adios2StMan *aParent, int aDataType,
                                     uInt aColNr, String aColName,
                                     std::shared_ptr<adios2::IO> aAdiosIO)
: StManColumn(aDataType), itsStManPtr(aParent), itsCasaDataType(aDataType),
  itsCasaShape(0), itsAdiosIO(aAdiosIO), itsColumnName(aColName)
{
    itsAdiosShape.resize(1);
    itsAdiosStart.resize(1);
    itsAdiosCount.resize(1);
    itsAdiosCount[0] = 1;
    itsAdiosShape[0] = itsStManPtr->getNrRows();

}

String Adios2StManColumn::getColumnName() { return itsColumnName; }

void Adios2StManColumn::setShapeColumn(const IPosition &aShape)
{
    itsCasaShape = aShape;
    itsAdiosShape.resize(aShape.size() + 1);
    itsAdiosStart.resize(aShape.size() + 1);
    itsAdiosCount.resize(aShape.size() + 1);
    for (int i = 1; i < itsCasaShape.size() + 1; ++i)
    {
        itsAdiosShape[i] = itsCasaShape[i - 1];
        itsAdiosCount[i] = itsCasaShape[i - 1];
        itsAdiosStart[i] = 0;
    }
    itsAdiosStart[0] = 0;
    itsAdiosCount[0] = 1;
}

IPosition Adios2StManColumn::shape(uInt aRowNr) { return itsCasaShape; }

int Adios2StManColumn::getDataTypeSize() { return itsDataTypeSize; }

int Adios2StManColumn::getDataType() { return itsCasaDataType; }

// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------

void Adios2StManColumn::putBoolV(uInt rownr, const Bool *dataPtr)
{
    putScalarV(rownr, dataPtr);
}
void Adios2StManColumn::putuCharV(uInt rownr, const uChar *dataPtr)
{
    putScalarV(rownr, dataPtr);
}
void Adios2StManColumn::putShortV(uInt rownr, const Short *dataPtr)
{
    putScalarV(rownr, dataPtr);
}
void Adios2StManColumn::putuShortV(uInt rownr, const uShort *dataPtr)
{
    putScalarV(rownr, dataPtr);
}
void Adios2StManColumn::putIntV(uInt rownr, const Int *dataPtr)
{
    putScalarV(rownr, dataPtr);
}
void Adios2StManColumn::putuIntV(uInt rownr, const uInt *dataPtr)
{
    putScalarV(rownr, dataPtr);
}
void Adios2StManColumn::putfloatV(uInt rownr, const Float *dataPtr)
{
    putScalarV(rownr, dataPtr);
}
void Adios2StManColumn::putdoubleV(uInt rownr, const Double *dataPtr)
{
    putScalarV(rownr, dataPtr);
}
void Adios2StManColumn::putComplexV(uInt rownr, const Complex *dataPtr)
{
    putScalarV(rownr, dataPtr);
}
void Adios2StManColumn::putDComplexV(uInt rownr, const DComplex *dataPtr)
{
    putScalarV(rownr, dataPtr);
}
void Adios2StManColumn::putStringV(uInt rownr, const String *dataPtr)
{
    putScalarV(rownr, dataPtr);
}

void Adios2StManColumn::getBoolV(uInt rownr, Bool *dataPtr)
{
    getScalarV(rownr, dataPtr);
}
void Adios2StManColumn::getuCharV(uInt rownr, uChar *dataPtr)
{
    getScalarV(rownr, dataPtr);
}
void Adios2StManColumn::getShortV(uInt rownr, Short *dataPtr)
{
    getScalarV(rownr, dataPtr);
}
void Adios2StManColumn::getuShortV(uInt rownr, uShort *dataPtr)
{
    getScalarV(rownr, dataPtr);
}
void Adios2StManColumn::getIntV(uInt rownr, Int *dataPtr)
{
    getScalarV(rownr, dataPtr);
}
void Adios2StManColumn::getuIntV(uInt rownr, uInt *dataPtr)
{
    getScalarV(rownr, dataPtr);
}
void Adios2StManColumn::getfloatV(uInt rownr, Float *dataPtr)
{
    getScalarV(rownr, dataPtr);
}
void Adios2StManColumn::getdoubleV(uInt rownr, Double *dataPtr)
{
    getScalarV(rownr, dataPtr);
}
void Adios2StManColumn::getComplexV(uInt rownr, Complex *dataPtr)
{
    getScalarV(rownr, dataPtr);
}
void Adios2StManColumn::getDComplexV(uInt rownr, DComplex *dataPtr)
{
    getScalarV(rownr, dataPtr);
}
void Adios2StManColumn::getStringV(uInt rownr, String *dataPtr)
{
    getScalarV(rownr, dataPtr);
}

}
