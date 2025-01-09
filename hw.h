// IO
void print_initial_parameters(double initial_smoothed, double initial_trend, double *initial_seasonals, int season_length);
void print_indices(double *smoothed, double *trend, double *seasonals, int length);
void print_forecast(double *series, int series_length, double *forecast, int forecast_length);

// Initialization
double get_initial_smoothed(double *series, int series_length);
double get_initial_trend(int season_length, double *series, int series_length);
double* get_initial_seasonals(int season_length, double *series, int series_length);


// HW methods
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
        double *seasonals
);
