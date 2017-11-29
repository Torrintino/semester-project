// IR.cpp

#include <atomic>
#include <iostream> // just for demo
#include <memory>
#include <queue>
#include <string> // just for demo
#include <thread>

#include <unistd.h> // just for demo

#include "IR.h"

namespace hardware {
namespace ir {


std::queue<uint32_t> send_queue;
std::queue<uint32_t> receive_queue;
std::atomic<bool> termination_requested;


void main_ir()
{
    uint32_t i = 0;

    // TODO: Hier kann moeglicherweise noch anderer Code hinzugefuegt werden, der zur
    // Initialisierung notwendig ist.
    std::cout << "IR initialized.\n";

    while (!termination_requested) {

        if (!send_queue.empty()) {
            // TODO: Hier kann beliebiger Code ausgefuehrt werden, um Daten zu senden. Zunaechst
            // werden hier nur zu sendende Daten ausgegeben.
            std::cout << ("Data sent: " + std::to_string(send_queue.front()) + "\n");
            send_queue.pop();
        }


        // TODO : Hier kann beliebiger Code ausgefuehrt werden, der zum Beispiel noetig waere um
        // nach neuen IR-Events zu pruefen. Hier wird beispielhaft simuliert, dass alle 3 Sekunden
        // eine Zahl entfangen wird.
        sleep(1);
        if (i % 3 == 0 && receive_queue.size() < 10) {
            receive_queue.push(i);
        }
        ++i;
    }

    // TODO: Hier kann moeglicherweise noch anderer Code hinzugefuegt werden, der zur Finalisierung
    // notwendig ist.
    std::cout << "IR finalized.\n";
}


class IR final
{
public:
    IR() : m_thread(main_ir) {}
    IR(IR const& _other) = delete;
    IR(IR&& _other) = delete;
    IR& operator=(IR const& _other) & = delete;
    IR& operator=(IR&& _other) & = delete;
    ~IR()
    {
        termination_requested = true;
        m_thread.join();
    }

private:
    std::thread m_thread;
};

std::unique_ptr<IR> g_ir;


void initIR()
{
    termination_requested = false;

    g_ir = std::make_unique<IR>();
}


void sendIR(uint32_t _data)
{
    if (send_queue.size() < 10)
        send_queue.push(_data);
}


void checkForIncomingIR(std::function<void(uint32_t)> _func)
{
    for (int i = 0; i < 4 && !receive_queue.empty(); ++i) {
        uint32_t argument = receive_queue.front();
        receive_queue.pop();
        _func(argument);
    }
}


} // namespace ir
} // namespace hardware
