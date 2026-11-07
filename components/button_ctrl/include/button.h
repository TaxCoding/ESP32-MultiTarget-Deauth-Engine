/**
 * @file button.h
 * @author TaxCoding (raja.naufal1605@student.unri.ac.id)
 * @date 2026
 * @brief Button FSM and LED indicator controller interface.
 */

#ifndef BUTTON_H
#define BUTTON_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    MODE_IDLE = 0,
    MODE_1 = 1,
    MODE_2 = 2,
    MODE_3 = 3
} system_mode_t;

/**
 * @brief Initialize button GPIO and LED D2 GPIO, then start the monitoring task.
 */
void button_ctrl_init(void);

/**
 * @brief Get the current active operational mode.
 * @return system_mode_t Current mode (0 to 3)
 */
system_mode_t button_ctrl_get_current_mode(void);

#ifdef __cplusplus
}
#endif

#endif // BUTTON_H