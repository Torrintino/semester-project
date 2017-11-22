// IR.h

#ifndef CODINGTAG_HARDWARE_IR_H_
#define CODINGTAG_HARDWARE_IR_H_ 1

#include <cstdint>
#include <functional>


namespace hardware {
namespace ir {


/** @brief initializes the IR functionallity.
 */
void initIR();


/** @brief sends data via the IR actor.
 *
 * sends the integer value _data via the IR actor. This function does not block.
 */
void sendIR(uint32_t _data);


/** @brief checks for incomming IR data and calls the callback function if required.
 *
 * checks whether new IR data is available and calls the function _func with the incoming data
 * as its argument. The argument is the integer some other device has called sendIR() with. If no
 * data is available this function returns immediately.
 */
void checkForIncomingIR(std::function<void(uint32_t)> _func);


} // namespace ir
} // namespace hardware


#endif // CODINGTAG_HARDWARE_IR_H_
