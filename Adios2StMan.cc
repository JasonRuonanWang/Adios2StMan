#include <casacore/casa/Containers/Record.h>
#include "Adios2StManColumn.tcc"

namespace casacore {

bool Adios2StMan::itsUsingMpi;
std::string Adios2StMan::itsAdiosEngineType;
adios2::Params Adios2StMan::itsAdiosEngineParams;
std::vector<adios2::Params> Adios2StMan::itsAdiosTransportParamsVec;

#ifdef HAVE_MPI
#warning "Adios2StMan compiled with MPI"

MPI_Comm Adios2StMan::itsMpiComm = MPI_COMM_WORLD;

Adios2StMan::Adios2StMan(MPI_Comm mpiComm)
    :DataManager()
{
    itsUsingMpi = true;
    itsMpiComm = mpiComm;
    std::string engineType;
    adios2::Params engineParams;
    std::vector<adios2::Params> transportParams;
    Adios2StManCommon(engineType, engineParams, transportParams);
}

Adios2StMan::Adios2StMan(
    MPI_Comm mpiComm,
    std::string engineType,
    std::map<std::string, std::string> engineParams,
    std::vector<std::map<std::string, std::string>> transportParams
    )
    :DataManager()
{
    itsUsingMpi = true;
    itsMpiComm = mpiComm;
    Adios2StManCommon(engineType, engineParams, transportParams);
}
#endif

Adios2StMan::Adios2StMan()
    :DataManager()
{
    itsUsingMpi = false;
    std::string engineType;
    adios2::Params engineParams;
    std::vector<adios2::Params> transportParams;
    Adios2StManCommon(engineType, engineParams, transportParams);
}

Adios2StMan::Adios2StMan(
    std::string engineType,
    std::map<std::string, std::string> engineParams,
    std::vector<std::map<std::string, std::string>> transportParams)
    :DataManager()
{
    itsUsingMpi = false;
    Adios2StManCommon(engineType, engineParams, transportParams);
}

Adios2StMan::~Adios2StMan()
{
    if(itsAdiosEngine){
        itsAdiosEngine->EndStep();
        itsAdiosEngine->Close();
    }
}

void Adios2StMan::Adios2StManCommon(const std::string &engineType,
        const std::map<std::string, std::string> &engineParams,
        const std::vector<std::map<std::string, std::string>> &transportParams) {

    itsAdiosEngineType = engineType;
    itsAdiosEngineParams = engineParams;
    itsAdiosTransportParamsVec = transportParams;

    if(Adios2StMan::itsUsingMpi){
#ifdef HAVE_MPI
        itsAdios = std::make_shared<adios2::ADIOS> (Adios2StMan::itsMpiComm, true);
#else
        throw(std::runtime_error("Adios2StMan using MPI but HAVE_MPI is not defined. This should never happen"));
        itsAdios = std::make_shared<adios2::ADIOS> (true);
#endif
    }
    else{
        itsAdios = std::make_shared<adios2::ADIOS> (true);
    }

    itsAdiosIO = std::make_shared<adios2::IO> (itsAdios->DeclareIO("Adios2StMan"));

    if (itsAdiosEngineType.empty() == false) {
        itsAdiosIO->SetEngine(itsAdiosEngineType);
    }
    if (itsAdiosEngineParams.empty() == false) {
        itsAdiosIO->SetParameters(itsAdiosEngineParams);
    }
    for (size_t i = 0; i < itsAdiosTransportParamsVec.size(); ++i) {
        std::string transportName = std::to_string(i);
        auto j = itsAdiosTransportParamsVec[i].find("Name");
        if (j != itsAdiosTransportParamsVec[i].end()) {
            transportName = j->second;
        }
        itsAdiosIO->AddTransport(transportName, itsAdiosTransportParamsVec[i]);
    }
}

DataManager *Adios2StMan::makeObject(const casa::String &aDataManType,
                                     const casa::Record &spec) {
    if(Adios2StMan::itsUsingMpi){
#ifdef HAVE_MPI
        return new Adios2StMan(itsMpiComm, itsAdiosEngineType, itsAdiosEngineParams, itsAdiosTransportParamsVec);
#else
        throw(std::runtime_error("Adios2StMan using MPI but HAVE_MPI is not defined. This should never happen"));
        return new Adios2StMan(itsAdiosEngineType, itsAdiosEngineParams, itsAdiosTransportParamsVec);
#endif
    }
    else{
        return new Adios2StMan(itsAdiosEngineType, itsAdiosEngineParams, itsAdiosTransportParamsVec);
    }
}

DataManager *Adios2StMan::clone() const
{
    Record tmp;
    return makeObject(itsDataManName, tmp);
}

String Adios2StMan::dataManagerType() const { return itsDataManName; }

void Adios2StMan::addRow(uInt aNrRows) {}

void Adios2StMan::create(uInt aNrRows) {
  itsNrRows = aNrRows;
  itsAdiosEngine = std::make_shared<adios2::Engine>(itsAdiosIO->Open(fileName(), adios2::Mode::Write));
  for(int i=0; i<ncolumn(); ++i){
      itsColumnPtrBlk[i]->create(aNrRows, itsAdiosEngine);
  }
  itsAdiosEngine->BeginStep();
}

void Adios2StMan::open(uInt aNrRows, AipsIO &ios) {
  ios.getstart(itsDataManName);
  ios >> itsDataManName;
  ios >> itsStManColumnType;
  ios.getend();

  itsNrRows = aNrRows;
}

void Adios2StMan::deleteManager() {}

DataManagerColumn *Adios2StMan::makeScalarColumn(const String &name,
                                                 int aDataType,
                                                 const String &dataTypeId) {
  return makeColumnCommon(name, aDataType, dataTypeId);
}

DataManagerColumn *Adios2StMan::makeDirArrColumn(const String &name,
                                                 int aDataType,
                                                 const String &dataTypeId) {
  return makeColumnCommon(name, aDataType, dataTypeId);
}

DataManagerColumn *Adios2StMan::makeIndArrColumn(const String &name,
                                                 int aDataType,
                                                 const String &dataTypeId) {
  return makeColumnCommon(name, aDataType, dataTypeId);
}

DataManagerColumn *Adios2StMan::makeColumnCommon(const String &name,
                                                 int aDataType,
                                                 const String &dataTypeId) {
  if (ncolumn() >= itsColumnPtrBlk.nelements()) {
    itsColumnPtrBlk.resize(itsColumnPtrBlk.nelements() + 32);
  }
  Adios2StManColumn *aColumn;
  switch (aDataType){
      case TpBool:
      case TpArrayBool:
          aColumn = new Adios2StManColumnT<unsigned char>(this, aDataType, ncolumn(), name, itsAdiosIO);
          break;
      case TpChar:
      case TpArrayChar:
          aColumn = new Adios2StManColumnT<char>(this, aDataType, ncolumn(), name, itsAdiosIO);
          break;
      case TpUChar:
      case TpArrayUChar:
          aColumn = new Adios2StManColumnT<unsigned char>(this, aDataType, ncolumn(), name, itsAdiosIO);
          break;
      case TpShort:
      case TpArrayShort:
          aColumn = new Adios2StManColumnT<short>(this, aDataType, ncolumn(), name, itsAdiosIO);
          break;
      case TpUShort:
      case TpArrayUShort:
          aColumn = new Adios2StManColumnT<unsigned short>(this, aDataType, ncolumn(), name, itsAdiosIO);
          break;
      case TpInt:
      case TpArrayInt:
          aColumn = new Adios2StManColumnT<int>(this, aDataType, ncolumn(), name, itsAdiosIO);
          break;
      case TpUInt:
      case TpArrayUInt:
          aColumn = new Adios2StManColumnT<unsigned int>(this, aDataType, ncolumn(), name, itsAdiosIO);
          break;
      case TpFloat:
      case TpArrayFloat:
          aColumn = new Adios2StManColumnT<float>(this, aDataType, ncolumn(), name, itsAdiosIO);
          break;
      case TpDouble:
      case TpArrayDouble:
          aColumn = new Adios2StManColumnT<double>(this, aDataType, ncolumn(), name, itsAdiosIO);
          break;
      case TpComplex:
      case TpArrayComplex:
          aColumn = new Adios2StManColumnT<Complex>(this, aDataType, ncolumn(), name, itsAdiosIO);
          break;
      case TpDComplex:
      case TpArrayDComplex:
          aColumn = new Adios2StManColumnT<std::complex<double>>(this, aDataType, ncolumn(), name, itsAdiosIO);
          break;
      case TpString:
      case TpArrayString:
          aColumn = new Adios2StManColumnT<std::string>(this, aDataType, ncolumn(), name, itsAdiosIO);
          break;
  }
  itsColumnPtrBlk[ncolumn()] = aColumn;
  return aColumn;
}

uInt Adios2StMan::getNrRows() { return itsNrRows; }

void Adios2StMan::resync(uInt aNrRows) {}

Bool Adios2StMan::flush(AipsIO &ios, Bool doFsync) {
  ios.putstart(itsDataManName, 2);
  ios << itsDataManName;
  ios << itsStManColumnType;
  ios.putend();
  return true;
}

String Adios2StMan::dataManagerName() const { return itsDataManName; }

void register_adiosstman() {
  DataManager::registerCtor("Adios2StMan", Adios2StMan::makeObject);
}
}
