#include "Adios2StManColumn.h"

namespace casacore {

    DataManager* Adios2StMan::makeObject (const casa::String& aDataManType, const casa::Record& spec){
        return new Adios2StMan();
    }

    DataManager* Adios2StMan::clone() const
    {
        return new Adios2StMan;
    }

    String Adios2StMan::dataManagerType() const
    {
        return itsDataManName;
    }

    void Adios2StMan::addRow (uInt aNrRows){
    }

    void Adios2StMan::create (uInt aNrRows){
        itsNrRows = aNrRows;
        itsNrCols = ncolumn();
    }

    void Adios2StMan::open (uInt aNrRows, AipsIO& ios){
        ios.getstart(itsDataManName);
        ios >> itsDataManName;
        ios >> itsStManColumnType;
        ios.getend();

        itsNrRows = aNrRows;
    }

    void Adios2StMan::deleteManager(){
    }

    DataManagerColumn* Adios2StMan::makeScalarColumn (const String& name, int aDataType, const String& dataTypeId){
        return makeColumnMeta(name, aDataType, dataTypeId);
    }

    DataManagerColumn* Adios2StMan::makeDirArrColumn (const String& name, int aDataType, const String& dataTypeId){
        return makeColumnMeta(name, aDataType, dataTypeId);
    }

    DataManagerColumn* Adios2StMan::makeIndArrColumn (const String& name, int aDataType, const String& dataTypeId){
        return makeColumnMeta(name, aDataType, dataTypeId);
    }

    DataManagerColumn* Adios2StMan::makeColumnMeta (const String& name, int aDataType, const String& dataTypeId){
        if (ncolumn() >= itsColumnPtrBlk.nelements()) {
            itsColumnPtrBlk.resize (itsColumnPtrBlk.nelements() + 32);
        }
        Adios2StManColumn* aColumn = new Adios2StManColumn (this, aDataType, ncolumn());
        aColumn->setColumnName(name);
        itsColumnPtrBlk[ncolumn()] = aColumn;
        return aColumn;
    }

    uInt Adios2StMan::getNrRows(){
        return itsNrRows;
    }

    void Adios2StMan::resync (uInt aNrRows){
    }

    Bool Adios2StMan::flush (AipsIO& ios, Bool doFsync){
        ios.putstart(itsDataManName, 2);
        ios << itsDataManName;
        ios << itsStManColumnType;
        ios.putend();
        return true;
    }

    String Adios2StMan::dataManagerName() const
    {
        return itsDataManName;
    }

    void register_adiosstman(){
        DataManager::registerCtor ("Adios2StMan", Adios2StMan::makeObject);
    }
}

