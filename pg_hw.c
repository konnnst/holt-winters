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
#include "build_options.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

#define MAX_SIZE 100000


PG_FUNCTION_INFO_V1(hw_forecast_manual);

Datum
hw_forecast_manual(PG_FUNCTION_ARGS)
{
    // Parsing the arguments
    int32 season_length = PG_GETARG_INT32(0);
    int32 forecast_length = PG_GETARG_INT32(1);

    float8 alpha = PG_GETARG_FLOAT8(3);
    float8 beta = PG_GETARG_FLOAT8(4);
    float8 gamma = PG_GETARG_FLOAT8(5);

    // Recieving information from the table
    int status = 0, series_length = 0;
    if (SPI_connect() == SPI_OK_CONNECT && SPI_execute("SELECT series FROM series_table", true, 0) == SPI_OK_SELECT && SPI_tuptable != NULL) {
        series_length = (int)SPI_tuptable->numvals;
    }

    TupleDesc tupdesc = SPI_tuptable->tupdesc;
    SPITupleTable *tuptable = SPI_tuptable;

    series_length = SPI_processed;

    double *series = MALLOC(sizeof(double) * series_length);
    
    for (int i = 0; i < series_length; i++)
    {
        HeapTuple tuple = tuptable->vals[i];
        series[i] = atof(SPI_getvalue(tuple, tupdesc, 1));
    }

    double *forecast = forecast_manual(series, series_length, season_length, forecast_length, alpha, beta, gamma);

    // Writeback to the table
    ArrayType *result;
    Datum result_array[MAX_SIZE];
    int16 typlen;
    bool typbyval;
    char typalign;

    for (int i = 0; i < series_length + forecast_length; ++i) {
        result_array[i] = forecast[i];
    }

    get_typlenbyvalalign(INT8OID, &typlen, &typbyval, &typalign);
    result = construct_array(result_array, series_length, INT8OID, typlen, typbyval, typalign);

    SPI_finish();

    // Releasing allocated memory
    PFREE(series);
    PFREE(forecast);

    PG_RETURN_ARRAYTYPE_P(result);
}


PG_FUNCTION_INFO_V1(hw_forecast_auto);

Datum
hw_forecast_auto(PG_FUNCTION_ARGS)
{
    // Parsing the arguments
    int32 season_length = PG_GETARG_INT32(0);
    int32 forecast_length = PG_GETARG_INT32(1);

    // Recieving information from the table
    int status = 0, series_length = 0;
    if (SPI_connect() == SPI_OK_CONNECT && SPI_execute("SELECT series FROM series_table", true, 0) == SPI_OK_SELECT && SPI_tuptable != NULL) {
        series_length = (int)SPI_tuptable->numvals;
    }

    TupleDesc tupdesc = SPI_tuptable->tupdesc;
    SPITupleTable *tuptable = SPI_tuptable;

    series_length = SPI_processed;

    double *series = MALLOC(sizeof(double) * series_length);
    
    for (int i = 0; i < series_length; i++)
    {
        HeapTuple tuple = tuptable->vals[i];
        series[i] = atof(SPI_getvalue(tuple, tupdesc, 1));
    }
    
    // Forecasting
    double *forecast = forecast_auto(series, series_length, season_length, forecast_length);

    // Writeback to the table
    ArrayType *result;
    Datum result_array[MAX_SIZE];
    int16 typlen;
    bool typbyval;
    char typalign;

    for (int i = 0; i < series_length + forecast_length; ++i) {
        result_array[i] = forecast[i];
    }

    get_typlenbyvalalign(INT8OID, &typlen, &typbyval, &typalign);
    result = construct_array(result_array, series_length, INT8OID, typlen, typbyval, typalign);

    SPI_finish();

    // Releasing allocated memory
    FREE(series);
    FREE(forecast);

    PG_RETURN_ARRAYTYPE_P(result);

}
