#include <string.h>
#include "postgresql/server/postgres.h"
#include "postgresql/server/fmgr.h"
#include "postgresql/server/utils/geo_decls.h"
#include "postgresql/server/utils/array.h"
#include "postgresql/server/utils/lsyscache.h"
#include "postgresql/server/utils/builtins.h"
#include <postgresql/server/catalog/pg_type_d.h>
#include <postgresql/server/executor/spi.h>

#include "hw_api.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif


//PG_FUNCTION_INFO_V1(add_two);
//
//Datum
//add_two(PG_FUNCTION_ARGS)
//{
//    int32 arg = PG_GETARG_INT32(0);
//
//    PG_RETURN_INT32(arg + 2);
//}

PG_FUNCTION_INFO_V1(hw_forecast_manual);


Datum
hw_forecast_manual(PG_FUNCTION_ARGS)
{
//    int32 series_length = PG_GETARG_INT32(0);
//    int32 season_length = PG_GETARG_INT32(1);
//    int32 forecast_length = PG_GETARG_INT32(2);
//
//    float8 alpha = PG_GETARG_FLOAT8(3);
//    float8 beta = PG_GETARG_FLOAT8(4);
//    float8 gamma = PG_GETARG_FLOAT8(5);
//    double array[3] = {1, 2, 3};

    SPI_execute("SELECT series FROM series_table", true, 0);
    SPI_tuptable->numvals;

    ArrayType *result;
    Datum numbers[3];
    int16 typlen;
    bool typbyval;
    char typalign;
    numbers[0] = 1;
    numbers[1] = 2;
    numbers[2] = 3;


    get_typlenbyvalalign(INT4OID, &typlen, &typbyval, &typalign);
    result = construct_array(numbers, 3, INT4OID, typlen, typbyval, typalign);

    PG_RETURN_ARRAYTYPE_P(result);
}
//
//
//PG_FUNCTION_INFO_V1(forecast_auto);
//
//Datum
//forecast_auto(PG_FUNCTION_ARGS)
//{
//    int32 series_length = PG_GETARG_INT32(0);
//    int32 season_length = PG_GETARG_INT32(1);
//    int32 forecast_length = PG_GETARG_INT32(2);
//
//    PG_RETURN_INT32(series_length + season_length + forecast_length);
//}
//