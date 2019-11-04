#include "utils.h"

unsigned long int no_of_calls = 0;
static uint8_t st;

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
	if (!lsb)
		return 1;

	*lsb = (uint8_t) val;

	return 0;
}

int (util_get_MSB)(uint16_t val, uint8_t *msb) {
	if (!msb)
		return 1;

	val = val >> 8;
	*msb = (uint8_t) val;

	return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  u32_t value32;

  if (sys_inb(port, &value32))
    return 1;

  *value = (uint8_t) value32;

  no_of_calls++;

  return 0;
}

uint8_t (kbc_send_cmd)(uint8_t port, uint8_t cmd) {        
    // This section waits until it can write to input buffer or it reaches a timeout state
    uint8_t i = TIMEOUT_ATTEMPTS;

    while (i) { // Tries for i attempts
        if (util_sys_inb(STAT_REG, &st)) // Read Status
            return 1;

        // Can only write if the ST_IN_BUF is set to 0
        if (st & ST_IN_BUF) {
            i--;
            
            if (tickdelay(micros_to_ticks(KBC_WAIT)))
                    return 1;
            continue;
        }

        else
            break;                
    }

    if (i == 0) // Timed out
            return 1;

    if (sys_outb(port, cmd)) // Write the command
            return 1;

    return 0;
}

uint8_t kbc_read_outbf(uint8_t port, uint8_t *content)
{
	// This section waits until there is something
	// to read from the output buffer
	// or it reaches a time out state
	uint8_t i = TIMEOUT_ATTEMPTS;
	while (i) // Tries for i attempts
	{
		//printf("Tries: %u\n", i);

		if (util_sys_inb(STAT_REG, &st)) // Read Status
  		return 1;

		// Can only read if the ST_OUT_BUF is set to 1
		if (st & ST_OUT_BUF)
		{
			break;
		}
		else
		{
			--i;
		  if (tickdelay(micros_to_ticks(KBC_WAIT)))
			 	return 1;
			continue;
		}		
	}
	if (i == 0) // Timed out
		return 1;

	if (util_sys_inb(OUT_BUF, content))
		return 1;
	
	return 0;

}
