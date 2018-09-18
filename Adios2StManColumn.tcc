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

#ifndef ADIOSSTMANCOLUMN_TCC
#define ADIOSSTMANCOLUMN_TCC

#include "Adios2StManColumn.h"

namespace casacore {
    template<class T> class Adios2StManColumnT : public Adios2StManColumn
    {
        public:
            Adios2StManColumnT(Adios2StMan *aParent, int aDataType, uInt aColNr,
                    String aColName, std::shared_ptr<adios2::IO> aAdiosIO)
                :Adios2StManColumn(aParent, aDataType, aColNr, aColName, aAdiosIO)
            {
            }
            void create(uInt aNrRows, std::shared_ptr<adios2::Engine> aAdiosEngine, char aOpenMode)
            {
                itsAdiosShape[0] = aNrRows;
                itsAdiosEngine = aAdiosEngine;
                itsAdiosVariable = itsAdiosIO->InquireVariable<T>(itsColumnName);
                if(!itsAdiosVariable && aOpenMode == 'w'){
                    itsAdiosVariable = itsAdiosIO->DefineVariable<T>(itsColumnName,
                            itsAdiosShape,
                            itsAdiosSingleRowStart,
                            itsAdiosSingleRowCount);
                }
            }
            virtual void putArrayV(uInt rownr, const void *dataPtr){
                Bool deleteIt;
                itsAdiosSingleRowStart[0] = rownr;
                itsAdiosVariable.SetSelection({itsAdiosSingleRowStart, itsAdiosSingleRowCount});
                const T *data = (reinterpret_cast<const Array<T> *>(dataPtr))->getStorage(deleteIt);
                itsAdiosEngine->Put(itsAdiosVariable, data);
                (reinterpret_cast<const Array<T> *>(dataPtr))->freeStorage(reinterpret_cast<const T *&>(data), deleteIt);
            }
            virtual void putV(uInt rownr, const void *dataPtr){
                itsAdiosSingleRowStart[0] = rownr;
                itsAdiosVariable.SetSelection({itsAdiosSingleRowStart, itsAdiosSingleRowCount});
                itsAdiosEngine->Put(itsAdiosVariable, reinterpret_cast<const T*>(dataPtr));
            }
            virtual void getArrayCommonV(uint64_t rowStart, uint64_t nrRows, const Slicer &ns, void *data) {
                itsAdiosSingleRowStart[0] = rowStart;
                itsAdiosSingleRowCount[0] = nrRows;
                for (int i=1; i<=itsAdiosShape.size(); i++){
                    itsAdiosSingleRowStart[i] = ns.start()(i-1);
                    itsAdiosSingleRowCount[i] = ns.length()(i-1);
                }
                itsAdiosVariable.SetSelection({itsAdiosSingleRowStart, itsAdiosSingleRowCount});
                itsAdiosEngine->Get<T>(itsAdiosVariable, reinterpret_cast<T*>(data), adios2::Mode::Sync);
            }
        private:
            adios2::Variable<T> itsAdiosVariable;
    };
}
#endif
