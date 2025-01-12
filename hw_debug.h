// IO
void print_coefficients(double alpha, double beta, double gamma);
void print_initial_parameters(double initial_smoothed, double initial_trend, double *initial_seasonals, int season_length);
void print_indices(double *smoothed, double *trend, double *seasonals, int length);
void print_forecast(double *series, int series_length, double *forecast, int forecast_length);
void print_error(double *error);
void print_series_and_forecast(double *series, int season_length, int series_length, int forecast_length);

// Initialization
double get_initial_smoothed(double *series, int series_length);
double get_initial_trend(int season_length, double *series, int series_length);
double* get_initial_seasonals(int season_length, double *series, int series_length);


// HW methods
double* forecast(double *series, int series_length, int season_length, int forecast_length, double *coefficients, int debug);
void holt_winters_forecast(
        double *forecast,
        int forecast_length,

        double *smoothed,
        double *trend,
        double *seasonals,

        int series_length,
        int season_length
);
void holt_winters(
        double *series,
        int series_length,
        int season_length,
        
        double initial_smoothed,
        double initial_trend,
        double* initial_seasonals,
        
        double alpha,
        double beta,
        double gamma,

        double *smoothed,
        double *trend,
        double *seasonals,

        double *error
);
void mse_coefficients(double *series, int series_length);
