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

// ################################################################################
// This code creates separate casa table files for each MPI process.
// In order to fool the casa table system as if different processes are
// dealing with independent casa tables so that one process won't lock
// the table and/or prevent others from putting data in, only the master MPI process
// creates casa table files in the intended place, while slave processes
// create table files with exactly the same contents in /tmp, which are supposed
// to be cleaned up once the job is finished.
//
// However, the AdiosStMan objects associated with these casa tables are actually all
// writing data into a single ADIOS container, which is in the same directory where
// the master MPI process writes casa table files. Therefore, after job is finished,
// these table files together with ADIOS files will contain all information that is
// necessary to reproduce the casa table.

#include "../Adios2StMan.h"
#include <casacore/tables/Tables/TableDesc.h>
#include <casacore/tables/Tables/SetupNewTab.h>
#include <casacore/tables/Tables/ScaColDesc.h>
#include <casacore/tables/Tables/ScalarColumn.h>
#include <casacore/tables/Tables/ArrColDesc.h>
#include <casacore/tables/Tables/ArrayColumn.h>
#include <casacore/casa/namespace.h>
#include <mpi.h>

#include "common.h"

int main(int argc, char **argv){

    int mpiRank, mpiSize;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpiRank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpiSize);

    std::string filename;
    if (argc < 2){
        filename = "aaa.table";
    }
    else{
        filename = argv[1];
    }

    Adios2StMan *stman = new Adios2StMan(MPI_COMM_WORLD);

    int NrRows = mpiSize;

    IPosition array_pos = IPosition(2,5,6);
    Array<Bool> arr_Bool(array_pos);
    Array<Char> arr_Char(array_pos);
    Array<uChar> arr_uChar(array_pos);
    Array<Short> arr_Short(array_pos);
    Array<uShort> arr_uShort(array_pos);
    Array<Int> arr_Int(array_pos);
    Array<uInt> arr_uInt(array_pos);
    Array<Float> arr_Float(array_pos);
    Array<Double> arr_Double(array_pos);
    Array<Complex> arr_Complex(array_pos);
    Array<DComplex> arr_DComplex(array_pos);

    TableDesc td("", "1", TableDesc::Scratch);
    td.addColumn (ScalarColumnDesc<Bool>("scalar_Bool"));
    td.addColumn (ScalarColumnDesc<Char>("scalar_Char"));
    td.addColumn (ScalarColumnDesc<uChar>("scalar_uChar"));
    td.addColumn (ScalarColumnDesc<Short>("scalar_Short"));
    td.addColumn (ScalarColumnDesc<uShort>("scalar_uShort"));
    td.addColumn (ScalarColumnDesc<Int>("scalar_Int"));
    td.addColumn (ScalarColumnDesc<uInt>("scalar_uInt"));
    td.addColumn (ScalarColumnDesc<Float>("scalar_Float"));
    td.addColumn (ScalarColumnDesc<Double>("scalar_Double"));
    td.addColumn (ScalarColumnDesc<Complex>("scalar_Complex"));
    td.addColumn (ScalarColumnDesc<DComplex>("scalar_DComplex"));

    td.addColumn (ArrayColumnDesc<Bool>("array_Bool", array_pos, ColumnDesc::FixedShape));
    td.addColumn (ArrayColumnDesc<Char>("array_Char", array_pos, ColumnDesc::FixedShape));
    td.addColumn (ArrayColumnDesc<uChar>("array_uChar", array_pos, ColumnDesc::FixedShape));
    td.addColumn (ArrayColumnDesc<Short>("array_Short", array_pos, ColumnDesc::FixedShape));
    td.addColumn (ArrayColumnDesc<uShort>("array_uShort", array_pos, ColumnDesc::FixedShape));
    td.addColumn (ArrayColumnDesc<Int>("array_Int", array_pos, ColumnDesc::FixedShape));
    td.addColumn (ArrayColumnDesc<uInt>("array_uInt", array_pos, ColumnDesc::FixedShape));
    td.addColumn (ArrayColumnDesc<Float>("array_Float", array_pos, ColumnDesc::FixedShape));
    td.addColumn (ArrayColumnDesc<Double>("array_Double", array_pos, ColumnDesc::FixedShape));
    td.addColumn (ArrayColumnDesc<Complex>("array_Complex", array_pos, ColumnDesc::FixedShape));
    td.addColumn (ArrayColumnDesc<DComplex>("array_DComplex", array_pos, ColumnDesc::FixedShape));

    SetupNewTable newtab(filename, td, Table::New);
    newtab.bindAll(*stman);
    Table *tab = new Table(MPI_COMM_WORLD, newtab, NrRows);

    ScalarColumn<Bool> scalar_Bool (*tab, "scalar_Bool");
    ScalarColumn<Char> scalar_Char (*tab, "scalar_Char");
    ScalarColumn<uChar> scalar_uChar (*tab, "scalar_uChar");
    ScalarColumn<Short> scalar_Short (*tab, "scalar_Short");
    ScalarColumn<uShort> scalar_uShort (*tab, "scalar_uShort");
    ScalarColumn<Int> scalar_Int (*tab, "scalar_Int");
    ScalarColumn<uInt> scalar_uInt (*tab, "scalar_uInt");
    ScalarColumn<Float> scalar_Float (*tab, "scalar_Float");
    ScalarColumn<Double> scalar_Double (*tab, "scalar_Double");
    ScalarColumn<Complex> scalar_Complex (*tab, "scalar_Complex");
    ScalarColumn<DComplex> scalar_DComplex (*tab, "scalar_DComplex");

    ArrayColumn<Bool> array_Bool (*tab, "array_Bool");
    ArrayColumn<Char> array_Char (*tab, "array_Char");
    ArrayColumn<uChar> array_uChar (*tab, "array_uChar");
    ArrayColumn<Short> array_Short (*tab, "array_Short");
    ArrayColumn<uShort> array_uShort (*tab, "array_uShort");
    ArrayColumn<Int> array_Int (*tab, "array_Int");
    ArrayColumn<uInt> array_uInt (*tab, "array_uInt");
    ArrayColumn<Float> array_Float (*tab, "array_Float");
    ArrayColumn<Double> array_Double (*tab, "array_Double");
    ArrayColumn<Complex> array_Complex (*tab, "array_Complex");
    ArrayColumn<DComplex> array_DComplex (*tab, "array_DComplex");

    scalar_Bool.put (mpiRank, mpiRank);
    scalar_uChar.put (mpiRank, mpiRank);
    scalar_Short.put (mpiRank, mpiRank);
    scalar_uShort.put (mpiRank, mpiRank);
    scalar_Int.put (mpiRank, mpiRank);
    scalar_uInt.put (mpiRank, mpiRank);
    scalar_Float.put (mpiRank, mpiRank);
    scalar_Double.put (mpiRank, mpiRank);
    scalar_Complex.put (mpiRank, mpiRank);
    scalar_DComplex.put (mpiRank, mpiRank);
    GenData(arr_Bool, array_pos, mpiRank); array_Bool.put(mpiRank, arr_Bool);
    GenData(arr_Char, array_pos, mpiRank); array_Char.put(mpiRank, arr_Char);
    GenData(arr_uChar, array_pos, mpiRank); array_uChar.put(mpiRank, arr_uChar);
    GenData(arr_Short, array_pos, mpiRank); array_Short.put(mpiRank, arr_Short);
    GenData(arr_uShort, array_pos, mpiRank); array_uShort.put(mpiRank, arr_uShort);
    GenData(arr_Int, array_pos, mpiRank); array_Int.put(mpiRank, arr_Int);
    GenData(arr_uInt, array_pos, mpiRank); array_uInt.put(mpiRank, arr_uInt);
    GenData(arr_Float, array_pos, mpiRank); array_Float.put(mpiRank, arr_Float);
    GenData(arr_Double, array_pos, mpiRank); array_Double.put(mpiRank, arr_Double);
    GenData(arr_Complex, array_pos, mpiRank); array_Complex.put(mpiRank, arr_Complex);
    GenData(arr_DComplex, array_pos, mpiRank); array_DComplex.put(mpiRank, arr_DComplex);

    delete stman;
    delete tab;

    MPI_Finalize();

    return 0;
}


