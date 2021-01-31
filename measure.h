#ifndef MEASURE_H
#define MEASURE_H

typedef struct {
    float temperature;
    float pressure;
    float humidity;
} measurement_t;

typedef struct {
    float temperature_mean;
    float temperature_std;
    float pressure_mean;
    float pressure_std;
    float humidity_mean;
    float humidity_std;
} normalization_t;

void sensors_init(void);
void sensors_read(measurement_t *measurement);
void sensors_normalize(measurement_t *measurement, const normalization_t *norm_factors);

#endif