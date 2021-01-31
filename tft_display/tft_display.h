#ifndef TFT_DISPLAY_H
#define TFT_DISPLAY_H

#include <time.h>
#include "measure.h"
#include "tensorflow/lite/micro/examples/hello_world/main_functions.h"


void display_init(void);

void display_update(measurement_t *measurement, struct tm *date_time);

void display_prediction(predictions_t *predictions);

#endif