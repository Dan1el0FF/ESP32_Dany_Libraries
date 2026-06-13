#include "GhostEncoder.h"

GhostEncoder::GhostEncoder(){
    current_pos=0.0f;
    counts=0;
}

void GhostEncoder::setup(gpio_num_t _step_pin, gpio_num_t _dir_pin,int _steps_per_mm,int _steps_per_rev){

    steps_per_mm=_steps_per_mm;
    steps_per_rev=_steps_per_rev;
    step_pin = _step_pin;
    dir_pin = _dir_pin;

    gpio_set_direction(step_pin,GPIO_MODE_OUTPUT);
    gpio_set_direction(dir_pin,GPIO_MODE_OUTPUT);

    const esp_timer_create_args_t timer_args = {
        .callback = [](void* arg) {
            // convertimos arg a GhostEncoder* y llamamos al método miembro
            static_cast<GhostEncoder*>(arg)->Timer_handler();
        },
        .arg             = this,
        .dispatch_method = ESP_TIMER_TASK,
        .name            = "GhostTimer"
    };
    esp_timer_create(&timer_args, &timerHandle);
}

void GhostEncoder::setwaypoint(float _desired_pos, int _frqz){  //coordenadas absolutas

    frqz = _frqz;
   // Detén movimiento previo
   if (moving) {
    esp_timer_stop(timerHandle);
    moving = false;
  }

  // Calcula pasos absolutos
  desired_pos = _desired_pos;
  float delta_mm = desired_pos - current_pos;
  int32_t target_steps = roundf(delta_mm * steps_per_mm);
  if (target_steps == 0) {
    direction = 0;
    return;
  }
  
  direction = (target_steps > 0) ? +1 : -1;
  steps_to_move = abs(target_steps);
  steps_done    = 0;

  // Fija pin de dirección
  gpio_set_level(dir_pin, (direction>0) ? 1 : 0);

  // Arranca el timer
  uint64_t half_period = (1000000ULL / uint64_t(frqz)) / 2ULL;
  esp_timer_start_periodic(timerHandle, half_period);
  moving = true;
}

void IRAM_ATTR GhostEncoder::Timer_handler() {
     // Alterna la línea de step
     flag = !flag;
     gpio_set_level(step_pin, flag);
   
     if (flag) {
       // Solo en flanco ascendente contamos un paso
       counts += direction;
       steps_done++;
       current_pos = float(counts) / float(steps_per_mm);
   
       // ¿Llegamos?
       if (steps_done >= steps_to_move) {
         esp_timer_stop(timerHandle);
         moving = false;
       }
     }
}

float GhostEncoder::getPos() {
    return current_pos;
}

void GhostEncoder::reset() {
    counts        = 0;
    current_pos   = 0.0f;
    direction     = 0;
    steps_done    = 0;
    steps_to_move = 0;
    moving        = false;
    flag          = false;
    esp_timer_stop(timerHandle);
}

float GhostEncoder::getspeed(){

    if (frqz <= 0 || steps_per_mm == 0) return 0.0f;
    // pasos por segundo = current_freq
    // mm por segundo = current_freq / steps_per_mm
    return float(frqz) / float(steps_per_mm);
}

void GhostEncoder::stop(){
  esp_timer_stop(timerHandle);
  moving = false;
  direction = 0;
  steps_done = 0;
  steps_to_move = 0;
  flag = false;
  gpio_set_level(step_pin,0);
}
