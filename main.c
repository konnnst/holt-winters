#include <stdio.h>
#include <stdlib.h>

double get_initial_smoothed(double *series, int series_length) {
    double smoothed  = series[0];
    return smoothed;
}

double get_initial_trend(int season_length, double *series, int series_length) {
    double trend = 0;
    
    for (int i = 0; i < season_length; ++i) {
        trend += series[season_length + i] - series[i];
    }

    return trend / (season_length * season_length);
}


double* get_initial_seasonals(int season_length, double *series, int series_length) {
    double* seasonals = malloc(sizeof(double) * season_length), cycle_sum;
    int cycles_count = series_length / season_length;

    for (int i = 0; i < season_length; ++i) {
        seasonals[i] = 0;
    }


    for (int i = 0; i < cycles_count; ++i) {
        cycle_sum = 0;

        for (int j = 0; j < season_length; ++j ) {
            cycle_sum += series[j + i * season_length];
        }

        for (int j = 0; j < season_length; ++j) {
            seasonals[j] += series[i * season_length + j] / cycle_sum;
        }
    }

    for (int i = 0; i < season_length; ++i) {
        seasonals[i] /= cycles_count;
    }

    return seasonals;
}

void print_initial_parameters(double initial_smoothed, double initial_trend, double *initial_seasonals, int season_length) {
    printf("Smoothed: %f\n", initial_smoothed);
    printf("Trend: %f\n", initial_trend);
    printf("Seasonals:\n");
    for (int i = 0; i < season_length; ++i) {
        printf("%i. %f\n", i + 1, initial_seasonals[i]);
    }
}

void print_indices(double *smoothed, double *trend, double *seasonals, int length) {
    printf("%10s %10s %10s\n", "Smoothed", "Trend", "Seasonals");
    for (int i = 0; i < length; ++i) {
        printf("%10f %10f %10f\n", smoothed[i], trend[i], seasonals[i]);
    }
}

void print_forecast(double *series, int series_length, double *forecast, int forecast_length) {
    printf("Series and forecast:");
    for (int i = 0; i < series_length; ++i) {
        printf("%f ", series[i]);
    }
    printf("\n");
    for (int i = 0; i < forecast_length; ++i) {
        printf("%f ", forecast[i]);
    }
}


void holt_winters_forecast(
        double *forecast,
        int forecast_length,

        double *smoothed,
        double *trend,
        double *seasonals,

        int series_length,
        int season_length
) {

}


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
) {

    smoothed[0] = initial_smoothed;
    trend[0] = initial_trend;
    for (int i = 0; i < season_length; ++i) {
        seasonals[i] = initial_seasonals[i];
    }

    // Compute for first period
    for (int i = 1; i < season_length; ++i) {
        smoothed[i] = alpha * series[i] / seasonals[i] + (1 - alpha) * (smoothed[i - 1] + trend[i - 1]);
        trend[i] = beta * (smoothed[i] - smoothed[i - 1]) + (1 - alpha) * trend[i - 1];
    }

    // Compute for later periods
    for (int i = season_length; i < series_length; ++i) {
        smoothed[i] = alpha * series[i] / seasonals[i - season_length] + (1 - alpha) * (smoothed[i - 1] + trend[i - 1]);
        trend[i] = beta * (smoothed[i] - smoothed[i - 1]) + (1 - alpha) * trend[i - 1];
        seasonals[i] = gamma * (series[i] / smoothed[i]) + (1 - gamma) * seasonals[i - season_length];
    }
}


int main() {
    // User data
    double series_arr[] = {
        41.7275, 24.0418, 32.3281, 37.3287,
        46.2132, 29.3463, 36.4829, 42.9777,
        48.9015, 31.1802, 37.7179, 40.4202,
        51.2069, 31.8872, 40.9783, 43.7725,
        55.5586, 33.8509, 42.0764, 45.6423,
        59.7668, 35.1919, 44.3197, 47.9137
    };
    int season_length = 4;

    // Initial calculations
    double *series = series_arr;
    int series_length = sizeof(series_arr) / sizeof(double);

    double initial_smoothed = get_initial_smoothed(series, series_length);
    double initial_trend = get_initial_trend(season_length, series, series_length);
    double* initial_seasonals = get_initial_seasonals(season_length, series, series_length);

    // double alpha = 1;
    // double beta = 1;
    // double gamma = 1;
    double alpha = 1.4901161193847666e-08;
    double beta = 1.5900321668960454e-09;
    double gamma = 8.854717309361246e-27;

    print_initial_parameters(initial_smoothed, initial_trend, initial_seasonals, season_length);


    // Getting smoothed indices for further predicting
    double *smoothed, *trend, *seasonals;
    smoothed = malloc(sizeof(double) * series_length);
    trend = malloc(sizeof(double) * series_length);
    seasonals = malloc(sizeof(double) * series_length);
    holt_winters(
        series,
        series_length,
        season_length,
        
        initial_smoothed,
        initial_trend,
        initial_seasonals,
        
        alpha,
        beta,
        gamma,

        smoothed,
        trend,
        seasonals
    );
    print_indices(smoothed, trend, seasonals, series_length);

    // Forecasting and printing
    int forecast_length = 8; 
    double *forecast = malloc(sizeof(double) * forecast_length);
    holt_winters_forecast(forecast, forecast_length, smoothed, trend, seasonals, series_length, season_length);
    print_forecast(series, series_length, forecast, forecast_length);

    free(initial_seasonals);
    free(smoothed);
    free(trend);
    free(seasonals);
}
