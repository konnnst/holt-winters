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


void holt_winters_forecast(
        double *forecast,
        int forecast_length,

        double *smoothed,
        double *trend,
        double *seasonals,

        int series_length,
        int season_length
) {
    double last_seasonals[season_length];

    for (int i = 1; i <= season_length; ++i) {
        last_seasonals[(series_length - i) % season_length] = seasonals[series_length - i];
    }
    for (int i = 0; i < forecast_length; ++i) {
        double now_s = seasonals[series_length - 1];
        double now_b = trend[series_length - 1];
        double now_c = last_seasonals[(season_length + i + 1) % season_length];
        double forecast_i = (now_s + (i + 1) * now_b) * now_c;
        forecast[i] = forecast_i;
        //forecast[i] = (seasonals[series_length - 1] + (i + 1) * trend[series_length - 1])
            //* last_seasonals[(season_length + i + 1) % season_length];
    }
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
//    double series_arr[] = {
//        41.7275, 24.0418, 32.3281, 37.3287,
//        46.2132, 29.3463, 36.4829, 42.9777,
//        48.9015, 31.1802, 37.7179, 40.4202,
//        51.2069, 31.8872, 40.9783, 43.7725,
//        55.5586, 33.8509, 42.0764, 45.6423,
//        59.7668, 35.1919, 44.3197, 47.9137
//    };
//    int season_length = 4;

    double series_arr[] = {
        362, 385, 432, 341,
        382, 409, 498, 387,
        473, 513, 582, 474,
        544, 582, 681, 557,
        628, 707, 773, 592,
        627, 725, 854, 661
    };
    int season_length = 4;

    // Initial calculations
    double *series = series_arr;
    int series_length = sizeof(series_arr) / sizeof(double);

    double initial_smoothed = get_initial_smoothed(series, series_length);
    double initial_trend = get_initial_trend(season_length, series, series_length);
    double* initial_seasonals = get_initial_seasonals(season_length, series, series_length);

    double alpha = 0.7556;
    double beta = 0.000;
    double gamma = 0.9837;
    //double alpha = 1.4901161193847666e-08;
    //double beta = 1.5900321668960454e-09;
    //double gamma = 8.854717309361246e-27;

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
    free(forecast);
}
