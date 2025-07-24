#ifndef EVENT_H
#define EVENT_H

#include <stdint.h> // Required for int32_t

/**
 * @brief Generic event type for the State Machine.
 *
 * This is defined as a fixed-width integer type (int32_t) to ensure portability
 * across different architectures. This allows any application to define
 * its own event enumerations and pass them as integers to the generic
 * state machine, with a guaranteed size.
 */
typedef int32_t event_t;

#endif // EVENT_H
