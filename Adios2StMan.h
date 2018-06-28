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

#ifndef HDF5STMAN_H
#define HDF5STMAN_H

#include <casacore/tables/DataMan/DataManager.h>
#include <casacore/tables/Tables/Table.h>
#include <casacore/casa/IO/AipsIO.h>

namespace casacore {

    class Adios2StManColumn;

    class Adios2StMan : public DataManager
    {
        public:
            Adios2StMan() = default;
            ~Adios2StMan() = default;
            virtual DataManager* clone() const;
            virtual String dataManagerType() const;
            virtual String dataManagerName() const;
            virtual void create (uInt aNrRows);
            virtual void open (uInt aRowNr, AipsIO&);
            virtual void resync (uInt aRowNr);
            virtual Bool flush (AipsIO&, Bool doFsync);
            DataManagerColumn* makeColumnMeta (const String& aName, int aDataType, const String& aDataTypeID);
            virtual DataManagerColumn* makeScalarColumn (const String& aName, int aDataType, const String& aDataTypeID);
            virtual DataManagerColumn* makeDirArrColumn (const String& aName, int aDataType, const String& aDataTypeID);
            virtual DataManagerColumn* makeIndArrColumn (const String& aName, int aDataType, const String& aDataTypeID);
            virtual void deleteManager();
            virtual void addRow (uInt aNrRows);
            static DataManager* makeObject (const casa::String& aDataManType, const casa::Record& spec);
            uInt getNrRows();

        private:
            String itsDataManName = "Adios2StMan";
            uInt itsNrRows;
            uInt itsNrCols;
            int itsStManColumnType;
            PtrBlock<Adios2StManColumn*> itsColumnPtrBlk;
            hid_t itsHdf5File;

    }; // end of class Adios2StMan


    extern "C" void register_adiosstman();
} // end of namespace casa

#endif
