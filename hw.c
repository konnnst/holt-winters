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
