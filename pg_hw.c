#include <string.h>
#include "postgresql/server/postgres.h"
#include "postgresql/server/fmgr.h"
#include "postgresql/server/utils/geo_decls.h"
#include "postgresql/server/utils/array.h"
#include "postgresql/server/utils/lsyscache.h"
#include "postgresql/server/utils/builtins.h"
#include <postgresql/server/catalog/pg_type_d.h>
#include <postgresql/server/executor/spi.h>
#include <postgresql/server/executor/executor.h>
#include "postgresql/server/funcapi.h"
#include <postgresql/server/access/htup.h>

#include "hw_api.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

#define MAX_SIZE 100000


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

    int status = 0, series_length = 0;
    if (SPI_connect() == SPI_OK_CONNECT && SPI_execute("SELECT series FROM series_table", true, 0) == SPI_OK_SELECT && SPI_tuptable != NULL) {
        series_length = (int)SPI_tuptable->numvals;
    }

    ArrayType *result;
    Datum series_prepared[MAX_SIZE];
    int16 typlen;
    bool typbyval;
    char typalign;
 
    TupleDesc tupdesc = SPI_tuptable->tupdesc;
    SPITupleTable *tuptable = SPI_tuptable;
    series_length = SPI_processed;
    for (int i = 0; i < series_length; i++) {
        HeapTuple tuple = tuptable->vals[i];
        series_prepared[i] = atof(SPI_getvalue(tuple, tupdesc, 1));
    }

    get_typlenbyvalalign(FLOAT8OID, &typlen, &typbyval, &typalign);
    result = construct_array(series_prepared, series_length, FLOAT8OID, typlen, typbyval, typalign);

    SPI_finish();

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