//#include "postgresql/server/postgres.h"

#include <stdlib.h>
#include <stdio.h>

#include "build_options.h"
#include "hw_debug.h"
#include "hw_api.h"

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
    }, *series = series_arr;
    int series_length = sizeof(series_arr) / sizeof(double);
    int season_length = 4;
    int forecast_length = 16;


    double alpha = 1;
    double beta = 0;
    double gamma = 0;

    double *fc_man = forecast_manual(series, series_length, season_length, forecast_length, alpha, beta, gamma);
    print_series_and_forecast(fc_man, season_length, series_length, forecast_length);
    FREE(fc_man);

    printf("\n\n");

    double *fc_auto = forecast_auto(series, series_length, season_length, forecast_length);
    print_series_and_forecast(fc_auto, season_length, series_length, forecast_length);
    FREE(fc_auto);
}
