#include <IRQ.h>
#include <DescriptorTables.h>
#include <TextScreen.h>
#include <Task.h>

extern "C" void IRQ0_Timer(void) {
    DescriptorTables::SendToEOI(32);
    Task::SwitchTaskInInterrupt();
    Hal::Timer::Interrupt();
}

extern "C" void IRQ1_Keyboard(void) {
    DescriptorTables::SendToEOI(33);
    if(Hal::ReadPort(0x64) & 0x01) {
        Hal::Keyboard::ProcessAndPutToQueue(Hal::ReadPort(0x60));
    }
}

void IRQ12_Mouse(void) {
    DescriptorTables::SendToEOI(44);
    if(Hal::ReadPort(0x64) & 0x01) {
    	if(!(Hal::ReadPort(0x64) & 0x20)) {
    		;
    	}
    	else {
			Hal::Mouse::ProcessAndPutToQueue(Hal::ReadPort(0x60));
		}
    }
}