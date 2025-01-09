#include <stdio.h>


void print_initial_parameters(double initial_smoothed, double initial_trend, double *initial_seasonals, int season_length) {
    printf("Smoothed: %f\n", initial_smoothed);
    printf("Trend: %f\n", initial_trend);
    printf("Seasonals:\n");
    for (int i = 0; i < season_length; ++i) {
        printf("%i. %f\n", i + 1, initial_seasonals[i]);
    }
}

void print_indices(double *smoothed, double *trend, double *seasonals, int length) {
    printf("     %10s %10s %10s\n", "Smoothed", "Trend", "Seasonals");
    for (int i = 0; i < length; ++i) {
        printf("%4i %10f %10f %10f\n", i, smoothed[i], trend[i], seasonals[i]);
    }
}

void print_forecast(double *series, int series_length, double *forecast, int forecast_length) {
    printf("Series (%i) and forecast (%i):\n", series_length, forecast_length);
    for (int i = 0; i < series_length; ++i) {
        printf("%f ", series[i]);
    }
    printf("\n");
    for (int i = 0; i < forecast_length; ++i) {
        printf("%f ", forecast[i]);
    }
}
