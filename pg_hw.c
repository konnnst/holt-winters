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
    int32 season_length = PG_GETARG_INT32(0);
    int32 forecast_length = PG_GETARG_INT32(1);
//
//    float8 alpha = PG_GETARG_FLOAT8(3);
//    float8 beta = PG_GETARG_FLOAT8(4);
//    float8 gamma = PG_GETARG_FLOAT8(5);
//    double array[3] = {1, 2, 3};

    int status = 0, series_length = 0;
    if (SPI_connect() == SPI_OK_CONNECT && SPI_execute("SELECT series FROM series_table", true, 0) == SPI_OK_SELECT && SPI_tuptable != NULL) {
        series_length = (int)SPI_tuptable->numvals;
    }

    TupleDesc tupdesc = SPI_tuptable->tupdesc;
    SPITupleTable *tuptable = SPI_tuptable;

    series_length = SPI_processed;

    double *series = palloc(sizeof(double) * series_length);
    
    for (int i = 0; i < series_length; i++)
    {
        HeapTuple tuple = tuptable->vals[i];
        series[i] = atof(SPI_getvalue(tuple, tupdesc, 1));
    }

    double *forecast = forecast_manual(series, series_length, season_length, forecast_length, 0.95, 0, 0.95);

    elog(INFO, "ok");
    // Writeback to table
    ArrayType *result;
    Datum result_array[MAX_SIZE];
    int16 typlen;
    bool typbyval;
    char typalign;

    elog(INFO, "ok");
    for (int i = 0; i < series_length + forecast_length; ++i) {
        result_array[i] = forecast[i];
    }

    pfree(series);
    pfree(forecast);

    get_typlenbyvalalign(INT8OID, &typlen, &typbyval, &typalign);
    result = construct_array(result_array, series_length, INT8OID, typlen, typbyval, typalign);

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