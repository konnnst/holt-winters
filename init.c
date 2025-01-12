#include <stdlib.h>
#include <stdio.h>

#include "build_options.h"
//#include "postgresql/server/postgres.h"

double get_initial_smoothed(double *series, int series_length) {
    double smoothed = series[0];
    return smoothed;
}

double get_initial_trend(int season_length, double *series, int series_length) {
    double trend = 0;
    
    for (int i = 0; i < season_length; ++i) {
        trend += series[season_length + i] - series[i];
    }
    
    trend /= season_length * season_length;
    return trend;
}


double* get_initial_seasonals(int season_length, double *series, int series_length) {
    int seasons_count = series_length / season_length;
    double *seasonals = MALLOC(sizeof(double) * season_length), season_sum, season_mean,
           *averages = MALLOC(sizeof(double) * seasons_count);

    for (int i = 0; i < seasons_count; ++i) {
        averages[i] = 0;
        for (int j = 0; j < season_length; ++j) {
            averages[i] += series[i * season_length + j];
        }
        averages[i] /= season_length;
    }

    for (int i = 0; i < season_length; ++i) {
        double sum_over_avg = 0;
        for (int j = 0; j < seasons_count; ++j) {
           sum_over_avg += series[season_length * j + i] - averages[j];
        }
        seasonals[i] = sum_over_avg / seasons_count;
    }

    return seasonals;
}
