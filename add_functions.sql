DROP FUNCTION hw_forecast_manual;
DROP FUNCTION hw_forecast_auto;

CREATE OR REPLACE FUNCTION hw_forecast_manual(integer, integer, float8, float8, float8) RETURNS float8[]
     AS '/lib/pg_hw.so', 'hw_forecast_manual'
     LANGUAGE C STRICT;
CREATE OR REPLACE FUNCTION hw_forecast_auto(integer, integer) RETURNS float8[]
     AS '/lib/pg_hw.so', 'hw_forecast_auto'
     LANGUAGE C STRICT;
