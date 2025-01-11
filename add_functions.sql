DROP FUNCTION hw_forecast_manual;
DROP FUNCTION hw_forecast_auto;

CREATE OR REPLACE FUNCTION hw_forecast_manual() RETURNS float8[]
     AS '/lib/pg_hw.so', 'hw_forecast_manual'
     LANGUAGE C STRICT;
CREATE OR REPLACE FUNCTION hw_forecast_auto() RETURNS float8[]
     AS '/lib/pg_hw.so', 'hw_forecast_auto'
     LANGUAGE C STRICT;