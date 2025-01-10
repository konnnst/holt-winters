#include <stdlib.h>

#include "hw.h"

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

    double alpha = 0.8;
    double beta = 0.2;
    double gamma = 0.8;
    //double alpha = 1.4901161193847666e-08;
    //double beta = 1.5900321668960454e-09;
    //double gamma = 8.854717309361246e-27;

    print_initial_parameters(initial_smoothed, initial_trend, initial_seasonals, season_length);


    // Getting smoothed indices for further predicting
    double *smoothed, *trend, *seasonals;
    smoothed = malloc(sizeof(double) * series_length);
    trend = malloc(sizeof(double) * series_length);
    double zero = 0, *error;
    error = &zero;
    seasonals = malloc(sizeof(double) * series_length);

    double alpha_0 = 0;
    double beta_0 = 0;
    double gamma_0 = 0;
    double min_error = -1;

    for (alpha = 0; alpha <= 1; alpha += 0.005) {
        for (beta = 0; beta <= 1; beta += 0.005) {
            for (gamma = 0; gamma <= 1; gamma += 0.005) {
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
                    seasonals,

                    error
                );

                if (min_error < 0 || *error < min_error) {
                    alpha_0 = alpha;
                    beta_0 = beta;
                    gamma_0 = gamma;
                    min_error = *error;
                }
                *error = 0;
            }
        }
    }
    print_error(&min_error);
    print_coefficients(alpha_0, beta_0, gamma_0);
    //holt_winters(
    //    series,
    //    series_length,
    //    season_length,
    //    
    //    initial_smoothed,
    //    initial_trend,
    //    initial_seasonals,
    //    
    //    alpha,
    //    beta,
    //    gamma,

    //    smoothed,
    //    trend,
    //    seasonals,

    //    error
    //);
    //print_error(error);
    //print_indices(smoothed, trend, seasonals, series_length);

}
