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
    double* seasonals = malloc(sizeof(double) * season_length), season_sum, season_mean;
    int seasons_count = series_length / season_length;

    for (int i = 0; i < season_length; ++i) {
        seasonals[i] = 0;
    }


    for (int i = 0; i < seasons_count; ++i) {
        season_sum = 0;
        for (int j = 0; j < season_length; ++j ) {
            season_sum += series[j + i * season_length];
        }
        season_mean = season_sum / season_length;

        for (int j = 0; j < season_length; ++j) {
            seasonals[j] += series[i * season_length + j] / season_mean;
        }
    }

    for (int i = 0; i < season_length; ++i) {
        seasonals[i] /= seasons_count;
    }

    return seasonals;
}

