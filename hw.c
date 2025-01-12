#include <stdlib.h>
#include <string.h>

#include "postgresql/server/postgres.h"

#include "build_options.h"
#include "hw_debug.h"


double* forecast_auto(double *series, int series_length, int season_length, int forecast_length) {
    double *result = forecast(series, series_length, season_length, forecast_length, NULL, DEBUG);
    return result;
}

double* forecast_manual(double *series, int series_length, int season_length, int forecast_length, double alpha, double beta, double gamma) {
    double coefficients_arr[] = { alpha, beta, gamma }, *coefficients = coefficients_arr;
    double *result = forecast(series, series_length, season_length, forecast_length, coefficients, DEBUG);
    return result;
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
        double now_s = smoothed[series_length - 1];
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
        double *seasonals,
        
        double *error
) {

    smoothed[0] = initial_smoothed;
    trend[0] = initial_trend;
    for (int i = 0; i < season_length; ++i) {
        seasonals[i] = initial_seasonals[i];
    }

    // Compute for first period
    for (int i = 1; i < season_length; ++i) {
        smoothed[i] = alpha * series[i] / seasonals[i] + (1 - alpha) * (smoothed[i - 1] + trend[i - 1]);
        //smoothed[i] = alpha * series[i] + (1 - alpha) * (smoothed[i - 1] + trend[i - 1]);
        trend[i] = beta * (smoothed[i] - smoothed[i - 1]) + (1 - beta) * trend[i - 1];

        *error += (series[i] - smoothed[i]) * (series[i] - smoothed[i]);
    }

    // Compute for later periods
    for (int i = season_length; i < series_length; ++i) {
        smoothed[i] = alpha * series[i] / seasonals[i - season_length] + (1 - alpha) * (smoothed[i - 1] + trend[i - 1]);
        //smoothed[i] = alpha * series[i] + (1 - alpha) * (smoothed[i - 1] + trend[i - 1]);
        trend[i] = beta * (smoothed[i] - smoothed[i - 1]) + (1 - beta) * trend[i - 1];
        seasonals[i] = gamma * (series[i] / smoothed[i]) + (1 - gamma) * seasonals[i - season_length];

        *error += (series[i] - smoothed[i]) * (series[i] - smoothed[i]);
    }
}


double* forecast(double *series, int series_length, int season_length, int forecast_length, double *coefficients, int debug) {
        double initial_smoothed = get_initial_smoothed(series, series_length);
        double initial_trend = get_initial_trend(season_length, series, series_length);
        double* initial_seasonals = get_initial_seasonals(season_length, series, series_length);

        double *smoothed, *trend, *seasonals;
        smoothed = MALLOC(sizeof(double) * series_length);
        trend = MALLOC(sizeof(double) * series_length);
        seasonals = MALLOC(sizeof(double) * series_length);

        double zero = 0, *error, min_error = -1;
        error = &zero;
        double alpha_0, beta_0, gamma_0;
        
        // Calculate indices up 
        if (coefficients == NULL) {

            for (double alpha = 0; alpha <= 1; alpha += 0.005) {
                for (double beta = 0; beta <= 1; beta += 0.005) {
                    for (double gamma = 0; gamma <= 1; gamma += 0.005) {
                        holt_winters(
                            series,
                            series_length,
                            season_length,
                            
                            initial_smoothed,
                            initial_trend,
                            initial_seasonals,
                            
                            alpha, beta, gamma,

                            smoothed, trend, seasonals,

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
        } else {
            alpha_0 = coefficients[0];
            beta_0 = coefficients[1];
            gamma_0 = coefficients[2];
        }

        holt_winters(
            series,
            series_length,
            season_length,
            
            initial_smoothed,
            initial_trend,
            initial_seasonals,
            
            alpha_0, beta_0, gamma_0,

            smoothed, trend, seasonals,

            &min_error
        );

        double *forecast = MALLOC(sizeof(double) * forecast_length);
        holt_winters_forecast(
                forecast, forecast_length,
                smoothed, trend, seasonals,
                series_length, season_length
        );
        

        // Concatenate smoothed and forecasted values
        double *result = MALLOC(sizeof(double) * (series_length + forecast_length));
        for (int i = 0; i < series_length + forecast_length; ++i) {
            if (i < series_length) {
                int now = smoothed[i];
                result[i] = smoothed[i];
            } else {
                int now = forecast[i - series_length];
                result[i] = forecast[i - series_length];
            }
        }

        // Print debug information
        if (debug) {
            print_error(&min_error);
            print_coefficients(alpha_0, beta_0, gamma_0);
            print_indices(smoothed, trend, seasonals, series_length);
        }

        // Releasing dynamically allocated memory
        FREE(initial_seasonals);
        FREE(smoothed);
        FREE(trend);
        FREE(seasonals);
        FREE(forecast);

        return result;
}
