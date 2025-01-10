// Interface methods for Holt-Winters forecasting
double* forecast_auto(double *series, int series_length, int season_length, int forecast_length);
double* forecast_manual(double *series, int series_length, int season_length, int forecast_length, double alpha, double beta, double gamma);
